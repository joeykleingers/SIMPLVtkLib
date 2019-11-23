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

#include "SIMPLib/Common/SIMPLArray.hpp"

#include "Database/MontageMetadata.h"

class SIMPLVtkLib_EXPORT RobometMontageMetadata : public MontageMetadata
{
public:
  /**
   * @brief getFilePath
   * @return
   */
  QString getFilePath() const;

  /**
   * @brief setFilePath
   * @param val
   * @return
   */
  void setFilePath(const QString& val);

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
  void setFilePrefix(QString val);

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
  void setFileExtension(QString val);

  /**
   * @brief getSliceLimits
   * @return
   */
  IntVec2Type getSliceLimits() const;

  /**
   * @brief setSliceLimits
   * @param val
   * @return
   */
  void setSliceLimits(IntVec2Type val);

  /**
   * @brief getChangeOrigin
   * @return
   */
  bool getChangeOrigin() const;

  /**
   * @brief setChangeOrigin
   * @param val
   * @return
   */
  void setChangeOrigin(const bool& val);

  /**
   * @brief getOrigin
   * @return
   */
  FloatVec3Type getOrigin() const;

  /**
   * @brief setOrigin
   * @param val
   * @return
   */
  void setOrigin(const FloatVec3Type& val);

  /**
   * @brief getChangeSpacing
   * @return
   */
  bool getChangeSpacing() const;

  /**
   * @brief setChangeSpacing
   * @param val
   * @return
   */
  void setChangeSpacing(bool val);

  /**
   * @brief getSpacing
   * @return
   */
  FloatVec3Type getSpacing() const;

  /**
   * @brief setSpacing
   * @param val
   * @return
   */
  void setSpacing(FloatVec3Type val);

  /**
   * @brief getLengthUnitsIdx
   * @return
   */
  int32_t getLengthUnitsIdx() const;

  /**
   * @brief setLengthUnits
   * @param val
   * @return
   */
  void setLengthUnitsIdx(const int32_t& val);

private:
  QString m_FilePath;
  QString m_FilePrefix;
  QString m_FileExtension;
  IntVec2Type m_SliceLimits;
  bool m_ChangeOrigin = false;
  FloatVec3Type m_Origin = {0.0F, 0.0F, 0.0F};
  bool m_ChangeSpacing = false;
  FloatVec3Type m_Spacing = {0.0F, 0.0F, 0.0F};
  int32_t m_LengthUnitsIdx = 0;
};
