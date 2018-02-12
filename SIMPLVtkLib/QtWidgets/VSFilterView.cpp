/* ============================================================================
* Copyright (c) 2009-2017 BlueQuartz Software, LLC
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

#include "VSFilterView.h"

#include "SIMPLVtkLib/Visualization/Controllers/VSFilterModel.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSFilterView::VSFilterView(QWidget* parent)
  : QTreeView(parent)
{
  connectSlots();
  setupGui();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterView::setupGui()
{
  setHeaderHidden(true);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterView::setController(VSController* controller)
{
  if(selectionModel())
  {
    disconnect(selectionModel(), SIGNAL(currentItemChanged(const QModelIndex&, const QModelIndex&)),
      this, SLOT(setCurrentItem(const QModelIndex&, const QModelIndex&)));
  }

  if(m_Controller)
  {
    disconnect(m_Controller, SIGNAL(filterAdded(VSAbstractFilter*)),
      this, SLOT(insertFilter(VSAbstractFilter*)));
  }

  m_Controller = controller;
  setModel(controller->getFilterModel());

  connect(m_Controller, SIGNAL(filterAdded(VSAbstractFilter*)),
    this, SLOT(insertFilter(VSAbstractFilter*)));
  connect(selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
    this, SLOT(setCurrentItem(const QModelIndex&, const QModelIndex&)));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterView::insertFilter(VSAbstractFilter* filter)
{
  VSFilterModel* filterModel = dynamic_cast<VSFilterModel*>(model());

  QModelIndex index = filterModel->getIndexFromFilter(filter);
  expand(index);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterView::setViewWidget(VSAbstractViewWidget* viewWidget)
{
  // Disconnect from the old view controller
  disconnect(m_ViewWidget, SIGNAL(visibilityChanged(VSFilterViewSettings*, bool)),
    this, SLOT(setFilterVisibility(VSFilterViewSettings*, bool)));

  m_ViewWidget = viewWidget;
  if(nullptr == m_Controller || nullptr == m_ViewWidget)
  {
    return;
  }

  // Connect to the new view controller
  connect(m_ViewWidget, SIGNAL(visibilityChanged(VSFilterViewSettings*, bool)),
    this, SLOT(setFilterVisibility(VSFilterViewSettings*, bool)));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterView::setFilterVisibility(VSFilterViewSettings* filterSettings, bool visible)
{
  VSAbstractFilter* filter = filterSettings->getFilter();
  if(filter)
  {
    filter->setCheckState(visible ? Qt::Checked : Qt::Unchecked);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterView::setActiveFilter(VSAbstractFilter* filter, VSAbstractFilterWidget* widget)
{
  VSFilterModel* filterModel = dynamic_cast<VSFilterModel*>(model());
  if(nullptr == filterModel)
  {
    return;
  }
  if(nullptr == selectionModel())
  {
    return;
  }
  if(nullptr == filter)
  {
    selectionModel()->clear();
    return;
  }

  QModelIndex currentIndex = selectionModel()->currentIndex();
  QModelIndex newIndex = filterModel->getIndexFromFilter(filter);

  // Do not update the selection if there is no change
  if(newIndex != currentIndex)
  {
    QFlags<QItemSelectionModel::SelectionFlag> flags = QItemSelectionModel::ClearAndSelect;
    selectionModel()->setCurrentIndex(newIndex, flags);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterView::itemClicked(const QModelIndex& index)
{
  VSAbstractFilter* filter = getFilterFromIndex(index);
  if(m_ViewWidget && filter)
  {
    VSFilterViewSettings* settings = m_ViewWidget->getFilterViewSettings(filter);
    bool checked = filter->checkState() == Qt::Checked;
    if(settings)
    {
      settings->setVisible(checked);
    }
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterView::setCurrentItem(const QModelIndex& current, const QModelIndex& previous)
{
  VSAbstractFilter* filter = getFilterFromIndex(current);
  if(nullptr == filter)
  {
    return;
  }

  emit filterClicked(filter);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void VSFilterView::connectSlots()
{
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(itemClicked(const QModelIndex&)));
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VSAbstractFilter* VSFilterView::getFilterFromIndex(const QModelIndex& index)
{
  VSFilterModel* filterModel = dynamic_cast<VSFilterModel*>(model());
  return filterModel->getFilterFromIndex(index);
}