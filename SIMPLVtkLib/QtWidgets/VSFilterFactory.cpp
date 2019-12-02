/* ============================================================================
 * Copyright (c) 2009-2017 BlueQuartz Software, LLC
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of BlueQuartz Software, the US Air Force, nor the names of
 * its contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The code contained herein was partially funded by the followig contracts:
 *    United States Air Force Prime Contract FA8650-07-D-5800
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *    United States Prime Contract Navy N00173-07-C-2068
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "VSFilterFactory.h"

#include <array>

#include <QtWidgets/QMessageBox>

#include "SIMPLib/DataContainers/DataContainerArrayProxy.h"
#include "SIMPLib/FilterParameters/FloatVec2FilterParameter.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/IntVec2FilterParameter.h"
#include "SIMPLib/FilterParameters/MontageFileListInfo.h"
#include "SIMPLib/Filtering/FilterFactory.hpp"
#include "SIMPLib/Filtering/FilterManager.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSFilterFactory::VSFilterFactory(QObject* parent)
: QObject(parent)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createDataContainerReaderFilter(const QString& inputFile, const DataContainerArrayProxy& inputFileDCAProxy)
{
  QString filterName = "DataContainerReader";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer dataContainerReader = factory->create();
    if(dataContainerReader.get() != nullptr)
    {
      QVariant var;

      // Set input file
      var.setValue(inputFile);
      if(!setFilterProperty(dataContainerReader, "InputFile", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set data container array proxy
      var.setValue(inputFileDCAProxy);
      if(!setFilterProperty(dataContainerReader, "InputFileDataContainerArrayProxy", var))
      {
        return AbstractFilter::NullPointer();
      }
    }
    return dataContainerReader;
  }
  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createDataContainerWriterFilter(const QString& outputFile, bool writeXdmfFile, bool writeTimeSeriesMarkers)
{
  QString filterName = "DataContainerWriter";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer dataContainerWriter = factory->create();
    if(dataContainerWriter.get() != nullptr)
    {
      QVariant var;

      // Set output file
      var.setValue(outputFile);
      if(!setFilterProperty(dataContainerWriter, "OutputFile", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set whether to write Xdmf file
      var.setValue(writeXdmfFile);
      if(!setFilterProperty(dataContainerWriter, "WriteXdmfFile", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set whether to include Xdmf time markers
      var.setValue(writeTimeSeriesMarkers);
      if(!setFilterProperty(dataContainerWriter, "WriteTimeSeries", var))
      {
        return AbstractFilter::NullPointer();
      }
    }
    return dataContainerWriter;
  }
  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createSetOriginResolutionFilter(const DataArrayPath& dcPath, bool changeResolution, bool changeOrigin, FloatVec3Type resolution, FloatVec3Type origin)
{
  QString filterName = "SetOriginResolutionImageGeom";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer setOriginResolutionFilter = factory->create();
    if(setOriginResolutionFilter.get() != nullptr)
    {
      QVariant var;

      // Set the data container name
      var.setValue(dcPath);
      if(!setFilterProperty(setOriginResolutionFilter, "DataContainerName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the change spacing boolean flag (change resolution)
      var.setValue(changeResolution);
      if(!setFilterProperty(setOriginResolutionFilter, "ChangeResolution", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the spacing
      var.setValue(resolution);
      if(!setFilterProperty(setOriginResolutionFilter, "Spacing", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the change origin boolean flag (change resolution)
      var.setValue(changeOrigin);
      if(!setFilterProperty(setOriginResolutionFilter, "ChangeOrigin", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the origin
      var.setValue(origin);
      if(!setFilterProperty(setOriginResolutionFilter, "Origin", var))
      {
        return AbstractFilter::NullPointer();
      }

      return setOriginResolutionFilter;
    }
  }
  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createImportFijiMontageFilter(const FijiMontageMetadata& metadata, const DataArrayPath& dcPath, const QString& amName, const QString& daName)
{
  QString filterName = "ITKImportFijiMontage";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer importFijiMontageFilter = factory->create();
    if(importFijiMontageFilter.get() != nullptr)
    {
      QVariant var;

      // Set the path for the Fiji Configuration File
      var.setValue(metadata.getConfigFilePath());
      if(!setFilterProperty(importFijiMontageFilter, "InputFile", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set montage start
      var.setValue(metadata.getRowLimits());
      if(!setFilterProperty(importFijiMontageFilter, "RowMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set montage end
      var.setValue(metadata.getColumnLimits());
      if(!setFilterProperty(importFijiMontageFilter, "ColumnMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set Origin
      bool changeOrigin = metadata.getChangeOrigin();
      var.setValue(changeOrigin);
      if(!setFilterProperty(importFijiMontageFilter, "ChangeOrigin", var))
      {
        return AbstractFilter::NullPointer();
      }

      if(changeOrigin)
      {
        FloatVec3Type origin = metadata.getOrigin();
        var.setValue(origin);
        if(!setFilterProperty(importFijiMontageFilter, "Origin", var))
        {
          return AbstractFilter::NullPointer();
        }
      }

      // Set the Data Container Prefix
      var.setValue(dcPath);
      if(!setFilterProperty(importFijiMontageFilter, "DataContainerPath", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Cell Attribute Matrix Name
      var.setValue(amName);
      if(!setFilterProperty(importFijiMontageFilter, "CellAttributeMatrixName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Image Array Name
      var.setValue(daName);
      if(!setFilterProperty(importFijiMontageFilter, "ImageDataArrayName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Length Unit
      var.setValue(metadata.getLengthUnitsIdx());
      if(!setFilterProperty(importFijiMontageFilter, "LengthUnit", var))
      {
        return AbstractFilter::NullPointer();
      }

      return importFijiMontageFilter;
    }
  }

  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createImportRobometMontageFilter(const RobometMontageMetadata& metadata, const DataArrayPath& dcPath, const QString& amName, const QString& daName,
                                                                          int sliceNumber)
{
  QString filterName = "ITKImportRoboMetMontage";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer importRoboMetMontageFilter = factory->create();
    if(importRoboMetMontageFilter.get() != nullptr)
    {
      QVariant var;

      // Set the path for the Robomet Configuration File
      var.setValue(metadata.getFilePath());
      if(!setFilterProperty(importRoboMetMontageFilter, "InputFile", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set montage start
      var.setValue(metadata.getRowLimits());
      if(!setFilterProperty(importRoboMetMontageFilter, "RowMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set montage end
      var.setValue(metadata.getColumnLimits());
      if(!setFilterProperty(importRoboMetMontageFilter, "ColumnMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set Origin
      bool changeOrigin = metadata.getChangeOrigin();
      var.setValue(changeOrigin);
      if(!setFilterProperty(importRoboMetMontageFilter, "ChangeOrigin", var))
      {
        return AbstractFilter::NullPointer();
      }

      if(changeOrigin)
      {
        FloatVec3Type origin = metadata.getOrigin();
        var.setValue(origin);
        if(!setFilterProperty(importRoboMetMontageFilter, "Origin", var))
        {
          return AbstractFilter::NullPointer();
        }
      }

      // Set Spacing
      bool changeSpacing = metadata.getChangeSpacing();
      var.setValue(changeSpacing);
      if(!setFilterProperty(importRoboMetMontageFilter, "ChangeSpacing", var))
      {
        return AbstractFilter::NullPointer();
      }

      if(changeSpacing)
      {
        FloatVec3Type spacing = metadata.getSpacing();
        var.setValue(spacing);
        if(!setFilterProperty(importRoboMetMontageFilter, "Spacing", var))
        {
          return AbstractFilter::NullPointer();
        }
      }

      // Set the Data Container Path
      var.setValue(dcPath);
      if(!setFilterProperty(importRoboMetMontageFilter, "DataContainerPath", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Cell Attribute Matrix Name
      var.setValue(amName);
      if(!setFilterProperty(importRoboMetMontageFilter, "CellAttributeMatrixName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Image Array Name
      var.setValue(daName);
      if(!setFilterProperty(importRoboMetMontageFilter, "ImageDataArrayName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Slice number
      var.setValue(sliceNumber);
      if(!setFilterProperty(importRoboMetMontageFilter, "SliceNumber", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Image file prefix
      var.setValue(metadata.getFilePrefix());
      if(!setFilterProperty(importRoboMetMontageFilter, "ImageFilePrefix", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Image file extension
      var.setValue(metadata.getFileExtension());
      if(!setFilterProperty(importRoboMetMontageFilter, "ImageFileExtension", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Length Unit
      var.setValue(metadata.getLengthUnitsIdx());
      if(!setFilterProperty(importRoboMetMontageFilter, "LengthUnit", var))
      {
        return AbstractFilter::NullPointer();
      }

      return importRoboMetMontageFilter;
    }
  }

  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createImportZeissZenMontageFilter(ZeissZenMontageMetadata& metadata, const DataArrayPath& dcPath, const QString& amName, const QString& daName)
{
  // Instantiate Import AxioVision V4 Montage filter
  QString filterName = "ImportZenInfoMontage";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer importZeissZenMontageFilter = factory->create();
    if(importZeissZenMontageFilter.get() != nullptr)
    {
      QVariant var;

      // Set the path for the Zeiss Zen File
      var.setValue(metadata.getConfigFilePath());
      if(!setFilterProperty(importZeissZenMontageFilter, "InputFile", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set montage start
      var.setValue(metadata.getRowLimits());
      if(!setFilterProperty(importZeissZenMontageFilter, "RowMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set montage end
      var.setValue(metadata.getColumnLimits());
      if(!setFilterProperty(importZeissZenMontageFilter, "ColumnMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Data Container Prefix
      var.setValue(dcPath);
      if(!setFilterProperty(importZeissZenMontageFilter, "DataContainerPath", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Cell Attribute Matrix Name
      var.setValue(amName);
      if(!setFilterProperty(importZeissZenMontageFilter, "CellAttributeMatrixName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Image Array Name
      var.setValue(daName);
      if(!setFilterProperty(importZeissZenMontageFilter, "ImageDataArrayName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set Convert to Grayscale
      bool convertToGreyscale = metadata.getConvertToGreyscale();
      var.setValue(convertToGreyscale);
      if(!setFilterProperty(importZeissZenMontageFilter, "ConvertToGrayScale", var))
      {
        return AbstractFilter::NullPointer();
      }

      if(convertToGreyscale)
      {
        FloatVec3Type colorWeights = metadata.getColorWeighting();
        var.setValue(colorWeights);
        if(!setFilterProperty(importZeissZenMontageFilter, "ColorWeights", var))
        {
          return AbstractFilter::NullPointer();
        }
      }

      // Set Origin
      bool changeOrigin = metadata.getChangeOrigin();
      var.setValue(changeOrigin);
      if(!setFilterProperty(importZeissZenMontageFilter, "ChangeOrigin", var))
      {
        return AbstractFilter::NullPointer();
      }

      if(changeOrigin)
      {
        FloatVec3Type origin = metadata.getOrigin();
        var.setValue(origin);
        if(!setFilterProperty(importZeissZenMontageFilter, "Origin", var))
        {
          return AbstractFilter::NullPointer();
        }
      }

      return importZeissZenMontageFilter;
    }
  }

  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createImportZeissMontageFilter(const ZeissMontageMetadata& metadata, const DataArrayPath& dcPath, const QString& amName, const QString& daName,
                                                                        const QString& metaAmName, bool importAllMetadata)
{
  // Instantiate Import AxioVision V4 Montage filter
  QString filterName = "ImportAxioVisionV4Montage";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer importZeissMontageFilter = factory->create();
    if(importZeissMontageFilter.get() != nullptr)
    {
      QVariant var;

      // Set the path for the Robomet Configuration File
      var.setValue(metadata.getConfigFilePath());
      if(!setFilterProperty(importZeissMontageFilter, "InputFile", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set montage start
      var.setValue(metadata.getRowLimits());
      if(!setFilterProperty(importZeissMontageFilter, "RowMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set montage end
      var.setValue(metadata.getColumnLimits());
      if(!setFilterProperty(importZeissMontageFilter, "ColumnMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Data Container Prefix
      var.setValue(dcPath);
      if(!setFilterProperty(importZeissMontageFilter, "DataContainerPath", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Cell Attribute Matrix Name
      var.setValue(amName);
      if(!setFilterProperty(importZeissMontageFilter, "CellAttributeMatrixName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Image Array Name
      var.setValue(daName);
      if(!setFilterProperty(importZeissMontageFilter, "ImageDataArrayName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Metadata Attribute MatrixName
      var.setValue(metaAmName);
      if(!setFilterProperty(importZeissMontageFilter, "MetaDataAttributeMatrixName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set Import All Metadata
      var.setValue(importAllMetadata);
      if(!setFilterProperty(importZeissMontageFilter, "ImportAllMetaData", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set Convert to Grayscale
      bool convertToGreyscale = metadata.getConvertToGreyscale();
      var.setValue(convertToGreyscale);
      if(!setFilterProperty(importZeissMontageFilter, "ConvertToGrayScale", var))
      {
        return AbstractFilter::NullPointer();
      }

      if(convertToGreyscale)
      {
        FloatVec3Type colorWeights = metadata.getColorWeighting();
        var.setValue(colorWeights);
        if(!setFilterProperty(importZeissMontageFilter, "ColorWeights", var))
        {
          return AbstractFilter::NullPointer();
        }
      }

      // Set Origin
      bool changeOrigin = metadata.getChangeOrigin();
      var.setValue(changeOrigin);
      if(!setFilterProperty(importZeissMontageFilter, "ChangeOrigin", var))
      {
        return AbstractFilter::NullPointer();
      }

      if(changeOrigin)
      {
        FloatVec3Type origin = metadata.getOrigin();
        var.setValue(origin);
        if(!setFilterProperty(importZeissMontageFilter, "Origin", var))
        {
          return AbstractFilter::NullPointer();
        }
      }

      // Set Spacing
      bool changeSpacing = metadata.getChangeSpacing();
      var.setValue(changeSpacing);
      if(!setFilterProperty(importZeissMontageFilter, "ChangeSpacing", var))
      {
        return AbstractFilter::NullPointer();
      }

      if(changeSpacing)
      {
        FloatVec3Type spacing = metadata.getSpacing();
        var.setValue(spacing);
        if(!setFilterProperty(importZeissMontageFilter, "Spacing", var))
        {
          return AbstractFilter::NullPointer();
        }
      }

      return importZeissMontageFilter;
    }
  }

  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createImportEbsdMontageFilter(const EbsdMontageMetadata& metadata, const DataArrayPath& dcPath, const QString& cellAMName, const QString& cellEnsembleAMName,
                                                                       const QString& daName, const QString& ipfColorsArrayName)
{
  QString filterName = "ImportEbsdMontage";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer importEbsdMontageFilter = factory->create();
    if(importEbsdMontageFilter.get() != nullptr)
    {
      QVariant var;

      // Set the path for the Fiji Configuration File
      MontageFileListInfo fileListInfo;
      IntVec2Type rowLimits = metadata.getRowLimits();
      IntVec2Type colLimits = metadata.getColumnLimits();
      fileListInfo.RowStart = rowLimits[0];
      fileListInfo.RowEnd = rowLimits[1];
      fileListInfo.ColStart = colLimits[0];
      fileListInfo.ColEnd = colLimits[1];
      fileListInfo.InputPath = metadata.getInputDirectory();
      fileListInfo.FilePrefix = metadata.getFilePrefix();
      fileListInfo.FileSuffix = metadata.getFileSuffix();
      fileListInfo.FileExtension = metadata.getFileExtension();
      fileListInfo.PaddingDigits = metadata.getNumIndexDigits();
      fileListInfo.IncrementIndex = metadata.getIncrementIndex();

      var.setValue(fileListInfo);
      if(!setFilterProperty(importEbsdMontageFilter, "InputFileListInfo", var))
      {
        return AbstractFilter::NullPointer();
      }

      var.setValue(metadata.getMontageName());
      if(!setFilterProperty(importEbsdMontageFilter, "MontageName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Cell Attribute Matrix Name
      var.setValue(cellAMName);
      if(!setFilterProperty(importEbsdMontageFilter, "CellAttributeMatrixName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Cell Ensemble Attribute Matrix Name
      var.setValue(cellEnsembleAMName);
      if(!setFilterProperty(importEbsdMontageFilter, "CellEnsembleAttributeMatrixName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Define Scan Overlap
      var.setValue(static_cast<int>(metadata.getScanOverlapType()));
      if(!setFilterProperty(importEbsdMontageFilter, "DefineScanOverlap", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Define Pixel Overlap
      var.setValue(metadata.getPixelOverlap());
      if(!setFilterProperty(importEbsdMontageFilter, "ScanOverlapPixel", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Define Percent Overlap
      var.setValue(metadata.getPercentOverlap());
      if(!setFilterProperty(importEbsdMontageFilter, "ScanOverlapPercent", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the Generate IPF Color Map
      var.setValue(metadata.getGenerateIPFColorMap());
      if(!setFilterProperty(importEbsdMontageFilter, "GenerateIPFColorMap", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the IPF Colors Array Name
      var.setValue(ipfColorsArrayName);
      if(!setFilterProperty(importEbsdMontageFilter, "CellIPFColorsArrayName", var))
      {
        return AbstractFilter::NullPointer();
      }

      return importEbsdMontageFilter;
    }
  }

  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createPCMTileRegistrationFilter(IntVec2Type rowMontageLimits, IntVec2Type colMontageLimits, const QString& dcPrefix, const QString& amName,
                                                                         const QString& daName)
{
  QString filterName = "ITKPCMTileRegistration";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer itkRegistrationFilter = factory->create();
    if(itkRegistrationFilter.get() != nullptr)
    {
      QVariant var;

      var.setValue(colMontageLimits);
      if(!setFilterProperty(itkRegistrationFilter, "ColumnMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      var.setValue(rowMontageLimits);
      if(!setFilterProperty(itkRegistrationFilter, "RowMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      var.setValue(amName);
      if(!setFilterProperty(itkRegistrationFilter, "CommonAttributeMatrixName", var))
      {
        return AbstractFilter::NullPointer();
      }

      var.setValue(daName);
      if(!setFilterProperty(itkRegistrationFilter, "CommonDataArrayName", var))
      {
        return AbstractFilter::NullPointer();
      }

      var.setValue(dcPrefix);
      if(!setFilterProperty(itkRegistrationFilter, "DataContainerPrefix", var))
      {
        return AbstractFilter::NullPointer();
      }

      return itkRegistrationFilter;
    }
  }

  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createTileStitchingFilter(IntVec2Type rowMontageLimits, IntVec2Type colMontageLimits, const QString& dcPrefix, const QString& amName, const QString& daName,
                                                                   const DataArrayPath& montagePath)
{
  QString filterName = "ITKStitchMontage";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer itkStitchingFilter = factory->create();
    if(itkStitchingFilter.get() != nullptr)
    {
      QVariant var;

      // Set montage start
      var.setValue(colMontageLimits);
      if(!setFilterProperty(itkStitchingFilter, "ColumnMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set montage end
      var.setValue(rowMontageLimits);
      if(!setFilterProperty(itkStitchingFilter, "RowMontageLimits", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set the data container prefix
      var.setValue(dcPrefix);
      if(!setFilterProperty(itkStitchingFilter, "DataContainerPrefix", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set Common Attribute Matrix Name
      var.setValue(amName);
      if(!setFilterProperty(itkStitchingFilter, "CommonAttributeMatrixName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set Common Data Array Name
      var.setValue(daName);
      if(!setFilterProperty(itkStitchingFilter, "CommonDataArrayName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Montage results data array path components
      QString montageDataContainer = montagePath.getDataContainerName();
      var.setValue(montageDataContainer);
      if(!setFilterProperty(itkStitchingFilter, "MontageDataContainerName", var))
      {
        return AbstractFilter::NullPointer();
      }

      QString montageAttriubeMatrixName = montagePath.getAttributeMatrixName();
      var.setValue(montageAttriubeMatrixName);
      if(!setFilterProperty(itkStitchingFilter, "MontageAttributeMatrixName", var))
      {
        return AbstractFilter::NullPointer();
      }

      QString montageDataArrayName = montagePath.getDataArrayName();
      var.setValue(montageDataArrayName);
      if(!setFilterProperty(itkStitchingFilter, "MontageDataArrayName", var))
      {
        return AbstractFilter::NullPointer();
      }

      return itkStitchingFilter;
    }
  }

  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createImageFileReaderFilter(const QString& inputFile, const DataArrayPath& dcPath)
{
  QString filterName = "ITKImageReader";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer itkImageReaderFilter = factory->create();
    if(itkImageReaderFilter.get() != nullptr)
    {
      QVariant var;

      // Set file name
      var.setValue(inputFile);
      if(!setFilterProperty(itkImageReaderFilter, "FileName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set data container name
      var.setValue(dcPath);
      if(!setFilterProperty(itkImageReaderFilter, "DataContainerName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set cell attribute matrix name
      QString cellAttributeMatrixName = "CellData";
      var.setValue(cellAttributeMatrixName);
      if(!setFilterProperty(itkImageReaderFilter, "CellAttributeMatrixName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set image data array name
      QString imageDataArrayName = "ImageData";
      var.setValue(imageDataArrayName);
      if(!setFilterProperty(itkImageReaderFilter, "ImageDataArrayName", var))
      {
        return AbstractFilter::NullPointer();
      }

      return itkImageReaderFilter;
    }
  }

  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer VSFilterFactory::createImageFileWriterFilter(const QString& outputFile, const DataArrayPath& imageArrayPath)
{
  QString filterName = "ITKImageWriter";
  FilterManager* fm = FilterManager::Instance();
  IFilterFactory::Pointer factory = fm->getFactoryFromClassName(filterName);

  if(factory.get() != nullptr)
  {
    AbstractFilter::Pointer itkImageWriterFilter = factory->create();
    if(itkImageWriterFilter.get() != nullptr)
    {
      QVariant var;

      // Set file name
      var.setValue(outputFile);
      if(!setFilterProperty(itkImageWriterFilter, "FileName", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set plane [Currently default to XY (the first option in the DREAM3D GUI)]
      int plane = 0;
      var.setValue(plane);
      if(!setFilterProperty(itkImageWriterFilter, "Plane", var))
      {
        return AbstractFilter::NullPointer();
      }

      // Set image data array name
      var.setValue(imageArrayPath);
      if(!setFilterProperty(itkImageWriterFilter, "ImageArrayPath", var))
      {
        return AbstractFilter::NullPointer();
      }

      return itkImageWriterFilter;
    }
  }

  return AbstractFilter::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSFilterFactory::setFilterProperty(const AbstractFilter::Pointer& filter, const char* propertyName, const QVariant& value)
{
  if(!filter->setProperty(propertyName, value))
  {
    QString msg = tr("%1: Unable to set '%2' property. Aborting.").arg(filter->getHumanLabel(), propertyName);
    QMessageBox::critical(nullptr, "Filter Factory Error", msg, QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Ok);
    return false;
  }
  return true;
}
