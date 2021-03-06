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

#pragma once

#include "SIMPLib/Common/SIMPLArray.hpp"
#include "SIMPLib/Geometry/IGeometry.h"

#include "SIMPLVtkLib/Dialogs/AbstractImportMontageDialog.h"
#include "SIMPLVtkLib/Dialogs/FijiListWidget.h"

#include "ui_ImportFijiMontageDialog.h"

class SIMPLVtkLib_EXPORT ImportFijiMontageDialog : public AbstractImportMontageDialog
{
  Q_OBJECT

public:
  SIMPL_SHARED_POINTERS(ImportFijiMontageDialog)

  /**
   * @brief New
   * @param parent
   * @return
   */
  static Pointer New(QWidget* parent = nullptr);

  ~ImportFijiMontageDialog() override;

  SIMPL_INSTANCE_PROPERTY(FijiListInfo_t, FijiListInfo)
  Q_PROPERTY(FijiListInfo_t FijiListInfo READ getFijiListInfo WRITE setFijiListInfo)

  /**
   * @brief Initializes some of the GUI elements with selections or other GUI related items
   */
  virtual void setupGui();

  /**
   * @brief checkComplete
   * @return
   */
  void checkComplete() const override;

  /**
   * @brief getMontageName
   * @return
   */
  QString getMontageName();

  /**
   * @brief getOverrideSpacing
   * @return
   */
  bool getOverrideSpacing();

  /**
   * @brief getSpacing
   * @return
   */
  FloatVec3Type getSpacing();

  /**
   * @brief getOverrideOrigin
   * @return
   */
  bool getOverrideOrigin();

  /**
   * @brief getOrigin
   * @return
   */
  FloatVec3Type getOrigin();

  /**
   * @brief getMontageStart
   * @return
   */
  IntVec2Type getMontageStart();

  /**
   * @brief getMontageEnd
   * @return
   */
  IntVec2Type getMontageEnd();

  /**
   * @brief getLengthUnit
   * @return
   */
  int32_t getLengthUnit();

protected:
  /**
   * @brief Constructor
   * @param parameter The FilterParameter object that this widget represents
   * @param filter The instance of the filter that this parameter is a part of
   * @param parent The parent QWidget for this Widget
   */
  ImportFijiMontageDialog(QWidget* parent = nullptr);

protected slots:

  // Slots to catch signals emitted by the various ui widgets
  void changeOrigin_stateChanged(int state);
  void changeSpacing_stateChanged(int state);
  void fijiListWidgetChanged();

protected:
  static void setOpenDialogLastFilePath(const QString& val)
  {
    m_OpenDialogLastDirectory = val;
  }
  static QString getOpenDialogLastFilePath()
  {
    return m_OpenDialogLastDirectory;
  }

signals:
  /**
   * @brief dataFileChanged
   * @param dataFile
   */
  void dataFileChanged(const QString& dataFile);

private:
  QSharedPointer<Ui::ImportFijiMontageDialog> m_Ui;

  static QString m_OpenDialogLastDirectory;

  /**
   * @brief connectSignalsSlots
   */
  void connectSignalsSlots();

  /**
   * @brief disconnectSignalsSlots
   */
  void disconnectSignalsSlots();

public:
  ImportFijiMontageDialog(const ImportFijiMontageDialog&) = delete;            // Copy Constructor Not Implemented
  ImportFijiMontageDialog(ImportFijiMontageDialog&&) = delete;                 // Move Constructor Not Implemented
  ImportFijiMontageDialog& operator=(const ImportFijiMontageDialog&) = delete; // Copy Assignment Not Implemented
  ImportFijiMontageDialog& operator=(ImportFijiMontageDialog&&) = delete;      // Move Assignment Not Implemented
};
