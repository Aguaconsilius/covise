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

#ifndef OSCEDITORTOOL_HPP
#define OSCEDITORTOOL_HPP

#include "editortool.hpp"

#include "toolaction.hpp"
#include "src/util/odd.hpp"

#include <QComboBox>

#include "ui_OSCRibbon.h"


class OpenScenarioEditorTool : public EditorTool
{
    Q_OBJECT

        //################//
        // FUNCTIONS      //
        //################//

public:
    explicit OpenScenarioEditorTool(ToolManager *toolManager);
    virtual ~OpenScenarioEditorTool()
    { /* does nothing */
    }


    void enableGraphEdit(bool);
    void setButtonColor(const QString &name, QColor color);
    void objectSelection(bool);

private:
    OpenScenarioEditorTool(); /* not allowed */
    OpenScenarioEditorTool(const OpenScenarioEditorTool &); /* not allowed */
    OpenScenarioEditorTool &operator=(const OpenScenarioEditorTool &); /* not allowed */

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
    void activateRibbonEditor();
    void handleToolClick(int);
    void handleCatalogSelection(int);
    void handleGraphState(bool);

    //################//
    // PROPERTIES     //
    //################//

private:
    ODD::ToolId toolId_;
    bool graphEdit_;
    Ui::OSCRibbon *ui;

    QButtonGroup *ribbonToolGroup_;
};

class OpenScenarioEditorToolAction : public ToolAction
{

    //################//
    // FUNCTIONS      //
    //################//

public:
    explicit OpenScenarioEditorToolAction(ODD::ToolId toolId, const QString &text);
    explicit OpenScenarioEditorToolAction(ODD::ToolId toolId, bool state);
    virtual ~OpenScenarioEditorToolAction()
    { /* does nothing */
    }

    QString getText() const
    {
        return text_;
    }

    bool getState() const
    {
        return state_;
    }

private:
    OpenScenarioEditorToolAction(); /* not allowed */
    OpenScenarioEditorToolAction(const OpenScenarioEditorToolAction &); /* not allowed */
    OpenScenarioEditorToolAction &operator=(const OpenScenarioEditorToolAction &); /* not allowed */

    //################//
    // PROPERTIES     //
    //################//

private:
    QString text_;
    bool state_;
};

#endif // OSCEDITORTOOL_HPP
