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

#include "EbsdMontageMetadata.h"

// -----------------------------------------------------------------------------
QString EbsdMontageMetadata::getInputDirectory() const
{
  return m_InputDirectory;
}

// -----------------------------------------------------------------------------
void EbsdMontageMetadata::setInputDirectory(const QString& val)
{
  m_InputDirectory = val;
}

// -----------------------------------------------------------------------------
QString EbsdMontageMetadata::getFilePrefix() const
{
  return m_FilePrefix;
}

// -----------------------------------------------------------------------------
void EbsdMontageMetadata::setFilePrefix(const QString& val)
{
  m_FilePrefix = val;
}

// -----------------------------------------------------------------------------
QString EbsdMontageMetadata::getFileSuffix() const
{
  return m_FileSuffix;
}

// -----------------------------------------------------------------------------
void EbsdMontageMetadata::setFileSuffix(const QString& val)
{
  m_FileSuffix = val;
}

// -----------------------------------------------------------------------------
QString EbsdMontageMetadata::getFileExtension() const
{
  return m_FileExtension;
}

// -----------------------------------------------------------------------------
void EbsdMontageMetadata::setFileExtension(const QString& val)
{
  m_FileExtension = val;
}

// -----------------------------------------------------------------------------
int EbsdMontageMetadata::getIncrementIndex() const
{
  return m_IncrementIndex;
}

// -----------------------------------------------------------------------------
void EbsdMontageMetadata::setIncrementIndex(const int& val)
{
  m_IncrementIndex = val;
}

// -----------------------------------------------------------------------------
int EbsdMontageMetadata::getNumIndexDigits() const
{
  return m_NumIndexDigits;
}

// -----------------------------------------------------------------------------
void EbsdMontageMetadata::setNumIndexDigits(const int& val)
{
  m_NumIndexDigits = val;
}

// -----------------------------------------------------------------------------
std::list<QString> EbsdMontageMetadata::getFilePaths() const
{
  return m_FilePaths;
}

// -----------------------------------------------------------------------------
void EbsdMontageMetadata::setFilePaths(const std::list<QString>& val)
{
  m_FilePaths = val;
}

// -----------------------------------------------------------------------------
int32_t EbsdMontageMetadata::getScanTypeOverlapIdx() const
{
  return m_ScanTypeOverlapIdx;
}

// -----------------------------------------------------------------------------
void EbsdMontageMetadata::setScanTypeOverlapIdx(const int32_t& val)
{
  m_ScanTypeOverlapIdx = val;
}

// -----------------------------------------------------------------------------
bool EbsdMontageMetadata::getGenerateIPFColorMap() const
{
  return m_GenerateIPFColorMap;
}

// -----------------------------------------------------------------------------
void EbsdMontageMetadata::setGenerateIPFColorMap(const bool& val)
{
  m_GenerateIPFColorMap = val;
}

// -----------------------------------------------------------------------------
int EbsdMontageMetadata::getPixelOverlap() const
{
  return m_PixelOverlap;
}

// -----------------------------------------------------------------------------
void EbsdMontageMetadata::setPixelOverlap(const int& val)
{
  m_PixelOverlap = val;
}

// -----------------------------------------------------------------------------
float EbsdMontageMetadata::getPercentOverlap() const
{
  return m_PercentOverlap;
}

// -----------------------------------------------------------------------------
void EbsdMontageMetadata::setPercentOverlap(const float& val)
{
  m_PercentOverlap = val;
}
