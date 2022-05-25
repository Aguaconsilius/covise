/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

 /**************************************************************************
 ** ODD: OpenDRIVE Designer
 **   Frank Naegele (c) 2010
 **   <mail@f-naegele.de>
 **   12.03.2010
 **
 **************************************************************************/

#ifndef SIGNALITEM_HPP
#define SIGNALITEM_HPP

#include "src/graph/items/roadsystem/roadsystemitem.hpp"

class SignalObject;
class RoadSystemItem;
class SignalTextItem;
class SignalEditor;
class SignalManager;
class ToolAction;
class LODSettings;

class QColor;

class SignalItem : public GraphElement
{
    Q_OBJECT

        //################//
        // FUNCTIONS      //
        //################//

public:
    explicit SignalItem(RoadSystemItem *roadSystemItem, Signal *signal, QPointF pos);
    virtual ~SignalItem();

    // Garbage //
    //
    virtual bool deleteRequest();

    Signal *getSignal() const
    {
        return signal_;
    }

    // Graphics //
    //
    void updateColor();
    virtual void createPath();
    void updatePosition();

    void move(QPointF &diff);
    void duplicate();

    void updateCategory();

    // Text //
    //
    SignalTextItem *getSignalTextItem() const
    {
        return signalTextItem_;
    }

    RSystemElementRoad *getClosestRoad() const
    {
        return closestRoad_;
    }

    // Garbage //
    //
    // virtual void   notifyDeletion();

    // Obsever Pattern //
    //
    virtual void updateObserver();

    //################//
    // SIGNALS        //
    //################//

signals:
    void toolAction(ToolAction *);  // send action to copy the selected item //

    //################//
    // SLOTS          //
    //################//

public slots:

    //Tools
    //
    void zoomAction();

    bool removeSignal();

    //################//
    // EVENTS         //
    //################//

public:
    virtual QVariant  itemChange(GraphicsItemChange change, const QVariant & value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    //################//
    // PROPERTIES     //
    //################//

private:
    RoadSystemItem *roadSystemItem_;
    void init();

    Signal *signal_;
    RSystemElementRoad *road_;
    RSystemElementRoad *closestRoad_;

    QPointF pos_;
    double size_;
    double halfsize_;
    double x_;
    double y_;
    double width_;
    double height_;
    double scale_;

    QPointF pressPos_;
    QPointF lastPos_;
    bool doPan_;
    bool copyPan_;

    SignalTextItem *signalTextItem_;

    QGraphicsPixmapItem *pixmapItem_;
    QPixmap pixmap_;

    bool showPixmap_;

    QColor outerColor_;

    SignalEditor *signalEditor_;

    SignalManager *signalManager_;

    LODSettings *lodSettings_;

    int categorySize_;
};

#endif // ROADITEM_HPP
