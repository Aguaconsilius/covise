/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

/**************************************************************************
** ODD: OpenDRIVE Designer
**   Frank Naegele (c) 2010
**   <mail@f-naegele.de>
**   06.04.2010
**
**************************************************************************/

#ifndef JUNCTIONEDITORTOOL_HPP
#define JUNCTIONEDITORTOOL_HPP

#include "editortool.hpp"

#include "toolaction.hpp"
#include "src/util/odd.hpp"

#include "src/data/prototypemanager.hpp"
#include "ui_JunctionRibbon.h"

// Qt //
//
#include <QMap>
#include <QDoubleSpinBox>
#include <QPushButton>

class QGroupBox;
//class QAction;
//class QMenu;
//class QToolButton;

class JunctionEditorTool : public EditorTool
{
    Q_OBJECT

    //################//
    // STATIC         //
    //################//

public:
    /*! \brief Ids of the TrackEditor tools.
	*
	* This enum defines the Id of each tool.
	*/
    //	enum TrackEditorToolId
    //	{
    //		TTE_UNKNOWN,
    //		TTE_SELECT,
    //		TTE_INSERT,
    //		TTE_DELETE
    //	};

    //################//
    // FUNCTIONS      //
    //################//

public:
    explicit JunctionEditorTool(ToolManager *toolManager);
    virtual ~JunctionEditorTool()
    { /* does nothing */
    }

    QPushButton *getCuttingCircleButton()
    {
        return cuttingCircleButton_;
    };

protected:
private:
    JunctionEditorTool(); /* not allowed */
    JunctionEditorTool(const JunctionEditorTool &); /* not allowed */
    JunctionEditorTool &operator=(const JunctionEditorTool &); /* not allowed */

    void initToolBar();
    void initToolWidget();

//################//
// SIGNALS        //
//################//

signals:
    void toolAction(ToolAction *);

    //################//
    // SLOTS          //
    //################//

public slots:
    void activateEditor();
	void activateRibbonEditor();
    void handleToolClick(int);
	void handleRibbonToolClick(int);

    //################//
    // PROPERTIES     //
    //################//

protected:
private:
    Ui::JunctionRibbon *ui;
    ODD::ToolId toolId_;
    QPushButton *cuttingCircleButton_;

	QButtonGroup *ribbonToolGroup_;
};

class JunctionEditorToolAction : public ToolAction
{

    //################//
    // FUNCTIONS      //
    //################//

public:
    explicit JunctionEditorToolAction(ODD::ToolId toolId, ODD::ToolId paramToolId = ODD::TNO_TOOL);
    virtual ~JunctionEditorToolAction()
    { /* does nothing */
    }


protected:
private:
    JunctionEditorToolAction(); /* not allowed */
    JunctionEditorToolAction(const JunctionEditorToolAction &); /* not allowed */
    JunctionEditorToolAction &operator=(const JunctionEditorToolAction &); /* not allowed */

    //################//
    // PROPERTIES     //
    //################//

protected:
private:

};

#endif // JUNCTIONEDITORTOOL_HPP
