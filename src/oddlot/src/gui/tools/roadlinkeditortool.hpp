/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

 /**************************************************************************
 ** ODD: OpenDRIVE Designer
 **   Frank Naegele (c) 2010
 **   <mail@f-naegele.de>
 **   11/2/2010
 **
 **************************************************************************/

#ifndef ROADLINKEDITORTOOL_HPP
#define ROADLINKEDITORTOOL_HPP

#include "editortool.hpp"

#include "toolaction.hpp"
#include "src/util/odd.hpp"

#include "ui_RoadLinkRibbon.h"

class QDoubleSpinBox;

class RoadLinkEditorTool : public EditorTool
{
    Q_OBJECT

        //################//
        // FUNCTIONS      //
        //################//

public:
    explicit RoadLinkEditorTool(ToolManager *toolManager);
    virtual ~RoadLinkEditorTool()
    { /* does nothing */
    }

private:
    RoadLinkEditorTool(); /* not allowed */
    RoadLinkEditorTool(const RoadLinkEditorTool &); /* not allowed */
    RoadLinkEditorTool &operator=(const RoadLinkEditorTool &); /* not allowed */

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
    void setThreshold();

    //################//
    // PROPERTIES     //
    //################//

private:
    Ui::RoadLinkRibbon *ui;
    ODD::ToolId toolId_;
    QDoubleSpinBox *thresholdEdit_;
    QButtonGroup *ribbonToolGroup_;
};

class RoadLinkEditorToolAction : public ToolAction
{

    //################//
    // FUNCTIONS      //
    //################//

public:
    explicit RoadLinkEditorToolAction(ODD::ToolId toolId, ODD::ToolId paramToolId = ODD::TNO_TOOL);
    virtual ~RoadLinkEditorToolAction()
    { /* does nothing */
    }

private:
    RoadLinkEditorToolAction(); /* not allowed */
    RoadLinkEditorToolAction(const RoadLinkEditorToolAction &); /* not allowed */
    RoadLinkEditorToolAction &operator=(const RoadLinkEditorToolAction &); /* not allowed */

    //################//
    // PROPERTIES     //
    //################//

private:
};

#endif // ROADLINKEDITORTOOL_HPP
