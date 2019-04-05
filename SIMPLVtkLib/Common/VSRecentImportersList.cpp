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

#include "VSRecentImportersList.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QSettings>
#include <QtWidgets/QMenu>

VSRecentImportersList* VSRecentImportersList::self = nullptr;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSRecentImportersList::VSRecentImportersList(int maxListSize, QObject* parent)
: QObject(parent)
, m_MaxListSize(maxListSize)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSRecentImportersList::~VSRecentImportersList()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSRecentImportersList* VSRecentImportersList::Instance(int maxListSize, QObject* parent)
{
  if(self == nullptr)
  {
    self = new VSRecentImportersList(maxListSize, parent);
  }
  return self;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool VSRecentImportersList::contains(const VSAbstractImporter::Pointer& importer)
{
  return (std::find(recentImporters.begin(), recentImporters.end(), importer) != recentImporters.end());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSRecentImportersList::addImporter(const VSAbstractImporter::Pointer& importer, AddType type)
{
  if(contains(importer))
  {
    // Remove the file from wherever it is in the list
    removeImporter(importer);
  }

  if(recentImporters.size() == m_MaxListSize)
  {
    recentImporters.pop_back();
  }

  if(type == APPEND)
  {
    this->recentImporters.emplace_back(importer);
  }
  else
  {
    this->recentImporters.emplace(recentImporters.begin(), importer);
  }

  emit importerListChanged(importer); // Emit the signal so all the menus can update their contents
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
std::list<VSAbstractImporter::Pointer> VSRecentImportersList::importerList()
{
  return this->recentImporters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSRecentImportersList::removeImporter(const VSAbstractImporter::Pointer& importer)
{
  std::list<VSAbstractImporter::Pointer>::iterator findIter = find(recentImporters.begin(), recentImporters.end(), importer);

  if(findIter != recentImporters.end())
  {
    this->recentImporters.erase(findIter);

    emit importerListChanged(importer); // Emit the signal so all the menus can update their contents
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSRecentImportersList::clear()
{
  this->recentImporters.clear();

  emit importerListChanged(VSAbstractImporter::NullPointer());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSRecentImportersList::writeList(QtSSettings* prefs)
{
  //  prefs->setValue("Recent Importers", this->importerList());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSRecentImportersList::readList(QtSSettings* prefs)
{
  this->clear();

  QStringList list = prefs->value("Recent Files", QStringList());

  for(int i = 0; i < list.size(); i++)
  {
    QString filePath = list[i];
    QFile file(filePath);
    if(file.exists())
    {
      //      this->addImporter(filePath, APPEND);
    }
  }
}
