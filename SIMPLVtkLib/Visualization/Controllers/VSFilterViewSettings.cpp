/* ============================================================================
 * Copyright (c) 2009-2015 BlueQuartz Software, LLC
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
 * contributors may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the followig contracts:
 *    United States Air Force Prime Contract FA8650-07-D-5800
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *    United States Prime Contract Navy N00173-07-C-2068
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "VSFilterViewSettings.h"

#include <QtWidgets/QColorDialog>
#include <QtWidgets/QInputDialog>

#include <vtkAbstractArray.h>
#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkDataSetMapper.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageProperty.h>
#include <vtkImageSliceMapper.h>
#include <vtkMapper.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkTextProperty.h>

#include "SIMPLVtkLib/Visualization/VisualFilters/VSAbstractDataFilter.h"
#include "SIMPLVtkLib/Visualization/VisualFilters/VSSIMPLDataContainerFilter.h"

double* VSFilterViewSettings::NULL_COLOR = new double[3]{0.0, 0.0, 0.0};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSFilterViewSettings::VSFilterViewSettings(VSAbstractFilter* filter)
: QObject(nullptr)
, m_ShowFilter(true)
{
  connectFilter(filter);
  setupActions();
  bool isSIMPL = dynamic_cast<VSSIMPLDataContainerFilter*>(filter);
  setupActors(isSIMPL);
  if(false == isSIMPL)
  {
    setRepresentation(Representation::Default);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSFilterViewSettings::VSFilterViewSettings(const VSFilterViewSettings& copy)
: QObject(nullptr)
, m_ShowFilter(copy.m_ShowFilter)
, m_ActiveArray(copy.m_ActiveArray)
, m_ActiveComponent(copy.m_ActiveComponent)
, m_MapColors(copy.m_MapColors)
, m_Alpha(copy.m_Alpha)
{
  connectFilter(copy.m_Filter);
  setupActions();
  setupActors();
  setScalarBarVisible(copy.isScalarBarVisible());
  setRepresentation(copy.getRepresentation());
  setActiveArrayIndex(copy.m_ActiveArray);
  setActiveComponentIndex(copy.m_ActiveComponent);
  setSolidColor(copy.getSolidColor());
  setPointSize(copy.getPointSize());

  if(copy.m_LookupTable)
  {
    // Create a new VSLookupTableController if necessary
    if(nullptr == m_LookupTable)
    {
      m_LookupTable = copy.m_LookupTable->deepCopy();
    }
    else
    {
      m_LookupTable->copy(*(copy.m_LookupTable));
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSFilterViewSettings::~VSFilterViewSettings()
{
  if(m_LookupTable)
  {
    delete m_LookupTable;
    m_LookupTable = nullptr;
  }

  m_Filter = nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSAbstractFilter* VSFilterViewSettings::getFilter()
{
  return m_Filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSFilterViewSettings::isValid() const
{
  bool valid = m_Mapper && m_Actor;
  return valid;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setupActions()
{
  m_SetColorAction = new QAction("Set Color", this);
  connect(m_SetColorAction, &QAction::triggered, [=] {
    QColor color = QColorDialog::getColor(getSolidQColor());
    if(color.isValid())
    {
      setSolidQColor(color);
    }
  });

  m_SetOpacityAction = new QAction("Set Opacity", this);

  // The step variable was introduced somewhere between v5.9 and v5.10.1
  // As the only valid values are between 0.0 and 1.0, step values of 1.0 do not make sense.
#if QT_VERSION >= QT_VERSION_CHECK(5,10,1)
  connect(m_SetOpacityAction, &QAction::triggered, [=] {
    setAlpha(QInputDialog::getDouble(nullptr,
      "Set Opacity for '" + getFilter()->getFilterName() + "'", "Opacity",
      getAlpha(), 0.0, 1.0, 2, nullptr, Qt::WindowFlags(), 0.1));
#else
  connect(m_SetOpacityAction, &QAction::triggered, [=] {
    setAlpha(QInputDialog::getDouble(nullptr, "Set Opacity for '" + getFilter()->getFilterName() + "'", "Opacity", getAlpha(), 0.0, 1.0, 2));
#endif
  });

  m_ToggleScalarBarAction = new QAction("Enable Scalar Bar", this);
  m_ToggleScalarBarAction->setCheckable(true);
  m_ToggleScalarBarAction->setChecked(true);
  connect(m_ToggleScalarBarAction, &QAction::toggled, [=](bool checked) {
    setScalarBarVisible(checked);
  });
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSFilterViewSettings::isVisible()
{
  return m_ShowFilter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSFilterViewSettings::isGridVisible()
{
  return m_GridVisible;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int VSFilterViewSettings::getActiveArrayIndex()
{
  return m_ActiveArray;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int VSFilterViewSettings::getActiveComponentIndex()
{
  return m_ActiveComponent;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int VSFilterViewSettings::getNumberOfComponents(int arrayIndex)
{
  if(nullptr == m_Filter->getOutput())
  {
    return -1;
  }

  vtkAbstractArray* array = nullptr;

  if(isPointData())
  {
    vtkPointData* pointData = m_Filter->getOutput()->GetPointData();
    array = pointData->GetAbstractArray(arrayIndex);
  }
  else
  {
    vtkCellData* cellData = m_Filter->getOutput()->GetCellData();
    array = cellData->GetAbstractArray(arrayIndex);
  }
  
  if(array)
  {
    return array->GetNumberOfComponents();
  }
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int VSFilterViewSettings::getNumberOfComponents(QString arrayName)
{
  if(nullptr == m_Filter->getOutput())
  {
    return -1;
  }

  const char* name = arrayName.toLatin1();
  vtkAbstractArray* array = nullptr;

  if(isPointData())
  {
    vtkPointData* pointData = m_Filter->getOutput()->GetPointData();
    array = pointData->GetAbstractArray(name);
  }
  else
  {
    vtkCellData* cellData = m_Filter->getOutput()->GetCellData();
    array = cellData->GetAbstractArray(name);
  }
  
  if(array)
  {
    return array->GetNumberOfComponents();
  }
  return 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Qt::CheckState VSFilterViewSettings::getMapColors()
{
  return m_MapColors;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
double VSFilterViewSettings::getAlpha()
{
  return m_Alpha;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSFilterViewSettings::isScalarBarVisible() const
{
  return m_ScalarBarWidget && m_ToggleScalarBarAction->isChecked();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VTK_PTR(vtkProp3D) VSFilterViewSettings::getActor()
{
  return m_Actor;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
vtkDataSetMapper* VSFilterViewSettings::getDataSetMapper() const
{
  if(ActorType::DataSet == m_ActorType && isValid())
  {
    return dynamic_cast<vtkDataSetMapper*>(m_Mapper.Get());
  }

  return nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
vtkActor* VSFilterViewSettings::getDataSetActor() const
{
  if(ActorType::DataSet == m_ActorType && isValid())
  {
    return vtkActor::SafeDownCast(m_Actor);
  }

  return nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
vtkImageSliceMapper* VSFilterViewSettings::getImageMapper() const
{
  if(ActorType::Image2D == m_ActorType && isValid())
  {
    return dynamic_cast<vtkImageSliceMapper*>(m_Mapper.Get());
  }

  return nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
vtkImageSlice* VSFilterViewSettings::getImageSliceActor() const
{
  if(ActorType::Image2D == m_ActorType && isValid())
  {
    return dynamic_cast<vtkImageSlice*>(m_Actor.Get());
  }

  return nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VTK_PTR(vtkScalarBarWidget) VSFilterViewSettings::getScalarBarWidget()
{
  return m_ScalarBarWidget;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VTK_PTR(vtkCubeAxesActor) VSFilterViewSettings::getCubeAxesActor()
{
  return m_CubeAxesActor;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::show()
{
  setVisible(true);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::hide()
{
  setVisible(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setVisible(bool visible)
{
  if(false == isValid())
  {
    return;
  }

  m_ShowFilter = visible;

  emit visibilityChanged(this, m_ShowFilter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setGridVisible(bool visible)
{
  if(false == isValid())
  {
    return;
  }

  m_GridVisible = visible;

  emit gridVisibilityChanged(this, m_GridVisible);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
vtkDataArray* VSFilterViewSettings::getArrayAtIndex(int index)
{
  vtkDataSet* dataSet = m_Filter->getOutput();
  if(nullptr == dataSet)
  {
    return nullptr;
  }

  if(isPointData())
  {
    if(dataSet->GetPointData())
    {
      return dataSet->GetPointData()->GetArray(index);
    }
  }
  else
  {
    if(dataSet->GetCellData())
    {
      return dataSet->GetCellData()->GetArray(index);
    }
  }

  return nullptr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setActiveArrayIndex(int index)
{
  if(nullptr == getDataSetMapper())
  {
    return;
  }

  // Draw a solid color if index is -1
  if(index == -1)
  {
    getDataSetMapper()->SelectColorArray(-1);
    m_ActiveArray = -1;

    emit activeArrayIndexChanged(this, m_ActiveArray);
    emit requiresRender();
    return;
  }

  VTK_PTR(vtkDataArray) dataArray = getArrayAtIndex(index);
  if(nullptr == dataArray)
  {
    return;
  }

  m_ActiveArray = index;

  emit activeArrayIndexChanged(this, m_ActiveArray);
  setActiveComponentIndex(-1);

  if(isColorArray(dataArray) && m_MapColors == Qt::Checked)
  {
    setMapColors(Qt::CheckState::PartiallyChecked);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setActiveComponentIndex(int index)
{
  vtkDataSetMapper* mapper = getDataSetMapper();
  if(nullptr == mapper)
  {
    return;
  }

  m_ActiveComponent = index;

  VTK_PTR(vtkScalarsToColors) lookupTable = mapper->GetLookupTable();
  VTK_PTR(vtkDataArray) dataArray = getArrayAtIndex(m_ActiveArray);
  if(nullptr == dataArray)
  {
    return;
  }

  // Clamp index with lower bound
  if(index < -1)
  {
    index = -1;
  }

  int numComponents = dataArray->GetNumberOfComponents();
  mapper->ColorByArrayComponent(m_ActiveArray, index);
  
  if(isPointData())
  {
    mapper->SetScalarModeToUsePointFieldData();
  }
  else
  {
    mapper->SetScalarModeToUseCellFieldData();
  }
  updateColorMode();

  if(numComponents == 1)
  {
    double* range = dataArray->GetRange();
    m_LookupTable->setRange(range);
    m_ScalarBarActor->SetTitle(dataArray->GetName());
  }
  else if(index == -1)
  {
    double* range = dataArray->GetRange(-1);
    QString dataArrayName = QString(dataArray->GetName());
    QString componentName = dataArrayName + " Magnitude";

    m_LookupTable->setRange(range);
    m_ScalarBarActor->SetTitle(qPrintable(componentName));
  }
  else if(index < numComponents)
  {
    double* range = dataArray->GetRange(index);
    m_LookupTable->setRange(range);
    m_ScalarBarActor->SetTitle(dataArray->GetComponentName(index));
  }

  m_Mapper->Update();

  emit activeComponentIndexChanged(this, m_ActiveComponent);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
vtkDataArray* VSFilterViewSettings::getDataArray()
{
  if(nullptr == m_Filter->getOutput())
  {
    return nullptr;
  }

  return getArrayAtIndex(m_ActiveArray);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSFilterViewSettings::isColorArray(vtkDataArray* dataArray)
{
  if(nullptr == dataArray)
  {
    return false;
  }

  if(dataArray->GetNumberOfComponents() == 3)
  {
    QString dataType = dataArray->GetDataTypeAsString();
    if(dataType.compare("unsigned char") == 0)
    {
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::updateColorMode()
{
  vtkDataSetMapper* mapper = getDataSetMapper();
  if(nullptr == mapper)
  {
    return;
  }

  vtkDataArray* dataArray = getDataArray();
  bool unmapColorArray = isColorArray(dataArray) && (m_ActiveComponent == -1);

  if(m_MapColors && !unmapColorArray)
  {
    mapper->SetColorModeToMapScalars();
  }
  else
  {
    mapper->SetColorModeToDirectScalars();
  }

  mapper->Update();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setMapColors(Qt::CheckState mapColorState)
{
  if(false == isValid())
  {
    return;
  }

  m_MapColors = mapColorState;

  updateColorMode();
  emit mapColorsChanged(this, m_MapColors);
  emit requiresRender();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setAlpha(double alpha)
{
  if(alpha < 0.0)
  {
    alpha = 0.0;
  }
  else if(alpha > 1.0)
  {
    alpha = 1.0;
  }

  m_Alpha = alpha;

  switch(m_ActorType)
  {
  case ActorType::DataSet:
    updateDataSetAlpha();
    break;
  case ActorType::Image2D:
    updateImageAlpha();
    break;
  default:
    return;
  }

  emit alphaChanged(this, m_Alpha);
  emit requiresRender();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::updateDataSetAlpha()
{
  vtkActor* actor = getDataSetActor();
  if(nullptr == actor)
  {
    return;
  }

  vtkProperty* property = actor->GetProperty();
  property->SetOpacity(m_Alpha);
  actor->SetProperty(property);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::updateImageAlpha()
{
  vtkImageSlice* actor = getImageSliceActor();
  if(nullptr == actor)
  {
    return;
  }

  vtkImageProperty* property = actor->GetProperty();
  property->SetOpacity(m_Alpha);
  actor->SetProperty(property);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::invertScalarBar()
{
  if(false == isValid() || nullptr == m_LookupTable)
  {
    return;
  }

  m_LookupTable->invert();
  emit requiresRender();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::loadPresetColors(const QJsonObject& colors)
{
  if(false == isValid() || nullptr == m_LookupTable)
  {
    return;
  }

  m_LookupTable->parseRgbJson(colors);
  emit requiresRender();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setScalarBarVisible(bool visible)
{
  if(false == isValid())
  {
    return;
  }

  m_ToggleScalarBarAction->setChecked(visible);
  
  emit showScalarBarChanged(this, visible);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setupActors(bool outline)
{
  VTK_PTR(vtkDataSet) outputData = m_Filter->getOutput();
  if(nullptr == outputData)
  {
    m_ShowFilter = false;
    return;
  }

  if(isFlatImage() && hasSinglePointArray())
  {
    setupImageActors();
  }
  else
  {
    setupDataSetActors();

    if(isFlatImage())
    {
      setScalarBarVisible(false);
    }
  }

  setupCubeAxesActor();

  if(outline)
  {
    setRepresentation(Representation::Outline);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSFilterViewSettings::isFlatImage()
{
  VTK_PTR(vtkDataSet) outputData = m_Filter->getOutput();
  if(nullptr == outputData)
  {
    return false;
  }

  if(m_Filter->getOutputType() != VSAbstractFilter::dataType_t::IMAGE_DATA)
  {
    return false;
  }

  vtkImageData* imageData = dynamic_cast<vtkImageData*>(outputData.Get());
  if(nullptr == imageData)
  {
    return false;
  }

  // Check extents
  int* extent = imageData->GetExtent();
  for(int i = 0; i < 3; i++)
  {
    if(extent[2 * i + 1] - extent[2 * i] <= 1)
    {
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSFilterViewSettings::hasSinglePointArray()
{
  VTK_PTR(vtkDataSet) outputData = m_Filter->getOutput();
  if(nullptr == outputData)
  {
    return false;
  }

  if(outputData->GetPointData()->GetNumberOfArrays() != 1)
  {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setupImageActors()
{
  vtkImageSliceMapper* mapper;
  vtkImageSlice* actor;
  if(ActorType::DataSet == m_ActorType || nullptr == m_Actor)
  {
    mapper = vtkImageSliceMapper::New();
    mapper->SliceAtFocalPointOn();
    mapper->SliceFacesCameraOff();

    actor = vtkImageSlice::New();
    actor->SetMapper(mapper);

    setMapColors(Qt::Unchecked);
    setScalarBarVisible(false);
  }
  else
  {
    mapper = vtkImageSliceMapper::SafeDownCast(m_Mapper);
    actor = vtkImageSlice::SafeDownCast(m_Actor);
  }

  mapper->SetInputConnection(m_Filter->getOutputPort());

  if(ActorType::DataSet == m_ActorType && isVisible())
  {
    emit swappingActors(m_Actor.Get(), actor);
  }

  m_Mapper = mapper;
  m_Actor = actor;

  m_ActorType = ActorType::Image2D;
  updateTransform();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setupDataSetActors()
{
  VTK_PTR(vtkDataSet) outputData = m_Filter->getOutput();

  vtkDataSetMapper* mapper;
  vtkActor* actor;
  if(ActorType::Image2D == m_ActorType || nullptr == m_Actor)
  {
    m_DataSetFilter = VTK_PTR(vtkDataSetSurfaceFilter)::New();
    m_OutlineFilter = VTK_PTR(vtkOutlineFilter)::New();
    mapper = vtkDataSetMapper::New();
    mapper->ReleaseDataFlagOn();
    actor = vtkActor::New();

    m_LookupTable = new VSLookupTableController();
    mapper->SetLookupTable(m_LookupTable->getColorTransferFunction());

    m_ScalarBarActor = VTK_PTR(vtkScalarBarActor)::New();
    m_ScalarBarActor->SetLookupTable(mapper->GetLookupTable());
    m_ScalarBarWidget = VTK_PTR(vtkScalarBarWidget)::New();
    m_ScalarBarWidget->SetScalarBarActor(m_ScalarBarActor);

    // Scalar Bar Title
    vtkTextProperty* titleProperty = m_ScalarBarActor->GetTitleTextProperty();
    titleProperty->SetJustificationToCentered();
    titleProperty->SetFontSize(titleProperty->GetFontSize() * 1.5);

    // Introduced in 7.1.0rc1, prevents resizing title to fill width
#if VTK_MAJOR_VERSION > 7 || (VTK_MAJOR_VERSION == 7 && VTK_MINOR_VERSION >= 1)
    m_ScalarBarActor->UnconstrainedFontSizeOn();
#endif

    m_ScalarBarActor->SetTitleRatio(0.75);
  }
  else
  {
    mapper = vtkDataSetMapper::SafeDownCast(m_Mapper);
    actor = vtkActor::SafeDownCast(m_Actor);
  }

  m_DataSetFilter->SetInputConnection(m_Filter->getTransformedOutputPort());
  m_OutlineFilter->SetInputConnection(m_Filter->getOutputPort());

  mapper->SetInputConnection(m_DataSetFilter->GetOutputPort());
  actor->SetMapper(mapper);

  // Check if there are any arrays to use
  bool hasArrays = false;
  if(isPointData())
  {
    hasArrays = outputData->GetPointData()->GetNumberOfArrays() > 0;
  }
  else
  {
    hasArrays = outputData->GetCellData()->GetNumberOfArrays() > 0;
  }

  // Set Mapper to use the active array and component
  if(hasArrays)
  {
    if(m_HadNoArrays)
    {
      setActiveArrayIndex(0);
      m_HadNoArrays = false;
    }
    else
    {
      setActiveArrayIndex(m_ActiveArray);
      setActiveComponentIndex(m_ActiveComponent);
    }
  }
  else
  {
    setMapColors(Qt::Unchecked);
    setScalarBarVisible(false);
    m_HadNoArrays = true;
  }

  if(ActorType::Image2D == m_ActorType && isVisible())
  {
    emit swappingActors(m_Actor.Get(), actor);
  }

  m_Mapper = mapper;
  m_Actor = actor;

  m_ActorType = ActorType::DataSet;
  updateTransform();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setupCubeAxesActor()
{
  if(nullptr == m_CubeAxesActor)
  {
    m_CubeAxesActor = VTK_PTR(vtkCubeAxesActor)::New();

    m_CubeAxesActor->XAxisMinorTickVisibilityOff();
    m_CubeAxesActor->YAxisMinorTickVisibilityOff();
    m_CubeAxesActor->ZAxisMinorTickVisibilityOff();
  }

  if(m_Filter && m_Filter->getOutput())
  {
    double* bounds = m_Filter->getOutput()->GetBounds();
    m_CubeAxesActor->SetBounds(bounds);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::updateInputPort(VSAbstractFilter* filter)
{
  if(false == isValid())
  {
    return;
  }

  if(m_DataSetFilter)
  {
    m_DataSetFilter->SetInputConnection(filter->getTransformedOutputPort());
    m_DataSetFilter->Update();
  }
  else
  {
    m_Mapper->SetInputConnection(filter->getOutputPort());
    m_Actor->SetUserTransform(m_Filter->getTransform()->getGlobalTransform());
  }
  emit requiresRender();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::updateTransform()
{
  if(false == (m_Filter && m_Filter->getTransform() && m_Actor))
  {
    return;
  }

  if(ActorType::Image2D == m_ActorType || Representation::Outline == m_Representation)
  {
    VSTransform* transform = m_Filter->getTransform();
    m_Actor->SetPosition(transform->getPosition());
    m_Actor->SetOrientation(transform->getRotation());
    m_Actor->SetScale(transform->getScale());
  }
  else
  {
    m_Actor->SetPosition(0.0, 0.0, 0.0);
    m_Actor->SetOrientation(0.0, 0.0, 0.0);
    m_Actor->SetScale(1.0, 1.0, 1.0);
  }

  emit requiresRender();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::connectFilter(VSAbstractFilter* filter)
{
  if(m_Filter)
  {
    disconnect(m_Filter, SIGNAL(updatedOutputPort(VSAbstractFilter*)), this, SLOT(updateInputPort(VSAbstractFilter*)));
    disconnect(m_Filter, SIGNAL(transformChanged()), this, SIGNAL(updateTransform()));
    disconnect(m_Filter, &VSAbstractFilter::removeFilter, this, &VSFilterViewSettings::filterDeleted);

    if(dynamic_cast<VSAbstractDataFilter*>(m_Filter))
    {
      disconnect(m_Filter, SIGNAL(dataImported()), this, SLOT(importedData()));
      disconnect(m_Filter, SIGNAL(dataReloaded()), this, SLOT(reloadedData()));
    }
  }

  m_Filter = filter;
  if(filter)
  {
    connect(filter, SIGNAL(updatedOutputPort(VSAbstractFilter*)), this, SLOT(updateInputPort(VSAbstractFilter*)));
    connect(filter, SIGNAL(transformChanged()), this, SLOT(updateTransform()));
    connect(filter, &VSAbstractFilter::removeFilter, this, &VSFilterViewSettings::filterDeleted);

    if(filter->getArrayNames().size() < 1)
    {
      setScalarBarVisible(false);
      setMapColors(Qt::Unchecked);
      m_ActiveArray = -1;
      m_HadNoArrays = true;
    }
    else
    {
      m_HadNoArrays = false;
    }

    if(dynamic_cast<VSAbstractDataFilter*>(filter))
    {
      connect(filter, SIGNAL(dataImported()), this, SLOT(importedData()));
      connect(filter, SIGNAL(dataReloaded()), this, SLOT(reloadedData()));
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::filterDeleted()
{
  setVisible(false);
  deleteLater();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
double* VSFilterViewSettings::getSolidColor() const
{
  vtkActor* actor = getDataSetActor();
  if(nullptr == actor)
  {
    return NULL_COLOR;
  }

  return actor->GetProperty()->GetColor();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setSolidColor(double color[3])
{
  vtkActor* actor = getDataSetActor();
  if(nullptr == actor)
  {
    return;
  }

  actor->GetProperty()->SetColor(color);

  emit solidColorChanged(this, color);
  emit requiresRender();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QColor VSFilterViewSettings::getSolidQColor() const
{
  QColor colorOut;
  double* color = getSolidColor();
  colorOut.setRgbF(color[0], color[1], color[2]);
  return colorOut;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setSolidQColor(QColor color)
{
  double dColor[3];
  dColor[0] = color.redF();
  dColor[1] = color.greenF();
  dColor[2] = color.blueF();

  setSolidColor(dColor);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSFilterViewSettings::Representation VSFilterViewSettings::getRepresentation() const
{
  return m_Representation;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int VSFilterViewSettings::getRepresentationi() const
{
  return static_cast<int>(getRepresentation());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSFilterViewSettings::ActorType VSFilterViewSettings::getActorType() const
{
  return m_ActorType;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSFilterViewSettings::isPointData()
{
  return m_Filter->isPointData();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSFilterViewSettings::isRenderingPoints()
{
  return isPointData() || Representation::Points == getRepresentation();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int VSFilterViewSettings::getPointSize() const
{
  if(ActorType::DataSet == getActorType() && getDataSetActor())
  {
    return getDataSetActor()->GetProperty()->GetPointSize();
  }

  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setPointSize(int pointSize)
{
  if(ActorType::DataSet == getActorType() && getDataSetActor())
  {
    getDataSetActor()->GetProperty()->SetPointSize(pointSize);
    emit pointSizeChanged(this, pointSize);
    emit requiresRender();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSFilterViewSettings::renderPointsAsSpheres() const
{
  if(ActorType::DataSet == getActorType() && getDataSetActor())
  {
    return getDataSetActor()->GetProperty()->GetRenderPointsAsSpheres();
  }

  return false;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setRenderPointsAsSpheres(bool renderSpheres)
{
  if(ActorType::DataSet == getActorType() && getDataSetActor())
  {
    getDataSetActor()->GetProperty()->SetRenderPointsAsSpheres(renderSpheres);
    emit renderPointSpheresChanged(this, renderSpheres);
    emit requiresRender();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::setRepresentation(Representation type)
{
  vtkActor* actor = getDataSetActor();
  if(nullptr == actor)
  {
    m_Representation = Representation::Invalid;
    return;
  }

  m_Representation = type;
  if(type == Representation::Outline)
  {
    getDataSetMapper()->SetInputConnection(m_OutlineFilter->GetOutputPort());
    actor->GetProperty()->SetRepresentation(static_cast<int>(Representation::Wireframe));
  }
  else
  {
    getDataSetMapper()->SetInputConnection(m_DataSetFilter->GetOutputPort());

    if(type == Representation::SurfaceWithEdges)
    {
      actor->GetProperty()->SetRepresentation(static_cast<int>(Representation::Surface));
      actor->GetProperty()->EdgeVisibilityOn();
    }
    else
    {
      actor->GetProperty()->SetRepresentation(static_cast<int>(type) - 1);
      actor->GetProperty()->EdgeVisibilityOff();
    }
  }

  updateTransform();
  emit representationChanged(this, type);
  emit requiresRender();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::importedData()
{
  if(dynamic_cast<VSAbstractDataFilter*>(getFilter()) && getRepresentation() == Representation::Outline)
  {
    setupActors(false);
    if(getImageMapper() && getDataSetActor())
    {
      getDataSetActor()->VisibilityOff();
      getDataSetMapper()->ScalarVisibilityOff();
    }
    setRepresentation(Representation::Surface);
    emit requiresRender();
  }

  emit dataLoaded();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::checkDataType()
{
  setupActors(false);
  emit requiresRender();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::reloadedData()
{
  setupActors(false);
  emit actorsUpdated();
  emit dataLoaded();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterViewSettings::copySettings(VSFilterViewSettings* copy)
{
  if((nullptr == copy) || (false == copy->isValid()))
  {
    return;
  }

  bool hasUi = copy->getScalarBarWidget();
  if(hasUi && m_ScalarBarWidget)
  {
    vtkRenderWindowInteractor* iren = copy->m_ScalarBarWidget->GetInteractor();
    m_ScalarBarWidget->SetInteractor(iren);
  }

  setVisible(copy->m_ShowFilter);
  setActiveArrayIndex(copy->m_ActiveArray);
  setActiveComponentIndex(copy->m_ActiveComponent);
  setMapColors(copy->m_MapColors);
  setScalarBarVisible(copy->m_ToggleScalarBarAction->isChecked());
  setAlpha(copy->m_Alpha);
  setSolidColor(copy->getSolidColor());
  setRepresentation(copy->getRepresentation());
  setPointSize(copy->getPointSize());

  if(hasUi && m_ScalarBarWidget)
  {
    m_LookupTable->copy(*(copy->m_LookupTable));
  }

  emit requiresRender();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QMenu* VSFilterViewSettings::getRepresentationMenu()
{
  QMenu* representationMenu = new QMenu("Representation");

  QAction* outlineAction = representationMenu->addAction("Outline");
  connect(outlineAction, &QAction::triggered, [=] { setRepresentation(VSFilterViewSettings::Representation::Outline); });

  QAction* pointsAction = representationMenu->addAction("Points");
  connect(pointsAction, &QAction::triggered, [=] { setRepresentation(VSFilterViewSettings::Representation::Points); });

  QAction* wireframeAction = representationMenu->addAction("WireFrame");
  connect(wireframeAction, &QAction::triggered, [=] { setRepresentation(VSFilterViewSettings::Representation::Wireframe); });

  QAction* surfaceAction = representationMenu->addAction("Surface");
  connect(surfaceAction, &QAction::triggered, [=] { setRepresentation(VSFilterViewSettings::Representation::Surface); });

  QAction* surfEdgesAction = representationMenu->addAction("Surface with Edges");
  connect(surfEdgesAction, &QAction::triggered, [=] { setRepresentation(VSFilterViewSettings::Representation::SurfaceWithEdges); });

  return representationMenu;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QMenu* VSFilterViewSettings::getColorByMenu()
{
  QMenu* arrayMenu = new QMenu("Colory By");
  QStringList arrayNames = getFilter()->getArrayNames();
  int numArrays = arrayNames.size();

  QAction* colorAction = arrayMenu->addAction("Solid Color");
  connect(colorAction, &QAction::triggered, [=] { setActiveArrayIndex(-1); });

  for(int i = 0; i < numArrays; i++)
  {
    int currentIndex = i;
    QAction* arrayAction = arrayMenu->addAction(arrayNames[i]);
    connect(arrayAction, &QAction::triggered, [=] { setActiveArrayIndex(currentIndex); });
  }

  return arrayMenu;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QMenu* VSFilterViewSettings::getMapScalarsMenu()
{
  QMenu* mapScalarsMenu = new QMenu("Map Scalars");

  QAction* mapAllAction = mapScalarsMenu->addAction("Map All Arrays");
  connect(mapAllAction, &QAction::triggered, [=] { setMapColors(Qt::Checked); });

  QAction* noMapAction = mapScalarsMenu->addAction("Do Not Map Arrays");
  connect(noMapAction, &QAction::triggered, [=] { setMapColors(Qt::Unchecked); });

  QAction* semiMapAction = mapScalarsMenu->addAction("Map Non-Color Arrays");
  connect(semiMapAction, &QAction::triggered, [=] { setMapColors(Qt::PartiallyChecked); });

  return mapScalarsMenu;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QAction* VSFilterViewSettings::getSetColorAction()
{
  return m_SetColorAction;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QAction* VSFilterViewSettings::getSetOpacityAction()
{
  return m_SetOpacityAction;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QAction* VSFilterViewSettings::getToggleScalarBarAction()
{
  return m_ToggleScalarBarAction;
}
