/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

/**************************************************************************
** ODD: OpenDRIVE Designer
**   Frank Naegele (c) 2010
**   <mail@f-naegele.de>
**   10/18/2010
**
**************************************************************************/

#include "laneeditortool.hpp"

#include "toolmanager.hpp"
#include "toolwidget.hpp"

#include "src/mainwindow.hpp"

// Qt //
//
#include <QGridLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QComboBox>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QApplication>

//################//
//                //
// LaneEditorTool //
//                //
//################//

/*! \todo Ownership/destructor
*/
LaneEditorTool::LaneEditorTool(ToolManager *toolManager)
    : EditorTool(toolManager)
    , toolId_(ODD::TLE_SELECT)
{
    // Connect emitted ToolActions to ToolManager //
    //
    connect(this, SIGNAL(toolAction(ToolAction *)), toolManager, SLOT(toolActionSlot(ToolAction *)));

    // Tool Bar //
    //
    initToolBar();
    initToolWidget();
}

void
LaneEditorTool::initToolWidget()
{
    QGridLayout *toolLayout = new QGridLayout;

    // ButtonGroup //
    //
    // A button group so only one button can be checked at a time
    QButtonGroup *toolGroup = new QButtonGroup;
    connect(toolGroup, SIGNAL(buttonClicked(int)), this, SLOT(handleToolClick(int)));

    // Tools //
    //
    QPushButton *toolButton;
    int row = -1; // button row

    toolButton = new QPushButton(tr("Select"));
    toolButton->setCheckable(true);
    toolLayout->addWidget(toolButton, ++row, 0);
    toolGroup->addButton(toolButton, ODD::TLE_SELECT); // button, id
    toolButton->setChecked(true);

    toolButton = new QPushButton(tr("Add Section"));
    toolButton->setCheckable(true);
    toolLayout->addWidget(toolButton, ++row, 0);
    toolGroup->addButton(toolButton, ODD::TLE_ADD); // button, id

    toolButton = new QPushButton(tr("Del Section"));
    toolButton->setCheckable(true);
    toolLayout->addWidget(toolButton, ++row, 0);
    toolGroup->addButton(toolButton, ODD::TLE_DEL); // button, id

    toolButton = new QPushButton(tr("Add Width Section"));
    toolButton->setCheckable(true);
    toolLayout->addWidget(toolButton, ++row, 0);
    toolGroup->addButton(toolButton, ODD::TLE_ADD_WIDTH); // button, id

    QLabel *widthLabel = new QLabel("Width:");
    widthEdit_ = new QDoubleSpinBox();
    widthEdit_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    widthEdit_->setRange(0.0, 20.0);
    widthEdit_->setValue(0.0);
    widthEdit_->setMinimumWidth(80.0);
    widthEdit_->setMaximumWidth(80.0);

    toolLayout->addWidget(widthLabel, ++row, 0);
    toolLayout->addWidget(widthEdit_, ++row, 0);

    connect(widthEdit_, SIGNAL(editingFinished()), this, SLOT(setWidth()));

    // Finish Layout //
    //
    toolLayout->setRowStretch(++row, 1); // row 3 fills the rest of the availlable space
    toolLayout->setColumnStretch(1, 1); // column 1 fills the rest of the availlable space
    // Widget/Layout //
    //
    ToolWidget *toolWidget = new ToolWidget();
    toolWidget->setLayout(toolLayout);
    toolManager_->addToolBoxWidget(toolWidget, tr("Lane Editor"));
    connect(toolWidget, SIGNAL(activated()), this, SLOT(activateEditor()));

    // Ribbon //
    //

    ToolWidget *ribbonWidget = new ToolWidget();
    //ribbonWidget->
    ui = new Ui::LaneRibbon();
    ui->setupUi(ribbonWidget);
    
	ribbonToolGroup_ = new QButtonGroup(toolManager_);
    connect(ribbonToolGroup_, SIGNAL(buttonClicked(int)), this, SLOT(handleRibbonToolClick(int)));
    
    
	ribbonToolGroup_->addButton(ui->select, ODD::TLE_SELECT);
	ribbonToolGroup_->addButton(ui->laneAdd, ODD::TLE_ADD);
	ribbonToolGroup_->addButton(ui->laneDelete, ODD::TLE_DEL);
	ribbonToolGroup_->addButton(ui->laneAddWidth, ODD::TLE_ADD_WIDTH);
	ribbonToolGroup_->addButton(ui->insertButton, ODD::TLE_INSERT);

	connect(ui->handleCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->widthEdit, SIGNAL(editingFinished()), this, SLOT(setRibbonWidth()));

    toolManager_->addRibbonWidget(ribbonWidget, tr("Lane"), ODD::ELN);
	connect(ribbonWidget, SIGNAL(activated()), this, SLOT(activateRibbonEditor()));
}

void
LaneEditorTool::initToolBar()
{
    // no toolbar for me //
}

//################//
// SLOTS          //
//################//

/*! \brief Gets called when this widget (tab) has been activated.
*
* Sends a ToolAction with the current ToolId and Radius.
*/
void
LaneEditorTool::activateEditor()
{
    LaneEditorToolAction *action = new LaneEditorToolAction(toolId_, widthEdit_->value());
    emit toolAction(action);
    delete action;
}

/*! \brief Is called by the toolmanager to initialize the UI */
/* UI sets the values of the current project */
void 
LaneEditorTool::activateRibbonEditor()
{
	ToolAction *action = toolManager_->getLastToolAction(ODD::ELN);

	if (action->getToolId() == ODD::TLE_SELECT_ALL)
	{
		ribbonToolGroup_->button(ODD::TLE_SELECT)->click();
		ui->handleCheckBox->setCheckState(Qt::CheckState::Unchecked);
	}
	else if (action->getToolId() == ODD::TLE_SELECT_CONTROLS)
	{
		ribbonToolGroup_->button(ODD::TLE_SELECT)->click();
		ui->handleCheckBox->setCheckState(Qt::CheckState::Checked);
	}
	else
	{
		LaneEditorToolAction *laneEditorToolAction = dynamic_cast<LaneEditorToolAction *>(action);

		if (action->getToolId() == ODD::TLE_SET_WIDTH)
		{
			emit toolAction(laneEditorToolAction);
		}
		else
		{ 
			ribbonToolGroup_->button(action->getToolId())->click();
		}
	}

}

/*! \brief Gets called when a tool has been selected.
*
* Sends a ToolAction with the current ToolId and Radius.
*/
void
LaneEditorTool::handleToolClick(int id)
{
    toolId_ = (ODD::ToolId)id;

    // Set a tool //
    //
	LaneEditorToolAction *action = new LaneEditorToolAction(toolId_, widthEdit_->value());
    emit toolAction(action);
    delete action;
}

void
LaneEditorTool::handleRibbonToolClick(int id)
{
    toolId_ = (ODD::ToolId)id;

    // Set a tool //
    //
    LaneEditorToolAction *action = new LaneEditorToolAction(toolId_, ui->widthEdit->value());
    emit toolAction(action);
 //   delete action;
}

/*! \brief Gets called when the width has been changed.
*
* Sends a ToolAction with the current ToolId and Width.
*/
void
LaneEditorTool::setWidth()
{
    LaneEditorToolAction *action = new LaneEditorToolAction(ODD::TLE_SET_WIDTH, widthEdit_->value());
    emit toolAction(action);
    delete action;

    QWidget * focusWidget = QApplication::focusWidget();
    if (focusWidget)
    {
        focusWidget->clearFocus();
    }
    widthEdit_->setValue(0.0);
}

void
LaneEditorTool::setRibbonWidth()
{
    LaneEditorToolAction *action = new LaneEditorToolAction(ODD::TLE_SET_WIDTH, ui->widthEdit->value());
    emit toolAction(action);
  //  delete action;

    QWidget * focusWidget = QApplication::focusWidget();
    if (focusWidget)
    {
        focusWidget->clearFocus();
    }
    ui->widthEdit->setValue(0.0);

    ribbonToolGroup_->button(toolId_)->click();
}

void
LaneEditorTool::onCheckBoxStateChanged(int state)
{
	if (state == Qt::CheckState::Unchecked)
	{
		toolId_ = ODD::TLE_SELECT_ALL;

	}
	else if (state == Qt::CheckState::Checked)
	{
		toolId_ = ODD::TLE_SELECT_CONTROLS;
	}

	LaneEditorToolAction *laneEditorToolAction = new LaneEditorToolAction(toolId_, ui->widthEdit->value());
	emit toolAction(laneEditorToolAction);
}

/*void
LaneEditorTool::setEditMode()
{
	LaneEditorToolAction *action = new LaneEditorToolAction(ODD::TLE_SELECT, LaneEditorToolAction::EditMode, ui->widthEdit->value());
	emit toolAction(action);
	delete action;

	QWidget * focusWidget = QApplication::focusWidget();
	if (focusWidget)
	{
		focusWidget->clearFocus();
	}
} */

//################//
//                //
// LaneEditorToolAction //
//                //
//################//

LaneEditorToolAction::LaneEditorToolAction(ODD::ToolId toolId, double width)
    : ToolAction(ODD::ELN, toolId),
	width_(width)
{
}
