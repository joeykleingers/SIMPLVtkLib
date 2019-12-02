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

#include <list>

#include "Database/MontageMetadata.h"

class SIMPLVtkLib_EXPORT EbsdMontageMetadata : public MontageMetadata
{
public:
  enum class ScanOverlapType
  {
    None,
    PixelOverlap,
    PercentOverlap
  };

  /**
   * @brief getInputDirectory
   * @return
   */
  QString getInputDirectory() const;

  /**
   * @brief setInputDirectory
   * @param val
   * @return
   */
  void setInputDirectory(const QString& val);

  /**
   * @brief getFilePrefix
   * @return
   */
  QString getFilePrefix() const;

  /**
   * @brief setFilePrefix
   * @param val
   * @return
   */
  void setFilePrefix(const QString& val);

  /**
   * @brief getFileSuffix
   * @return
   */
  QString getFileSuffix() const;

  /**
   * @brief setFileSuffix
   * @param val
   * @return
   */
  void setFileSuffix(const QString& val);

  /**
   * @brief getFileExtension
   * @return
   */
  QString getFileExtension() const;

  /**
   * @brief setFileExtension
   * @param val
   * @return
   */
  void setFileExtension(const QString& val);

  /**
   * @brief getIncrementIndex
   * @return
   */
  int getIncrementIndex() const;

  /**
   * @brief setIncrementIndex
   * @param val
   * @return
   */
  void setIncrementIndex(const int& val);

  /**
   * @brief getNumIndexDigits
   * @return
   */
  int getNumIndexDigits() const;

  /**
   * @brief setNumIndexDigits
   * @param val
   * @return
   */
  void setNumIndexDigits(const int& val);

  /**
   * @brief getFilePaths
   * @return
   */
  std::list<QString> getFilePaths() const;

  /**
   * @brief setFilePaths
   * @param val
   * @return
   */
  void setFilePaths(const std::list<QString>& val);

  /**
   * @brief getScanOverlapType
   * @return
   */
  ScanOverlapType getScanOverlapType() const;

  /**
   * @brief setScanOverlapType
   * @param val
   * @return
   */
  void setScanOverlapType(const ScanOverlapType& val);

  /**
   * @brief getGenerateIPFColorMap
   * @return
   */
  bool getGenerateIPFColorMap() const;

  /**
   * @brief setGenerateIPFColorMap
   * @param val
   * @return
   */
  void setGenerateIPFColorMap(const bool& val);

  /**
   * @brief getPixelOverlap
   * @return
   */
  IntVec2Type getPixelOverlap() const;

  /**
   * @brief setPixelOverlap
   * @param val
   * @return
   */
  void setPixelOverlap(const IntVec2Type& val);

  /**
   * @brief getPercentOverlap
   * @return
   */
  FloatVec2Type getPercentOverlap() const;

  /**
   * @brief setPercentOverlap
   * @param val
   * @return
   */
  void setPercentOverlap(const FloatVec2Type& val);

private:
  QString m_InputDirectory;
  QString m_FilePrefix;
  QString m_FileSuffix;
  QString m_FileExtension;
  int m_IncrementIndex;
  int m_NumIndexDigits;
  std::list<QString> m_FilePaths;
  ScanOverlapType m_ScanOverlapType = ScanOverlapType::None;
  IntVec2Type m_PixelOverlap = {0, 0};
  FloatVec2Type m_PercentOverlap = {0.0F, 0.0F};
  bool m_GenerateIPFColorMap = false;
};
