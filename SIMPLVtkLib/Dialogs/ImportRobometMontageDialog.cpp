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

#include "ImportRobometMontageDialog.h"

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
QString ImportRobometMontageDialog::m_OpenDialogLastDirectory = "";

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportRobometMontageDialog::ImportRobometMontageDialog(QWidget* parent)
: AbstractImportMontageDialog(parent)
, m_Ui(new Ui::ImportRobometMontageDialog)
{
  m_Ui->setupUi(this);

  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportRobometMontageDialog::~ImportRobometMontageDialog()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportRobometMontageDialog::Pointer ImportRobometMontageDialog::New(QWidget* parent)
{
  Pointer sharedPtr(new ImportRobometMontageDialog(parent));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRobometMontageDialog::setupGui()
{
  m_Ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setText("Import");

  qRegisterMetaType<RobometListInfo_t>();

  QVector<QString> lengthUnitStrings = IGeometry::GetAllLengthUnitStrings();
  m_Ui->unitsCB->addItems(lengthUnitStrings.toList());
  m_Ui->unitsCB->setCurrentIndex(lengthUnitStrings.indexOf("Micrometer"));

  connectSignalsSlots();

  checkComplete();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRobometMontageDialog::connectSignalsSlots()
{
  connect(m_Ui->robometListWidget, &RobometListWidget::inputDirectoryChanged, [=] { checkComplete(); });
  connect(m_Ui->robometListWidget, &RobometListWidget::filePrefixChanged, [=] { checkComplete(); });
  connect(m_Ui->robometListWidget, &RobometListWidget::fileSuffixChanged, [=] { checkComplete(); });
  connect(m_Ui->robometListWidget, &RobometListWidget::fileExtensionChanged, [=] { checkComplete(); });
  connect(m_Ui->robometListWidget, &RobometListWidget::sliceMinChanged, [=] { checkComplete(); });
  connect(m_Ui->robometListWidget, &RobometListWidget::sliceMaxChanged, [=] { checkComplete(); });
  connect(m_Ui->robometListWidget, &RobometListWidget::montageStartColChanged, [=] { checkComplete(); });
  connect(m_Ui->robometListWidget, &RobometListWidget::montageStartRowChanged, [=] { checkComplete(); });
  connect(m_Ui->robometListWidget, &RobometListWidget::montageEndColChanged, [=] { checkComplete(); });
  connect(m_Ui->robometListWidget, &RobometListWidget::montageEndRowChanged, [=] { checkComplete(); });
  connect(m_Ui->robometListWidget, &RobometListWidget::slicePaddingChanged, [=] { checkComplete(); });
  connect(m_Ui->robometListWidget, &RobometListWidget::rowColPaddingChanged, [=] { checkComplete(); });

  connect(m_Ui->changeOriginCB, &QCheckBox::stateChanged, this, &ImportRobometMontageDialog::changeOrigin_stateChanged);
  connect(m_Ui->originX, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->originY, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->originZ, &QLineEdit::textChanged, [=] { checkComplete(); });

  connect(m_Ui->changeSpacingCB, &QCheckBox::stateChanged, this, &ImportRobometMontageDialog::changeSpacing_stateChanged);
  connect(m_Ui->spacingX, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->spacingY, &QLineEdit::textChanged, [=] { checkComplete(); });
  connect(m_Ui->spacingZ, &QLineEdit::textChanged, [=] { checkComplete(); });
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRobometMontageDialog::changeOrigin_stateChanged(int state)
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
void ImportRobometMontageDialog::changeSpacing_stateChanged(int state)
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

  checkComplete();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportRobometMontageDialog::checkComplete() const
{
  bool result = true;

  if(m_Ui->montageNameLE->isEnabled())
  {
    if(m_Ui->montageNameLE->text().isEmpty())
    {
      result = false;
    }
  }

  if(m_Ui->robometListWidget->isEnabled())
  {
    if(!m_Ui->robometListWidget->isComplete())
    {
      result = false;
    }
  }

  if(m_Ui->originX->isEnabled())
  {
    if(m_Ui->originX->text().isEmpty())
    {
      result = false;
    }
  }

  if(m_Ui->originY->isEnabled())
  {
    if(m_Ui->originY->text().isEmpty())
    {
      result = false;
    }
  }

  if(m_Ui->originZ->isEnabled())
  {
    if(m_Ui->originZ->text().isEmpty())
    {
      result = false;
    }
  }

  if(m_Ui->spacingX->isEnabled())
  {
    if(m_Ui->spacingX->text().isEmpty())
    {
      result = false;
    }
  }

  if(m_Ui->spacingY->isEnabled())
  {
    if(m_Ui->spacingY->text().isEmpty())
    {
      result = false;
    }
  }

  if(m_Ui->spacingZ->isEnabled())
  {
    if(m_Ui->spacingZ->text().isEmpty())
    {
      result = false;
    }
  }

  QPushButton* okBtn = m_Ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok);
  if(!okBtn)
  {
    throw InvalidOKButtonException();
  }

  okBtn->setEnabled(result);
}

// -----------------------------------------------------------------------------
RobometMontageMetadata ImportRobometMontageDialog::getMetadata() const
{
  RobometMontageMetadata metadata = m_Ui->robometListWidget->getMetadata();
  metadata.setMontageName(m_Ui->montageNameLE->text());
  metadata.setDataDisplayType(static_cast<MontageMetadata::DisplayType>(m_Ui->dataDisplayTypeCB->currentIndex()));
  metadata.setChangeOrigin(m_Ui->changeOriginCB->isChecked());
  float originX = m_Ui->originX->text().toFloat();
  float originY = m_Ui->originY->text().toFloat();
  float originZ = m_Ui->originZ->text().toFloat();
  FloatVec3Type origin = {originX, originY, originZ};
  metadata.setOrigin(origin);
  metadata.setChangeSpacing(m_Ui->changeSpacingCB->isChecked());
  float spacingX = m_Ui->spacingX->text().toFloat();
  float spacingY = m_Ui->spacingY->text().toFloat();
  float spacingZ = m_Ui->spacingZ->text().toFloat();
  FloatVec3Type spacing = {spacingX, spacingY, spacingZ};
  metadata.setSpacing(spacing);
  metadata.setLengthUnitsIdx(m_Ui->unitsCB->currentIndex());
  return metadata;
}
