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

#include "RobometMontageMetadata.h"

// -----------------------------------------------------------------------------
QString RobometMontageMetadata::getFilePath() const
{
  return m_FilePath;
}

// -----------------------------------------------------------------------------
void RobometMontageMetadata::setFilePath(const QString& val)
{
  m_FilePath = val;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString RobometMontageMetadata::getFilePrefix() const
{
  return m_FilePrefix;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RobometMontageMetadata::setFilePrefix(QString val)
{
  m_FilePrefix = val;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString RobometMontageMetadata::getFileExtension() const
{
  return m_FileExtension;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RobometMontageMetadata::setFileExtension(QString val)
{
  m_FileExtension = val;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IntVec2Type RobometMontageMetadata::getSliceLimits() const
{
  return m_SliceLimits;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RobometMontageMetadata::setSliceLimits(IntVec2Type val)
{
  m_SliceLimits = val;
}

// -----------------------------------------------------------------------------
bool RobometMontageMetadata::getChangeOrigin() const
{
  return m_ChangeOrigin;
}

// -----------------------------------------------------------------------------
void RobometMontageMetadata::setChangeOrigin(const bool& val)
{
  m_ChangeOrigin = val;
}

// -----------------------------------------------------------------------------
FloatVec3Type RobometMontageMetadata::getOrigin() const
{
  return m_Origin;
}

// -----------------------------------------------------------------------------
void RobometMontageMetadata::setOrigin(const FloatVec3Type& val)
{
  m_Origin = val;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool RobometMontageMetadata::getChangeSpacing() const
{
  return m_ChangeSpacing;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RobometMontageMetadata::setChangeSpacing(bool val)
{
  m_ChangeSpacing = val;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatVec3Type RobometMontageMetadata::getSpacing() const
{
  return m_Spacing;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void RobometMontageMetadata::setSpacing(FloatVec3Type val)
{
  m_Spacing = val;
}

// -----------------------------------------------------------------------------
int32_t RobometMontageMetadata::getLengthUnitsIdx() const
{
  return m_LengthUnitsIdx;
}

// -----------------------------------------------------------------------------
void RobometMontageMetadata::setLengthUnitsIdx(const int32_t& val)
{
  m_LengthUnitsIdx = val;
}
