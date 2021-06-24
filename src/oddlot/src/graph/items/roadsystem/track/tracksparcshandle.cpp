/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

 /**************************************************************************
 ** ODD: OpenDRIVE Designer
 **   Frank Naegele (c) 2010
 **   <mail@f-naegele.de>
 **   10/26/2010
 **
 **************************************************************************/

#include "tracksparcshandle.hpp"

 // Data //
 //
#include "src/data/roadsystem/track/trackspiralarcspiral.hpp"
#include "src/data/commands/trackcommands.hpp"

// Graph //
//
#include "src/graph/projectgraph.hpp"
#include "tracksparcsitem.hpp"
#include "src/graph/items/handles/slidermovehandle.hpp"

// Qt //
//
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

//################//
// CONSTRUCTOR    //
//################//

TrackSparcsHandle::TrackSparcsHandle(TrackSpArcSItem *parentSpArcSItem)
    : SliderHandle(parentSpArcSItem)
    , parentSpArcSItem_(parentSpArcSItem)
    , min_(0.0)
    , max_(30.0)
{
    setBrush(QBrush(ODD::instance()->colors()->brightGreen()));
    setPen(QPen(ODD::instance()->colors()->darkGreen()));

    connect(getMoveHandle(), SIGNAL(requestPositionChange(QPointF)), this, SLOT(moveHandlePositionChange(QPointF)));
}

TrackSparcsHandle::~TrackSparcsHandle()
{
}

void
TrackSparcsHandle::setFactor(double factor)
{
    if (factor > 1.0)
    {
        factor = 1.0;
    }
    else if (factor < 0.0)
    {
        factor = 0.0;
    }
    getMoveHandle()->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    getMoveHandle()->setPos((max_ - min_) * factor, 0.0);
    getMoveHandle()->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//################//
// EVENTS         //
//################//

void
TrackSparcsHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    Handle::mousePressEvent(event); // pass to baseclass
}

void
TrackSparcsHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    Handle::mouseReleaseEvent(event); // pass to baseclass
}

void
TrackSparcsHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Handle::mouseMoveEvent(event); // pass to baseclass
}

void
TrackSparcsHandle::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    Handle::hoverEnterEvent(event);
}

void
TrackSparcsHandle::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
    Handle::hoverLeaveEvent(event);
}

void
TrackSparcsHandle::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Handle::hoverMoveEvent(event);
}

//################//
// SLOTS          //
//################//

void
TrackSparcsHandle::moveHandlePositionChange(const QPointF &pos)
{
    // qDebug() << pos.x() << " " << pos.y();
    // qDebug() << "factor: " << pos.x()/(max_-min_);
    // getMoveHandle()->setPos(pos.x(), 0.0);
    SetSpArcSFactorCommand *command = new SetSpArcSFactorCommand(parentSpArcSItem_->getSpArcS(), parentSpArcSItem_->getSpArcS()->getFactor() + pos.x() / (max_ - min_));
    parentSpArcSItem_->getProjectGraph()->executeCommand(command);
}
