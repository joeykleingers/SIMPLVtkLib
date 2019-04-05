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

#include "VSDatasetImporter.h"

#include <QtCore/QFileInfo>

#include "SIMPLVtkLib/QtWidgets/VSAbstractImporterHandler.h"
#include "SIMPLVtkLib/Visualization/VisualFilters/VSDataSetFilter.h"
#include "SIMPLVtkLib/Visualization/VisualFilters/VSFileNameFilter.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSDatasetImporter::VSDatasetImporter(VSFileNameFilter* textFilter, VSDataSetFilter* filter)
: VSAbstractImporter()
, m_TextFilter(textFilter)
, m_DatasetFilter(filter)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSDatasetImporter::~VSDatasetImporter()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSDatasetImporter::Pointer VSDatasetImporter::New(VSFileNameFilter* textFilter,
  VSDataSetFilter* filter)
{
  VSDatasetImporter::Pointer sharedPtr(new VSDatasetImporter(textFilter, filter));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString VSDatasetImporter::getName()
{
  return m_TextFilter->getFilePath();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSDatasetImporter::execute()
{
  setState(State::Executing);

  if(m_TextFilter == nullptr || m_DatasetFilter == nullptr)
  {
	cancel();
  }

  setState(State::Finished);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSDatasetImporter::cancel()
{
  // Cancel
  setState(State::Canceled);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSDatasetImporter::reset()
{
  // Cancel
  setState(State::Ready);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSAbstractImporter::Pointer VSDatasetImporter::deepCopy() const
{
  VSFileNameFilter* textFilter = new VSFileNameFilter(m_TextFilter->getFilePath(), m_TextFilter->getParentFilter());
  VSDataSetFilter* datasetFilter = new VSDataSetFilter(m_DatasetFilter->getFilePath(), m_DatasetFilter->getParentFilter());

  VSDatasetImporter::Pointer datasetImporter = VSDatasetImporter::New(textFilter, datasetFilter);
  return datasetImporter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSDatasetImporter::visit(VSAbstractImporterHandler* handler) const
{
  handler->processImporter(this);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSFileNameFilter* VSDatasetImporter::getFileNameFilter() const
{
  return m_TextFilter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSDataSetFilter* VSDatasetImporter::getDataSetFilter() const
{
  return m_DatasetFilter;
}
