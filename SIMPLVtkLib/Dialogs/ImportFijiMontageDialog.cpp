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

#include "ImportFijiMontageDialog.h"

#include <QtCore/QDir>
#include <QtCore/QMimeDatabase>

#include <QtWidgets/QCompleter>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QFileSystemModel>

#include "SIMPLib/Utilities/SIMPLDataPathValidator.h"
#include "SVWidgetsLib/Core/SVWidgetsLibConstants.h"
#include "SVWidgetsLib/QtSupport/QtSDisclosableWidget.h"
#include "SVWidgetsLib/QtSupport/QtSFileCompleter.h"
#include "SVWidgetsLib/QtSupport/QtSFileUtils.h"

// Initialize private static member variable
QString ImportFijiMontageDialog::m_OpenDialogLastDirectory = "";

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportFijiMontageDialog::ImportFijiMontageDialog(QWidget* parent)
: AbstractImportMontageDialog(parent)
, m_Ui(new Ui::ImportFijiMontageDialog)
{
  m_Ui->setupUi(this);

  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportFijiMontageDialog::~ImportFijiMontageDialog() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportFijiMontageDialog::Pointer ImportFijiMontageDialog::New(QWidget* parent)
{
  Pointer sharedPtr(new ImportFijiMontageDialog(parent));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportFijiMontageDialog::setupGui()
{
  m_Ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setText("Import");

  qRegisterMetaType<FijiListInfo_t>();

  QVector<QString> lengthUnitStrings = IGeometry::GetAllLengthUnitStrings();
  m_Ui->unitsCB->addItems(lengthUnitStrings.toList());
  m_Ui->unitsCB->setCurrentIndex(lengthUnitStrings.indexOf("Micrometer"));

  connectSignalsSlots();

  SVStyle* style = SVStyle::Instance();
  m_Ui->errLabel->setStyleSheet(tr("QLabel { color: %1; }").arg(style->getWidget_Error_color().name()));

  m_Ui->originX->setValidator(new QDoubleValidator);
  m_Ui->originY->setValidator(new QDoubleValidator);
  m_Ui->originZ->setValidator(new QDoubleValidator);

  m_Ui->montageColStart->setValidator(new QDoubleValidator);
  m_Ui->montageColEnd->setValidator(new QDoubleValidator);
  m_Ui->montageRowStart->setValidator(new QDoubleValidator);
  m_Ui->montageRowEnd->setValidator(new QDoubleValidator);

  checkComplete();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportFijiMontageDialog::connectSignalsSlots()
{
  connect(m_Ui->fijiListWidget, &FijiListWidget::inputDirectoryChanged, [=] { checkComplete(); });

  connect(m_Ui->changeOriginCB, &QCheckBox::stateChanged, this, &ImportFijiMontageDialog::changeOrigin_stateChanged);
  connect(m_Ui->originX, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->originY, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->originZ, &QLineEdit::textChanged, [=] { checkComplete(); });

  connect(m_Ui->montageColStart, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->montageColEnd, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->montageRowStart, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->montageRowEnd, &QLineEdit::textChanged, [=] { checkComplete(); });
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportFijiMontageDialog::changeOrigin_stateChanged(int state)
{
  m_Ui->originX->setEnabled(state == Qt::Checked);
  m_Ui->originY->setEnabled(state == Qt::Checked);
  m_Ui->originZ->setEnabled(state == Qt::Checked);
  if(state == Qt::Unchecked)
  {
    m_Ui->originX->setText("0");
    m_Ui->originY->setText("0");
    m_Ui->originZ->setText("0");
  }

  checkComplete();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportFijiMontageDialog::checkComplete() const
{
  bool result = true;

  if(m_Ui->montageNameLE->isEnabled())
  {
    if(m_Ui->montageNameLE->text().isEmpty())
    {
      m_Ui->errLabel->setText("Montage name is empty");
      result = false;
    }
  }

  if(m_Ui->fijiListWidget->isEnabled())
  {
    if(!m_Ui->fijiListWidget->isComplete())
    {
      m_Ui->errLabel->setText("Fiji file list is incomplete.");
      result = false;
    }
  }

  if(m_Ui->originX->isEnabled())
  {
    if(m_Ui->originX->text().isEmpty())
    {
      m_Ui->errLabel->setText("Origin X is empty");
      result = false;
    }
  }

  if(m_Ui->originY->isEnabled())
  {
    if(m_Ui->originY->text().isEmpty())
    {
      m_Ui->errLabel->setText("Origin Y is empty");
      result = false;
    }
  }

  if(m_Ui->originZ->isEnabled())
  {
    if(m_Ui->originZ->text().isEmpty())
    {
      m_Ui->errLabel->setText("Origin Z is empty");
      result = false;
    }
  }

  if(m_Ui->montageColStart->isEnabled())
  {
    if(m_Ui->montageColStart->text().isEmpty())
    {
      m_Ui->errLabel->setText("Montage Column Start is empty");
      result = false;
    }
  }

  if(m_Ui->montageColEnd->isEnabled())
  {
    if(m_Ui->montageColEnd->text().isEmpty())
    {
      m_Ui->errLabel->setText("Montage Column End is empty");
      result = false;
    }
  }

  if(m_Ui->montageRowStart->isEnabled())
  {
    if(m_Ui->montageRowStart->text().isEmpty())
    {
      m_Ui->errLabel->setText("Montage Row Start is empty");
      result = false;
    }
  }

  if(m_Ui->montageRowEnd->isEnabled())
  {
    if(m_Ui->montageRowEnd->text().isEmpty())
    {
      m_Ui->errLabel->setText("Montage Row End is empty");
      result = false;
    }
  }

  // Check that size of montage based on start and end is valid
  int numCols = m_Ui->montageColEnd->text().toInt() - m_Ui->montageColStart->text().toInt() + 1;
  int numRows = m_Ui->montageRowEnd->text().toInt() - m_Ui->montageRowStart->text().toInt() + 1;

  int numberOfMontageTiles = numCols * numRows;
  int numberOfSelectedTiles = m_Ui->fijiListWidget->getCurrentNumberOfTiles();
  if(numberOfSelectedTiles < numberOfMontageTiles)
  {
    m_Ui->errLabel->setText(tr("The number of tiles in the tile list (%1) is less than the number of tiles declared in the montage (%2).\nPlease update"
                               " the tile list as well as the 'Montage Start' and 'Montage End' fields.")
                                .arg(numberOfSelectedTiles)
                                .arg(numberOfMontageTiles));
    result = false;
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
FijiMontageMetadata ImportFijiMontageDialog::getMetadata() const
{
  FijiMontageMetadata metadata = m_Ui->fijiListWidget->getMetadata();
  metadata.setMontageName(m_Ui->montageNameLE->text());
  metadata.setDataDisplayType(static_cast<MontageMetadata::DisplayType>(m_Ui->dataDisplayTypeCB->currentIndex()));
  metadata.setChangeOrigin(m_Ui->changeOriginCB->isChecked());
  float originX = m_Ui->originX->text().toFloat();
  float originY = m_Ui->originY->text().toFloat();
  float originZ = m_Ui->originZ->text().toFloat();
  FloatVec3Type origin = {originX, originY, originZ};
  metadata.setOrigin(origin);
  metadata.setLengthUnitsIdx(m_Ui->unitsCB->currentIndex());
  metadata.setRowLimits({m_Ui->montageRowStart->text().toInt(), m_Ui->montageRowEnd->text().toInt()});
  metadata.setColumnLimits({m_Ui->montageColStart->text().toInt(), m_Ui->montageColEnd->text().toInt()});
  return metadata;
}
