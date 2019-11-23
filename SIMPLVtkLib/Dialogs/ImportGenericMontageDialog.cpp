/* ============================================================================
 * Copyright (c) 2009-2016 BlueQuartz Software, LLC
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

#include "ImportGenericMontageDialog.h"

#include <QtCore/QDir>

#include <QtWidgets/QCompleter>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QFileSystemModel>

#include "SVWidgetsLib/QtSupport/QtSDisclosableWidget.h"
#include "SVWidgetsLib/Widgets/SVStyle.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportGenericMontageDialog::ImportGenericMontageDialog(QWidget* parent)
: AbstractImportMontageDialog(parent)
, m_Ui(new Ui::ImportGenericMontageDialog)
{
  m_Ui->setupUi(this);

  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportGenericMontageDialog::~ImportGenericMontageDialog() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportGenericMontageDialog::Pointer ImportGenericMontageDialog::New(QWidget* parent)
{
  Pointer sharedPtr(new ImportGenericMontageDialog(parent));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportGenericMontageDialog::setupGui()
{
  m_Ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setText("Import");

  QVector<QString> lengthUnitStrings = IGeometry::GetAllLengthUnitStrings();
  m_Ui->unitsCB->addItems(lengthUnitStrings.toList());
  m_Ui->unitsCB->setCurrentIndex(lengthUnitStrings.indexOf("Micrometer"));

  connectSignalsSlots();

  updateOrderChoices(MontageSettings::MontageType::GridRowByRow);

  SVStyle* style = SVStyle::Instance();
  m_Ui->errLabel->setStyleSheet(tr("QLabel { color: %1; }").arg(style->getWidget_Error_color().name()));

  m_Ui->tileOverlapSB->setMinimum(0);
  m_Ui->tileOverlapSB->setMaximum(100);

  m_Ui->originX->setValidator(new QDoubleValidator);
  m_Ui->originY->setValidator(new QDoubleValidator);
  m_Ui->originZ->setValidator(new QDoubleValidator);
  m_Ui->spacingX->setValidator(new QDoubleValidator);
  m_Ui->spacingY->setValidator(new QDoubleValidator);
  m_Ui->spacingZ->setValidator(new QDoubleValidator);
  m_Ui->rowStart->setValidator(new QDoubleValidator);
  m_Ui->rowEnd->setValidator(new QDoubleValidator);
  m_Ui->colStart->setValidator(new QDoubleValidator);
  m_Ui->colEnd->setValidator(new QDoubleValidator);

  checkComplete();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportGenericMontageDialog::connectSignalsSlots()
{
  connect(m_Ui->tileOverlapSB, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=] { checkComplete(); });

  connect(m_Ui->montageNameLE, &QLineEdit::textChanged, this, [=] { checkComplete(); });

  connect(m_Ui->tileListWidget, &TileListWidget::inputDirectoryChanged, [=] { tileListWidgetChanged(); });
  connect(m_Ui->tileListWidget, &TileListWidget::fileOrderingChanged, [=] { tileListWidgetChanged(); });
  connect(m_Ui->tileListWidget, &TileListWidget::filePrefixChanged, [=] { tileListWidgetChanged(); });
  connect(m_Ui->tileListWidget, &TileListWidget::fileSuffixChanged, [=] { tileListWidgetChanged(); });
  connect(m_Ui->tileListWidget, &TileListWidget::fileExtensionChanged, [=] { tileListWidgetChanged(); });
  connect(m_Ui->tileListWidget, &TileListWidget::startIndexChanged, [=] { tileListWidgetChanged(); });
  connect(m_Ui->tileListWidget, &TileListWidget::endIndexChanged, [=] { tileListWidgetChanged(); });
  connect(m_Ui->tileListWidget, &TileListWidget::incrementIndexChanged, [=] { tileListWidgetChanged(); });
  connect(m_Ui->tileListWidget, &TileListWidget::paddingDigitsChanged, [=] { tileListWidgetChanged(); });

  connect(m_Ui->collectionTypeCB, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) { updateOrderChoices(static_cast<MontageSettings::MontageType>(index)); });

  connect(m_Ui->orderCB, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) { checkComplete(); });

  connect(m_Ui->originX, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->originY, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->originZ, &QLineEdit::textChanged, [=] { checkComplete(); });

  connect(m_Ui->rowStart, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->rowEnd, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->colStart, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->colEnd, &QLineEdit::textChanged, [=] { checkComplete(); });

  connect(m_Ui->changeSpacingCB, &QCheckBox::stateChanged, [=] {
    bool isChecked = m_Ui->changeSpacingCB->isChecked();
    m_Ui->spacingX->setEnabled(isChecked);
    m_Ui->spacingY->setEnabled(isChecked);
    m_Ui->spacingZ->setEnabled(isChecked);
  });
  connect(m_Ui->changeSpacingCB, &QCheckBox::stateChanged, this, &ImportGenericMontageDialog::changeSpacing_stateChanged);
  connect(m_Ui->spacingX, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->spacingY, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->spacingZ, &QLineEdit::textChanged, [=] { checkComplete(); });
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportGenericMontageDialog::tileListWidgetChanged()
{
  checkComplete();

  StackFileListInfo fileListInfo = m_Ui->tileListWidget->getFileListInfo();
  setFileListInfo(fileListInfo);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportGenericMontageDialog::changeSpacing_stateChanged(int state)
{
  m_Ui->spacingX->setEnabled(state == Qt::Checked);
  m_Ui->spacingY->setEnabled(state == Qt::Checked);
  m_Ui->spacingZ->setEnabled(state == Qt::Checked);
  if(state == Qt::Unchecked)
  {
    m_Ui->spacingX->setText("1");
    m_Ui->spacingY->setText("1");
    m_Ui->spacingZ->setText("1");
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportGenericMontageDialog::checkComplete() const
{
  bool result = true;

  if(m_Ui->montageNameLE->isEnabled())
  {
    if(m_Ui->montageNameLE->text().isEmpty())
    {
      m_Ui->errLabel->setText("The montage name is empty.");
      result = false;
    }
  }

  QString tileListWidgetErrMsg;
  if(!m_Ui->tileListWidget->isComplete(tileListWidgetErrMsg))
  {
    m_Ui->errLabel->setText(tileListWidgetErrMsg);
    result = false;
  }

  int numCols = m_Ui->colEnd->text().toInt() - m_Ui->colStart->text().toInt() + 1;
  int numRows = m_Ui->rowEnd->text().toInt() - m_Ui->rowStart->text().toInt() + 1;

  int numberOfMontageTiles = numCols * numRows;
  int numberOfSelectedTiles = m_Ui->tileListWidget->getCurrentNumberOfTiles();
  if(numberOfSelectedTiles < numberOfMontageTiles)
  {
    m_Ui->errLabel->setText(tr("The number of tiles in the tile list (%1) is less than the number of tiles declared in the montage (%2).\nPlease update"
                               " the tile list as well as the 'Montage Start' and 'Montage End' fields.")
                                .arg(numberOfSelectedTiles)
                                .arg(numberOfMontageTiles));
    result = false;
  }

  if(m_Ui->originX->isEnabled())
  {
    if(m_Ui->originX->text().isEmpty())
    {
      m_Ui->errLabel->setText("Origin X is empty.");
      result = false;
    }
  }

  if(m_Ui->originY->isEnabled())
  {
    if(m_Ui->originY->text().isEmpty())
    {
      m_Ui->errLabel->setText("Origin Y is empty.");
      result = false;
    }
  }

  if(m_Ui->originZ->isEnabled())
  {
    if(m_Ui->originZ->text().isEmpty())
    {
      m_Ui->errLabel->setText("Origin Z is empty.");
      result = false;
    }
  }

  if(m_Ui->spacingX->isEnabled())
  {
    if(m_Ui->spacingX->text().isEmpty())
    {
      m_Ui->errLabel->setText("Spacing X is empty.");
      result = false;
    }
  }

  if(m_Ui->spacingY->isEnabled())
  {
    if(m_Ui->spacingY->text().isEmpty())
    {
      m_Ui->errLabel->setText("Spacing Y is empty.");
      result = false;
    }
  }

  if(m_Ui->spacingZ->isEnabled())
  {
    if(m_Ui->spacingZ->text().isEmpty())
    {
      m_Ui->errLabel->setText("Spacing Z is empty.");
      result = false;
    }
  }

  QPushButton* okBtn = m_Ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok);
  if(okBtn == nullptr)
  {
    throw InvalidOKButtonException();
  }

  okBtn->setEnabled(result);
  m_Ui->errLabel->setVisible(!result);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportGenericMontageDialog::updateOrderChoices(MontageSettings::MontageType collectionType)
{
  m_Ui->orderCB->setEnabled(true);
  m_Ui->orderCB->clear();

  switch(collectionType)
  {
  case MontageSettings::MontageType::GridRowByRow:
    m_Ui->orderCB->addItem("Right & Down");
    m_Ui->orderCB->addItem("Left & Down");
    m_Ui->orderCB->addItem("Right & Up");
    m_Ui->orderCB->addItem("Left & Up");
    break;
  case MontageSettings::MontageType::GridColByCol:
    m_Ui->orderCB->addItem("Down & Right");
    m_Ui->orderCB->addItem("Down & Left");
    m_Ui->orderCB->addItem("Up & Right");
    m_Ui->orderCB->addItem("Up & Left");
    break;
  case MontageSettings::MontageType::GridSnakeByRows:
    m_Ui->orderCB->addItem("Right & Down");
    m_Ui->orderCB->addItem("Left & Down");
    m_Ui->orderCB->addItem("Right & Up");
    m_Ui->orderCB->addItem("Left & Up");
    break;
  case MontageSettings::MontageType::GridSnakeByCols:
    m_Ui->orderCB->addItem("Down & Right");
    m_Ui->orderCB->addItem("Down & Left");
    m_Ui->orderCB->addItem("Up & Right");
    m_Ui->orderCB->addItem("Up & Left");
    break;
  case MontageSettings::MontageType::FilenameDefined:
    m_Ui->orderCB->setDisabled(true);
    break;
  default:
    m_Ui->orderCB->setDisabled(true);
    break;
  }

  checkComplete();
}

// -----------------------------------------------------------------------------
GenericMontageMetadata ImportGenericMontageDialog::getMetadata() const
{
  GenericMontageMetadata metadata;
  metadata.setMontageName(m_Ui->montageNameLE->text());
  metadata.setDataDisplayType(static_cast<MontageMetadata::DisplayType>(m_Ui->dataDisplayTypeCB->currentIndex()));
  metadata.setMontageType(static_cast<MontageSettings::MontageType>(m_Ui->collectionTypeCB->currentIndex()));
  metadata.setMontageOrder(static_cast<MontageSettings::MontageOrder>(m_Ui->orderCB->currentIndex()));
  metadata.setTileOverlap(m_Ui->tileOverlapSB->value());
  float originX = m_Ui->originX->text().toFloat();
  float originY = m_Ui->originY->text().toFloat();
  float originZ = m_Ui->originZ->text().toFloat();
  FloatVec3Type origin = {originX, originY, originZ};
  metadata.setOrigin(origin);
  metadata.setChangeSpacing(m_Ui->changeSpacingCB->isChecked());
  float spacingX = m_Ui->originX->text().toFloat();
  float spacingY = m_Ui->originY->text().toFloat();
  float spacingZ = m_Ui->originZ->text().toFloat();
  FloatVec3Type spacing = {spacingX, spacingY, spacingZ};
  metadata.setSpacing(spacing);
  metadata.setLengthUnitsIdx(m_Ui->unitsCB->currentIndex());
  metadata.setRowLimits({m_Ui->rowStart->text().toInt(), m_Ui->rowEnd->text().toInt()});
  metadata.setColumnLimits({m_Ui->colStart->text().toInt(), m_Ui->colEnd->text().toInt()});
  return metadata;
}
