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

#include "VSAbstractFilterValues.h"

class VSCropFilter;

/**
 * @class VSCropValues VSCropValues.h SIMPLVtkLib/Visualization/VisualFilters/VSCropValues.h
 * @brief Stores values for a given VSCropFilter and allows the user to edit them in the user interface.
 */
class SIMPLVtkLib_EXPORT VSCropValues : public VSAbstractFilterValues
{
  Q_OBJECT

public:
  using FilterType = VSCropFilter;

  VSCropValues(VSCropFilter* filter);
  VSCropValues(const VSCropValues& values);
  virtual ~VSCropValues();

  /**
   * @brief Returns the current filter as a VSCropFilter
   * @return
   */
  VSCropFilter* getCropFilter() const;

  /**
   * @brief Applies the current values to the selected crop filters
   */
  void applyValues() override;

  /**
   * @brief Resets the selected filters to their last applied values.
   */
  void resetValues() override;

  /**
   * @brief Returns true if there are changes waiting to be applied.  Returns false otherwise.
   * @return
   */
  bool hasChanges() const override;

  /**
   * @brief Creates and returns a new filter widget
   * @return
   */
  QWidget* createFilterWidget() override;

  /**
   * @brief Returns the current volume of interest
   * @return
   */
  int* getVOI() const;

  /**
   * @brief Returns the current sample rate
   * @return
   */
  int* getSampleRate() const;

  /**
   * @brief Sets the volume of interest
   * @param volumeOfInterest
   */
  void setVOI(int volumeOfInterest[6]);

  /**
   * @brief Sets the sample rate
   * @param sampleRate
   */
  void setSampleRate(int sampleRate[3]);

  /**
   * @brief Updates the last applied volume of interest
   * @param voi
   */
  void setLastVOI(int voi[6]);

  /**
   * @brief Updates the last applied sample rate
   * @param sampleRate
   */
  void setLastSampleRate(int sampleRate[3]);

  /**
   * @brief Writes values to Json
   * @param json
   */
  void writeJson(QJsonObject& json);

  /**
   * @brief Loads values from Json
   * @param json
   */
  void loadJson(QJsonObject& json);

signals:
  void volumeOfInterestChanged(int voi[6]);
  void sampleRateChanged(int sampleRate[3]);
  void lastVolumeOfInterestChanged(int lastVOI[6]);
  void lastSampleRateChanged(int lastSampleRate[3]);

private:
  int* m_Voi;
  int* m_SampleRate;
  int m_LastVoi[6];
  int m_LastSampleRate[3];
  bool m_FreshFilter = true;
};
