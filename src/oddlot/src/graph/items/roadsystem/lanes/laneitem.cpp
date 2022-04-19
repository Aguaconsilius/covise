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

#include "laneitem.hpp"

 // Data //
 //
#include "src/data/roadsystem/rsystemelementroad.hpp"

#include "src/data/roadsystem/sections/lanesection.hpp"
#include "src/data/roadsystem/sections/lane.hpp"
#include "src/data/roadsystem/sections/laneborder.hpp"
#include "src/data/commands/dataelementcommands.hpp"
#include "src/data/commands/roadcommands.hpp"
#include "src/data/commands/lanesectioncommands.hpp"

// Graph //
//
#include "lanesectionitem.hpp"
#include "lanelinkitem.hpp"
#include "src/graph/topviewgraph.hpp"
#include "src/graph/graphscene.hpp"
#include "src/graph/items/roadsystem/sections/sectionhandle.hpp"

// Editor //
//
#include "src/graph/editors/laneeditor.hpp"

// Utils //
//
#include "src/util/odd.hpp"
#include "src/util/colorpalette.hpp"

// GUI //
//
#include "src/gui/projectwidget.hpp"
#include "src/gui/lodsettings.hpp"

// Qt //
//
#include <QBrush>
#include <QPen>
#include <QCursor>

#include <QMessageBox>

#include "math.h"

// Editor //
//
#include "src/graph/editors/laneeditor.hpp"

//################//
// CONSTRUCTOR    //
//################//

LaneItem::LaneItem(LaneEditor *laneEditor, LaneSectionItem *parentLaneSectionItem, Lane *lane)
    : GraphElement(parentLaneSectionItem, lane)
    , laneEditor_(laneEditor)
    , parentLaneSectionItem_(parentLaneSectionItem)
    , parentLaneSection_(parentLaneSectionItem->getLaneSection())
    , lane_(lane)
    , handlesItem_(NULL)
{
    grandparentRoad_ = parentLaneSection_->getParentRoad();

    init();
}

LaneItem::~LaneItem()
{
    // Observer Pattern //
    //
    grandparentRoad_->detachObserver(this);
}

void
LaneItem::init()
{
    // Observer Pattern //
    //
    grandparentRoad_->attachObserver(this);

    // Selection/Hovering //
    //
    setAcceptHoverEvents(true);
    setSelectable();

    // Color & Path //
    //
    updateColor();
    createPath();

    // LaneLinkItem //
    //
    // new LaneLinkItem(this, Lane::DLLT_Predecessor);
    // new LaneLinkItem(this, Lane::DLLT_Successor);

    // ContextMenu //
    //
    QAction *hideSectionAction = getHideMenu()->addAction(tr("Lane"));
    connect(hideSectionAction, SIGNAL(triggered()), this, SLOT(hideGraphElement()));

    QAction *hideParentRoadAction = getHideMenu()->addAction(tr("Road"));
    connect(hideParentRoadAction, SIGNAL(triggered()), this, SLOT(hideParentRoad()));

    QAction *removeLaneAction = getRemoveMenu()->addAction(tr("Lane"));
    connect(removeLaneAction, SIGNAL(triggered()), this, SLOT(removeLane()));

    QAction *removeParentRoadAction = getRemoveMenu()->addAction(tr("Road"));
    connect(removeParentRoadAction, SIGNAL(triggered()), this, SLOT(removeParentRoad()));

    // rebuildMoveRotateHandles(false);
}

//################//
// SLOTS          //
//################//

void
LaneItem::hideParentRoad()
{
    HideDataElementCommand *command = new HideDataElementCommand(grandparentRoad_, NULL);
    if (getProjectGraph())
    {
        getProjectGraph()->postponeGarbageDisposal();
        getProjectGraph()->executeCommand(command);
    }
    else
    {
        qDebug("LaneItem::removeRoad() not yet supported for profile graph!");
    }
}

bool
LaneItem::removeLane()
{
    parentLaneSectionItem_->removeLaneItem(this);

    RemoveLaneCommand *command = new RemoveLaneCommand(lane_->getParentLaneSection(), lane_, NULL);
    if (getProjectGraph())
    {
        if (getProjectGraph()->executeCommand(command))
        {
            return true;
        }
    }
    else
    {
        qDebug("LaneItem::removeLane() not yet supported for profile graph!");
    }

    return false;
}

void
LaneItem::removeParentRoad()
{
    RemoveRoadCommand *command = new RemoveRoadCommand(grandparentRoad_, NULL);
    if (getProjectGraph())
    {
        getProjectGraph()->executeCommand(command);
    }
    else
    {
        qDebug("LaneItem::removeRoad() not yet supported for profile graph!");
    }
}

//################//
// TOOLS          //
//################//

//################//
// GRAPHICS       //
//################//

/*! \brief Sets the color according to ...
*/
void
LaneItem::updateColor()
{
    Lane::LaneType type = lane_->getLaneType();
    if (type == Lane::LT_DRIVING)
    {
        setBrush(QBrush(ODD::instance()->colors()->brightGreen()));
        setPen(QPen(ODD::instance()->colors()->darkGreen()));
    }
    else if ((type == Lane::LT_MWYENTRY) || (type == Lane::LT_MWYEXIT))
    {
        setBrush(QBrush(ODD::instance()->colors()->brightCyan()));
        setPen(QPen(ODD::instance()->colors()->darkCyan()));
    }
    else if ((type == Lane::LT_SHOULDER) || (type == Lane::LT_PARKING))
    {
        setBrush(QBrush(ODD::instance()->colors()->brightOrange()));
        setPen(QPen(ODD::instance()->colors()->darkOrange()));
    }
    else if ((type == Lane::LT_BORDER) || (type == Lane::LT_RESTRICTED) || (type == Lane::LT_SIDEWALK) || (type == Lane::LT_STOP))
    {
        setBrush(QBrush(ODD::instance()->colors()->brightRed()));
        setPen(QPen(ODD::instance()->colors()->darkRed()));
    }
    else if ((type == Lane::LT_BIKING) || (type == Lane::LT_SPECIAL1) || (type == Lane::LT_SPECIAL2) || (type == Lane::LT_SPECIAL3))
    {
        setBrush(QBrush(ODD::instance()->colors()->brightBlue()));
        setPen(QPen(ODD::instance()->colors()->darkBlue()));
    }
    else if (type == Lane::LT_NONE)
    {
        setBrush(QBrush(ODD::instance()->colors()->brightGrey()));
        setPen(QPen(ODD::instance()->colors()->darkGrey()));
    }
    else
    {
        setBrush(QBrush(QColor(255, 0, 0)));
        qDebug("WARNING 1010181018! Unknown Lane Type!");
    }
}

void
LaneItem::createPath()
{

    // Initialization //
    //
    double sStart = parentLaneSection_->getSStart();
    double sEnd = parentLaneSection_->getSEnd();
    if (sEnd < sStart)
        sEnd = sStart;

    // double pointsPerMeter = 1.0; // BAD: hard coded!
    double pointsPerMeter = getProjectGraph()->getProjectWidget()->getLODSettings()->TopViewEditorPointsPerMeter;
    int pointCount = int(ceil((sEnd - sStart) * pointsPerMeter)); // TODO curvature...
    if (pointCount < 2)
    {
        pointCount = 2;
    }

    QVector<QPointF> points(2 * pointCount + 1);
    double segmentLength = (sEnd - sStart) / (pointCount - 1);

    int laneSide = 1;
    if (lane_->getId() < 0)
    {
        laneSide = -1;
    }

    // Right side //
    //
    if (lane_->isWidthActive())
    {
        for (int i = 0; i < pointCount; ++i)
        {
            double s = sStart + i * segmentLength; // [sStart, sEnd]
            points[i] = grandparentRoad_->getGlobalPoint(s, laneSide * parentLaneSection_->getLaneSpanWidth(0, lane_->getId() - laneSide, s) + grandparentRoad_->getLaneOffset(s));
        }

        // Left side //
        //
        for (int i = 0; i < pointCount; ++i)
        {
            double s = sEnd - i * segmentLength; // [sEnd, sStart]
            if (s < 0.0)
                s = 0.0; // can happen due to numerical inaccuracy (around -1.0e-15)
            points[i + pointCount] = grandparentRoad_->getGlobalPoint(s, laneSide * parentLaneSection_->getLaneSpanWidth(0, lane_->getId(), s) + grandparentRoad_->getLaneOffset(s));
        }

        // End point //
        //
        points[2 * pointCount] = grandparentRoad_->getGlobalPoint(sStart, laneSide * parentLaneSection_->getLaneSpanWidth(0, lane_->getId() - laneSide, sStart) + grandparentRoad_->getLaneOffset(sStart));
    }
    else
    {
        for (int i = 0; i < pointCount; ++i)
        {
            double s = sStart + i * segmentLength; // [sStart, sEnd]
            points[i] = grandparentRoad_->getGlobalPoint(s, laneSide * parentLaneSection_->getLaneWidth(lane_->getId() - laneSide, s));
        }

        // Left side //
        //
        for (int i = 0; i < pointCount; ++i)
        {
            double s = sEnd - i * segmentLength; // [sEnd, sStart]
            if (s < 0.0)
                s = 0.0; // can happen due to numerical inaccuracy (around -1.0e-15)
            points[i + pointCount] = grandparentRoad_->getGlobalPoint(s, laneSide * parentLaneSection_->getLaneWidth(lane_->getId(), s));
        }

        // End point //
        //
        points[2 * pointCount] = grandparentRoad_->getGlobalPoint(sStart, laneSide * parentLaneSection_->getLaneWidth(lane_->getId() - laneSide, sStart));
    }


    // Psycho-Path //
    //
    QPainterPath path;
    path.addPolygon(QPolygonF(points));

    setPath(path);
}

//################//
// OBSERVER       //
//################//

void
LaneItem::updateObserver()
{
    // Parent //
    //
    GraphElement::updateObserver();
    if (isInGarbage())
    {
        return; // will be deleted anyway
    }

    // Lane //
    //
    int changes = lane_->getLaneChanges();
    if ((changes & Lane::CLN_TypeChanged))
    {
        updateColor();
    }
    else if (changes & Lane::CLN_WidthsChanged)
    {
        createPath();
    }

    // Road //
    //
    int roadChanges = grandparentRoad_->getRoadChanges();
    if ((roadChanges & RSystemElementRoad::CRD_TrackSectionChange)
        || (roadChanges & RSystemElementRoad::CRD_LaneSectionChange)
        || (roadChanges & RSystemElementRoad::CRD_ShapeChange))
    {
        createPath();
    }

    // Data //
    //
    changes = lane_->getDataElementChanges();

    if (changes & DataElement::CDE_SelectionChange)
    {
        if (laneEditor_->getCurrentTool() == ODD::TLE_INSERT)
        {
            if (lane_->isElementSelected())
            {
                laneEditor_->registerLane(this, lane_);
            }
            else
            {
                laneEditor_->deregisterLane(this);
            }
        }
    }
}

//################//
// EVENTS         //
//################//

void
LaneItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (laneEditor_->getCurrentTool() == ODD::TLE_INSERT) 
    {
        if (laneEditor_->getCurrentParameterTool() != ODD::TPARAM_SELECT)
        {
            event->ignore();
            return;
        }

        if (!isSelected())
        {
            laneEditor_->registerLane(this, lane_);
        }
    }

    QGraphicsItem::mousePressEvent(event);
}

void
LaneItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    ODD::ToolId tool = laneEditor_->getCurrentTool();
    if ((tool == ODD::TLE_ADD_WIDTH) && (event->button() == Qt::LeftButton))
    {
        double s = grandparentRoad_->getSFromGlobalPoint(event->pos(), parentLaneSection_->getSStart(), parentLaneSection_->getSEnd()) - parentLaneSection_->getSStart();

        double startWidth = lane_->getWidth(s);
        //       double slope = lane_->getSlope(s);
        LaneWidth *laneWidth = lane_->getWidthEntryContains(s);
        LaneWidth *newLaneWidth = NULL;
        LaneBorder *newLaneBorder = NULL;
        if (laneWidth)
        {
            double x = s - laneWidth->getSSectionStart();
            double b = laneWidth->getB() + 2 * laneWidth->getC() * x + 3 * laneWidth->getD() * x * x;
            double c = laneWidth->getC() + 3 * laneWidth->getD() * x;
            newLaneWidth = new LaneWidth(s, startWidth, b, c, laneWidth->getD());
        }
        else
        {
            LaneBorder *laneBorder = lane_->getBorderEntryContains(s);
            double x = s - laneBorder->getSSectionStart();
            double b = laneBorder->getB() + 2 * laneBorder->getC() * x + 3 * laneBorder->getD() * x * x;
            double c = laneBorder->getC() + 3 * laneBorder->getD() * x;
            newLaneBorder = new LaneBorder(s, startWidth, b, c, laneBorder->getD());
        }

        InsertLaneWidthCommand *command = new InsertLaneWidthCommand(lane_, newLaneWidth, newLaneBorder);
        getProjectGraph()->executeCommand(command);
    }
    else if ((laneEditor_->getCurrentTool() == ODD::TLE_INSERT) && (laneEditor_->getCurrentParameterTool() != ODD::TPARAM_SELECT))
    {
        event->ignore();
        return;
    }

    parentLaneSectionItem_->mouseReleaseEvent(event);
}


void
LaneItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    ODD::ToolId tool = laneEditor_->getCurrentTool();
    if (tool == ODD::TLE_ADD_WIDTH)
    {
        setCursor(Qt::CrossCursor);

        double s = grandparentRoad_->getSFromGlobalPoint(event->pos(), parentLaneSection_->getSStart(), parentLaneSection_->getSEnd());
        double t = parentLaneSection_->getTValue(lane_, s, lane_->getWidth(s - parentLaneSection_->getSStart()));

        laneEditor_->getAddWidthHandle()->updatePos(parentLaneSectionItem_->getParentRoadItem(), grandparentRoad_->getGlobalPoint(s, t), parentLaneSection_->getSStart(), parentLaneSection_->getSEnd());
        laneEditor_->getAddWidthHandle()->show();
    }
    parentLaneSectionItem_->hoverMoveEvent(event);
}


//*************//
// Delete Item
//*************//

bool
LaneItem::deleteRequest()
{
    return removeLane();
    // return parentLaneSectionItem_->deleteRequest();
}
