/* ============================================================================
 * Copyright (c) 2009-2019 BlueQuartz Software, LLC
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
 * The code contained herein was partially funded by the following contracts:
 *    United States Air Force Prime Contract FA8650-07-D-5800
 *    United States Air Force Prime Contract FA8650-10-D-5210
 *    United States Prime Contract Navy N00173-07-C-2068
 *    United States Air Force Prime Contract FA8650-15-D-5231
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "AbstractFilterHandler.h"

#include "VisualFilters/VSClipFilter.h"
#include "VisualFilters/VSCropFilter.h"
#include "VisualFilters/VSDataSetFilter.h"
#include "VisualFilters/VSFileNameFilter.h"
#include "VisualFilters/VSMaskFilter.h"
#include "VisualFilters/VSPipelineFilter.h"
#include "VisualFilters/VSRootFilter.h"
#include "VisualFilters/VSSIMPLDataContainerFilter.h"
#include "VisualFilters/VSSliceFilter.h"
#include "VisualFilters/VSTextFilter.h"
#include "VisualFilters/VSThresholdFilter.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilterHandler::AbstractFilterHandler() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractFilterHandler::processMessage(const VSClipFilter* filter) const
{
  /* This is a default method that can be reimplemented in a subclass.  Subclassed filter handlers
   * should reimplement this method if they care about processing clip filters. */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractFilterHandler::processMessage(const VSCropFilter* filter) const
{
  /* This is a default method that can be reimplemented in a subclass.  Subclassed filter handlers
   * should reimplement this method if they care about processing crop filters. */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractFilterHandler::processMessage(const VSDataSetFilter* filter) const
{
  /* This is a default method that can be reimplemented in a subclass.  Subclassed filter handlers
   * should reimplement this method if they care about processing dataset filters. */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractFilterHandler::processMessage(const VSFileNameFilter* filter) const
{
  /* This is a default method that can be reimplemented in a subclass.  Subclassed filter handlers
   * should reimplement this method if they care about processing file name filters. */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractFilterHandler::processMessage(const VSMaskFilter* filter) const
{
  /* This is a default method that can be reimplemented in a subclass.  Subclassed filter handlers
   * should reimplement this method if they care about processing mask filters. */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractFilterHandler::processMessage(const VSPipelineFilter* filter) const
{
  /* This is a default method that can be reimplemented in a subclass.  Subclassed filter handlers
   * should reimplement this method if they care about processing pipeline filters. */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractFilterHandler::processMessage(const VSRootFilter* filter) const
{
  /* This is a default method that can be reimplemented in a subclass.  Subclassed filter handlers
   * should reimplement this method if they care about processing root filters. */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractFilterHandler::processMessage(const VSSIMPLDataContainerFilter* filter) const
{
  /* This is a default method that can be reimplemented in a subclass.  Subclassed filter handlers
   * should reimplement this method if they care about processing data container filters. */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractFilterHandler::processMessage(const VSSliceFilter* filter) const
{
  /* This is a default method that can be reimplemented in a subclass.  Subclassed filter handlers
   * should reimplement this method if they care about processing slice filters. */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractFilterHandler::processMessage(const VSTextFilter* filter) const
{
  /* This is a default method that can be reimplemented in a subclass.  Subclassed filter handlers
   * should reimplement this method if they care about processing text filters. */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void AbstractFilterHandler::processMessage(const VSThresholdFilter* filter) const
{
  /* This is a default method that can be reimplemented in a subclass.  Subclassed filter handlers
   * should reimplement this method if they care about processing threshold filters. */
}
