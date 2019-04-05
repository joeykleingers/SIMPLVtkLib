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

#include <QtCore/QFileSystemWatcher>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

#include "SIMPLVtkLib/QtWidgets/VSAbstractImporter.h"

#include "SVWidgetsLib/QtSupport/QtSSettings.h"
#include "SVWidgetsLib/SVWidgetsLib.h"

enum AddType
{
  PREPEND,
  APPEND
};

class QMenu;

/**
 * @class VSRecentImportersList VSRecentImportersList.h Shared/VSRecentImportersList.h
 * @brief Holds a list of recently executed importers. Uses the Singleton design pattern
 * to ensure there is only one list per application instance.
 *
 * @date Oct 2008
 * @version $Revision: 1.2 $
 */
class SVWidgetsLib_EXPORT VSRecentImportersList : public QObject
{

  Q_OBJECT

public:
  VSRecentImportersList(int maxListSize = 12, QObject* parent = nullptr);

  /**
   * @brief Returns the singleton instance of this class.
   * @return Instance of this class
   */
  static VSRecentImportersList* Instance(int maxListSize = 12, QObject* parent = nullptr);

  /**
   * @brief Returns the current list of importers for this application
   * @return
   */
  std::list<VSAbstractImporter::Pointer> importerList();

  /**
   * @brief Adds an importer to the recents list
   * @param importer The importer to add to the list
   */
  void addImporter(const VSAbstractImporter::Pointer& importer, AddType type = PREPEND);

  /**
   * @brief Writes the list of datasets to a preference file.
   * @param prefs The QSettings object to use.
   */
  void writeList(QtSSettings* prefs);

  /**
   * @brief Reads the list of datasets from the preferences file
   * @param prefs The QSettins object to use
   */
  void readList(QtSSettings* prefs);

  /**
   * @brief Clears the list of recent files
   */
  void clear();

public slots:

  /**
   * @brief Removes an importer from the list of recent importers.
   * @param importer The importer to remove
   */
  void removeImporter(const VSAbstractImporter::Pointer& importer);

  /* ******************* Begin Qt Signals ********************************* */
signals:

  /**
   * @brief Qt Signal emitted when the list of importers has changed.
   * @param importer The newly added importer
   */
  void importerListChanged(const VSAbstractImporter::Pointer& importer);

protected:
  /**
   * @brief Returns true if the list already contains the importer.
   * @param importer The importer to check.
   */
  bool contains(const VSAbstractImporter::Pointer& importer);

private:
  static VSRecentImportersList* self;

  ~VSRecentImportersList() override;

  std::list<VSAbstractImporter::Pointer> recentImporters;
  int m_MaxListSize = 0;

  VSRecentImportersList(const VSRecentImportersList&); // Copy Constructor Not Implemented
  void operator=(const VSRecentImportersList&);        // Copy Assignment Not Implemented
};
