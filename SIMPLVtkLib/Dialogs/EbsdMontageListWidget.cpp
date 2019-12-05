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

#include "EbsdMontageListWidget.h"

//-- Qt Includes
#include <QtCore/QDir>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"

#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Utilities/FilePathGenerator.h"
#include "SIMPLib/Utilities/SIMPLDataPathValidator.h"
#include "SIMPLib/Utilities/StringOperations.h"

#include "SVWidgetsLib/Core/SVWidgetsLibConstants.h"
#include "SVWidgetsLib/QtSupport/QtSFileCompleter.h"
#include "SVWidgetsLib/QtSupport/QtSFileUtils.h"

#include "ui_EbsdMontageListWidget.h"

// Initialize private static member variable
QString EbsdMontageListWidget::m_OpenDialogLastFilePath = "";

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
EbsdMontageListWidget::EbsdMontageListWidget(QWidget* parent)
: QFrame(parent)
, m_Ui(new Ui::EbsdMontageListWidget)
{
  m_Ui->setupUi(this);
  setupGui();
  if(m_Ui->inputDir->text().isEmpty())
  {
    setInputDirectory(QDir::homePath());
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
EbsdMontageListWidget::~EbsdMontageListWidget() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::setIcon(const QPixmap& path)
{
  m_Icon = path;
  setupMenuField();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QPixmap EbsdMontageListWidget::getIcon()
{
  return m_Icon;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::SetOpenDialogLastFilePath(const QString& val)
{
  m_OpenDialogLastFilePath = val;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString EbsdMontageListWidget::GetOpenDialogLastFilePath()
{
  return m_OpenDialogLastFilePath;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::setWidgetListEnabled(bool b)
{
  foreach(QWidget* w, m_WidgetList)
  {
    w->setEnabled(b);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::setupGui()
{
  connectSignalsSlots();

  setupMenuField();

  m_Ui->absPathLabel->hide();

  m_WidgetList << m_Ui->inputDir << m_Ui->inputDirBtn;
  m_WidgetList << m_Ui->fileExt << m_Ui->errorMessage << m_Ui->totalDigits << m_Ui->fileSuffix;
  m_WidgetList << m_Ui->filePrefix << m_Ui->totalSlices << m_Ui->rowStart << m_Ui->rowEnd << m_Ui->colStart << m_Ui->colEnd;

  m_Ui->errorMessage->setVisible(false);

  // Update the widget when the data directory changes
  //  SIMPLDataPathValidator* validator = SIMPLDataPathValidator::Instance();
  //  connect(validator, &SIMPLDataPathValidator::dataDirectoryChanged, [=] {
  //    blockSignals(true);
  //    inputDir_textChanged(m_Ui->inputDir->text());
  //    blockSignals(false);

  //    emit parametersChanged();
  //  });
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::connectSignalsSlots()
{
  // Connections for the various ui widgets
  connect(m_Ui->inputDirBtn, &QPushButton::clicked, this, &EbsdMontageListWidget::inputDirBtn_clicked);

  QtSFileCompleter* com = new QtSFileCompleter(this, true);
  m_Ui->inputDir->setCompleter(com);
  connect(com, static_cast<void (QtSFileCompleter::*)(const QString&)>(&QtSFileCompleter::activated), this, &EbsdMontageListWidget::inputDir_textChanged);

  connect(m_Ui->inputDir, &QtSLineEdit::textChanged, this, &EbsdMontageListWidget::inputDir_textChanged);

  connect(m_Ui->filePrefix, &QtSLineEdit::textChanged, this, &EbsdMontageListWidget::lineEditChanged);
  connect(m_Ui->fileSuffix, &QtSLineEdit::textChanged, this, &EbsdMontageListWidget::lineEditChanged);
  connect(m_Ui->fileExt, &QtSLineEdit::textChanged, this, &EbsdMontageListWidget::lineEditChanged);

  connect(m_Ui->totalDigits, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &EbsdMontageListWidget::spinnerChanged);
  connect(m_Ui->rowStart, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &EbsdMontageListWidget::spinnerChanged);
  connect(m_Ui->rowEnd, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &EbsdMontageListWidget::spinnerChanged);
  connect(m_Ui->colStart, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &EbsdMontageListWidget::spinnerChanged);
  connect(m_Ui->colEnd, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &EbsdMontageListWidget::spinnerChanged);
  connect(m_Ui->increment, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &EbsdMontageListWidget::spinnerChanged);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::spinnerChanged(int value)
{
  updateFileListView();
  emit widgetChanged();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::pair<bool, QString> EbsdMontageListWidget::isComplete() const
{
  if(m_Ui->fileListView->count() <= 0)
  {
    return std::make_pair(false, "");
  }

  int fileCount = m_Ui->fileListView->count();
  for(int i = 0; i < fileCount; i++)
  {
    QListWidgetItem* item = m_Ui->fileListView->item(i);
    QString filePath = item->text();
    QFileInfo fi(filePath);
    if(!fi.exists())
    {
      return std::make_pair(false, "");
    }
  }

  // Check that size of montage based on start and end is valid
  int numCols = m_Ui->colEnd->value() - m_Ui->colStart->value() + 1;
  int numRows = m_Ui->rowEnd->value() - m_Ui->rowStart->value() + 1;

  int numberOfMontageTiles = numCols * numRows;
  int numberOfSelectedTiles = m_Ui->fileListView->count();
  if(numberOfSelectedTiles < numberOfMontageTiles)
  {
    QString errMsg = tr("The number of tiles in the tile list (%1) is less than the number of tiles declared in the montage (%2).\nPlease update"
                        " the 'Row Start', 'Row End', 'Col Start', and 'Col End' fields.")
                         .arg(numberOfSelectedTiles)
                         .arg(numberOfMontageTiles);
    return std::make_pair(false, errMsg);
  }

  return std::make_pair(true, "");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::keyPressEvent(QKeyEvent* event)
{
  if(event->key() == Qt::Key_Escape)
  {
    m_Ui->inputDir->setText(m_CurrentText);
    m_Ui->inputDir->setStyleSheet("");
    m_Ui->inputDir->setToolTip("");
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::setupMenuField()
{
  SIMPLDataPathValidator* validator = SIMPLDataPathValidator::Instance();
  QString inputPath = validator->convertToAbsolutePath(m_Ui->inputDir->text());

  QFileInfo fi(inputPath);

  QMenu* lineEditMenu = new QMenu(m_Ui->inputDir);
  m_Ui->inputDir->setButtonMenu(QtSLineEdit::Left, lineEditMenu);
  m_Ui->inputDir->setButtonVisible(QtSLineEdit::Left, true);

  QPixmap pixmap(8, 8);
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  painter.drawPixmap(0, (pixmap.height() - m_Icon.height()) / 2, m_Icon);
  m_Ui->inputDir->setButtonPixmap(QtSLineEdit::Left, pixmap);

  {
    m_ShowFileAction = new QAction(lineEditMenu);
    m_ShowFileAction->setObjectName(QString::fromUtf8("showFileAction"));
#if defined(Q_OS_WIN)
    m_ShowFileAction->setText("Show in Windows Explorer");
#elif defined(Q_OS_MAC)
    m_ShowFileAction->setText("Show in Finder");
#else
    m_ShowFileAction->setText("Show in File System");
#endif
    lineEditMenu->addAction(m_ShowFileAction);
    connect(m_ShowFileAction, SIGNAL(triggered()), this, SLOT(showFileInFileSystem()));
  }

  if(!inputPath.isEmpty() && fi.exists())
  {
    m_ShowFileAction->setEnabled(true);
  }
  else
  {
    m_ShowFileAction->setDisabled(true);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::inputDirBtn_clicked()
{
  QString outputFile = QFileDialog::getExistingDirectory(this, tr("Select Input Directory"), getInputDirectory());

  if(!outputFile.isNull())
  {
    m_Ui->inputDir->blockSignals(true);
    m_Ui->inputDir->setText(QDir::toNativeSeparators(outputFile));
    inputDir_textChanged(m_Ui->inputDir->text());
    SetOpenDialogLastFilePath(outputFile);
    m_Ui->inputDir->blockSignals(false);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::inputDir_textChanged(const QString& text)
{
  SIMPLDataPathValidator* validator = SIMPLDataPathValidator::Instance();
  QString inputPath = validator->convertToAbsolutePath(text);

  QFileInfo fi(text);
  if(fi.isRelative())
  {
    m_Ui->absPathLabel->setText(inputPath);
    m_Ui->absPathLabel->show();
  }
  else
  {
    m_Ui->absPathLabel->hide();
  }

  m_Ui->inputDir->setToolTip("Absolute File Path: " + inputPath);

  if(QtSFileUtils::VerifyPathExists(inputPath, m_Ui->inputDir))
  {
    m_ShowFileAction->setEnabled(true);
    QDir dir(inputPath);
    dir.cdUp();

    updateFileListView();
    m_Ui->inputDir->blockSignals(true);
    m_Ui->inputDir->setText(QDir::toNativeSeparators(m_Ui->inputDir->text()));

    m_Ui->inputDir->blockSignals(false);
  }
  else
  {
    m_ShowFileAction->setEnabled(false);
    m_Ui->fileListView->clear();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::updateFileListView()
{
  int rowStart = m_Ui->rowStart->value();
  int rowEnd = m_Ui->rowEnd->value() + 1;
  int colStart = m_Ui->colStart->value();
  int colEnd = m_Ui->colEnd->value() + 1;

  // int increment = m_Ui->increment->value();
  bool hasMissingFiles = false;
  const bool k_RowColOrdering = true;
  SIMPLDataPathValidator* validator = SIMPLDataPathValidator::Instance();
  QString inputPath = validator->convertToAbsolutePath(m_Ui->inputDir->text());

  // Now generate all the file names the user is asking for and populate the table
  FilePathGenerator::TileRCIndexLayout2D tileLayout2d = FilePathGenerator::GenerateRCIndexMontageFileList(rowStart, rowEnd, colStart, colEnd, hasMissingFiles, k_RowColOrdering, inputPath, m_Ui->filePrefix->text(),
                                                                                            m_Ui->fileSuffix->text(), m_Ui->fileExt->text(), m_Ui->totalDigits->value());

  m_Ui->fileListView->clear();
  QIcon greenDot = QIcon(QString(":/SIMPL/icons/images/bullet_ball_green.png"));
  QIcon redDot = QIcon(QString(":/SIMPL/icons/images/bullet_ball_red.png"));
  for(const FilePathGenerator::TileRCIndexRow2D& tileRow2D : tileLayout2d)
  {
    for(const FilePathGenerator::TileRCIndex2D& tile2D : tileRow2D)
    {
      QFileInfo fi(tile2D.FileName);
      QListWidgetItem* item = new QListWidgetItem(tile2D.FileName, m_Ui->fileListView);
      if(fi.exists())
      {
        item->setIcon(greenDot);
        m_Ui->generatedFileNameExample->setText(tile2D.FileName);
      }
      else
      {
        hasMissingFiles = true;
        item->setIcon(redDot);
      }
    }
  }
  if(hasMissingFiles)
  {
    m_Ui->errorMessage->setVisible(true);
    m_Ui->errorMessage->setText("Alert: Red Dot File(s) on the list do NOT exist on the filesystem. Please make sure all files exist");
  }
  else
  {
    m_Ui->errorMessage->setVisible(true);
    m_Ui->errorMessage->setText("All files exist.");
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::lineEditChanged(const QString& text)
{
  updateFileListView();
  emit widgetChanged();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::beforePreflight()
{
  if(!m_DidCausePreflight)
  {
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::afterPreflight()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdMontageListWidget::setInputDirectory(const QString& val)
{
  m_Ui->inputDir->setText(val);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString EbsdMontageListWidget::getInputDirectory()
{
  if(m_Ui->inputDir->text().isEmpty())
  {
    return QDir::homePath();
  }
  return m_Ui->inputDir->text();
}

// -----------------------------------------------------------------------------
EbsdMontageMetadata EbsdMontageListWidget::getMetadata() const
{
  EbsdMontageMetadata metadata;
  metadata.setInputDirectory(m_Ui->inputDir->text());
  metadata.setFilePrefix(m_Ui->filePrefix->text());
  metadata.setFileSuffix(m_Ui->fileSuffix->text());
  metadata.setFileExtension(m_Ui->fileExt->text());
  metadata.setRowLimits({m_Ui->rowStart->value(), m_Ui->rowEnd->value() + 1});
  metadata.setColumnLimits({m_Ui->colStart->value(), m_Ui->colEnd->value() + 1});
  metadata.setIncrementIndex(m_Ui->increment->value());
  metadata.setNumIndexDigits(m_Ui->totalDigits->value());

  std::list<QString> filePaths;
  int fileCount = m_Ui->fileListView->count();
  for(int i = 0; i < fileCount; i++)
  {
    QListWidgetItem* item = m_Ui->fileListView->item(i);
    if(item == nullptr)
    {
      continue;
    }

    filePaths.push_back(item->text());
  }
  metadata.setFilePaths(filePaths);
  return metadata;
}
