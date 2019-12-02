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

#include "ImportEbsdMontageDialog.h"

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
QString ImportEbsdMontageDialog::m_OpenDialogLastDirectory = "";

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportEbsdMontageDialog::ImportEbsdMontageDialog(QWidget* parent)
: AbstractImportMontageDialog(parent)
, m_Ui(new Ui::ImportEbsdMontageDialog)
{
  m_Ui->setupUi(this);

  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportEbsdMontageDialog::~ImportEbsdMontageDialog() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ImportEbsdMontageDialog::Pointer ImportEbsdMontageDialog::New(QWidget* parent)
{
  Pointer sharedPtr(new ImportEbsdMontageDialog(parent));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportEbsdMontageDialog::setupGui()
{
  m_Ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setText("Import");

  qRegisterMetaType<FijiListInfo_t>();

  connectSignalsSlots();

  checkComplete();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportEbsdMontageDialog::connectSignalsSlots()
{
  connect(m_Ui->ebsdMontageListFrame, &EbsdMontageListWidget::widgetChanged, this, &ImportEbsdMontageDialog::checkComplete);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ImportEbsdMontageDialog::checkComplete() const
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

  if(m_Ui->ebsdMontageListFrame->isEnabled())
  {
    std::pair<bool, QString> complete = m_Ui->ebsdMontageListFrame->isComplete();
    if(!complete.first)
    {
      m_Ui->errLabel->setText(complete.second);
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
EbsdMontageMetadata ImportEbsdMontageDialog::getMetadata() const
{
  EbsdMontageMetadata metadata = m_Ui->ebsdMontageListFrame->getMetadata();
  metadata.setMontageName(m_Ui->montageNameLE->text());
  metadata.setDataDisplayType(static_cast<MontageMetadata::DisplayType>(m_Ui->dataDisplayTypeCB->currentIndex()));
  EbsdMontageMetadata::ScanOverlapType scanOverlapType = static_cast<EbsdMontageMetadata::ScanOverlapType>(m_Ui->scanOverlapTypeCB->currentIndex());
  switch(scanOverlapType)
  {
  case EbsdMontageMetadata::ScanOverlapType::PixelOverlap:
    metadata.setPixelOverlap({m_Ui->overlapValueXLE->text().toInt(), m_Ui->overlapValueYLE->text().toInt()});
    break;
  case EbsdMontageMetadata::ScanOverlapType::PercentOverlap:
    metadata.setPercentOverlap({m_Ui->overlapValueXLE->text().toFloat(), m_Ui->overlapValueYLE->text().toFloat()});
    break;
  case EbsdMontageMetadata::ScanOverlapType::None:
    break;
  }
  metadata.setScanOverlapType(scanOverlapType);
  metadata.setGenerateIPFColorMap(m_Ui->generateIPFColorMapCB->isChecked());
  return metadata;
}
