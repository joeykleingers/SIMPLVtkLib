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

#include "VSMontageImporter.h"

#include "SIMPLib/Geometry/ImageGeom.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSMontageImporter::VSMontageImporter(FilterPipeline::Pointer pipeline, MontageMetadata::DisplayType displayType)
: VSAbstractImporter()
, m_Pipeline(pipeline)
, m_DisplayType(displayType)
{
  pipeline->addMessageReceiver(this);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSMontageImporter::VSMontageImporter(FilterPipeline::Pointer pipeline, DataContainerArray::Pointer dataContainerArray, MontageMetadata::DisplayType displayType)
: VSAbstractImporter()
, m_Pipeline(pipeline)
, m_DataContainerArray(dataContainerArray)
, m_DisplayType(displayType)
{
  pipeline->addMessageReceiver(this);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSMontageImporter::~VSMontageImporter() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSMontageImporter::Pointer VSMontageImporter::New(FilterPipeline::Pointer pipeline, MontageMetadata::DisplayType displayType)
{
  VSMontageImporter::Pointer sharedPtr(new VSMontageImporter(pipeline, displayType));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSMontageImporter::Pointer VSMontageImporter::New(FilterPipeline::Pointer pipeline, DataContainerArray::Pointer dataContainerArray, MontageMetadata::DisplayType displayType)
{
  VSMontageImporter::Pointer sharedPtr(new VSMontageImporter(pipeline, dataContainerArray, displayType));
  return sharedPtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSMontageImporter::processPipelineMessage(const AbstractMessage::Pointer& pipelineMsg)
{
  emit notifyMessage(pipelineMsg);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString VSMontageImporter::getName()
{
  return m_Pipeline->getName();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSMontageImporter::execute()
{
  setState(State::Executing);
  if(m_DataContainerArray != nullptr)
  {
    m_Pipeline->execute(m_DataContainerArray);
  }
  else
  {
    m_Pipeline->execute();
  }

  //  qInfo() << "Pipeline err condition: " << err;
  //  // For now, quit after an error condition
  //  // However, may want to consider returning
  //  // list of errors associated with each pipeline
  //  if(err < 0)
  //  {
  //    break;
  //  }

  if(m_Resetting)
  {
    setState(State::Ready);
    m_Resetting = false;
  }
  else if(m_Pipeline->getExecutionResult() == FilterPipeline::ExecutionResult::Canceled)
  {
    setState(State::Canceled);
  }
  else
  {
    setState(State::Finished);
    handleMontageResults();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSMontageImporter::cancel()
{
  if(m_Pipeline->getState() == FilterPipeline::State::Executing)
  {
    m_Pipeline->cancel();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSMontageImporter::reset()
{
  if(m_Pipeline->getState() == FilterPipeline::State::Executing)
  {
    m_Resetting = true;
    m_Pipeline->cancel();
  }
  else
  {
    setState(State::Ready);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSMontageImporter::handleMontageResults()
{
  int err = m_Pipeline->getErrorCode();
  if(err >= 0)
  {
    DataContainerArray::Pointer dca = m_Pipeline->getDataContainerArray();
    QStringList pipelineNameTokens = m_Pipeline->getName().split("_", QString::SplitBehavior::SkipEmptyParts);
    int slice = 0;
    if(pipelineNameTokens.size() > 1)
    {
      slice = pipelineNameTokens[1].toInt();
    }

    // If Display Montage was selected, remove non-stitched image data containers
    if(m_DisplayType == MontageMetadata::DisplayType::Montage)
    {
      for(const DataContainer::Pointer& dc : dca->getDataContainers())
      {
        if(dc->getName() == "MontageDC")
        {
          ImageGeom::Pointer imageGeom = dc->getGeometryAs<ImageGeom>();
          if(imageGeom)
          {
            FloatVec3Type origin = imageGeom->getOrigin();
            origin[2] += slice;
            imageGeom->setOrigin(origin);
          }
        }
        else
        {
          dca->removeDataContainer(dc->getName());
        }
      }
    }

    emit montageResultsReady(m_Pipeline, dca);
  }
}
