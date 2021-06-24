/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

 /**************************************************************************
 ** ODD: OpenDRIVE Designer
 **   Frank Naegele (c) 2010
 **   <mail@f-naegele.de>
 **   23.02.2010
 **
 **************************************************************************/

#include "laneborder.hpp"

#include "lane.hpp"

LaneBorder::LaneBorder(double sOffset, double a, double b, double c, double d)
    : LaneWidth(sOffset, a, b, c, d)
{
}

LaneBorder::~LaneBorder()
{
}

/*! \brief Creates and returns a deep copy clone of this object.
*
*/
LaneBorder *
LaneBorder::getClone()
{
    LaneBorder *clone = new LaneBorder(getSOffset(), a_, b_, c_, d_);

    return clone;
}

/** Returns the end coordinate of this lane road mark.
* In lane section coordinates [m].
*/
double
LaneBorder::getSSectionEnd() const
{
    return getParentLane()->getBorderEnd(getSOffset());
}

/** Returns the length coordinate of this lane section.
* In [m].
*/
double
LaneBorder::getLength() const
{
    return getParentLane()->getBorderEnd(getSOffset()) - getSSectionStartAbs();
}


double
LaneBorder::getT(double s)
{
    return f(s - getSOffset());
}


//################//
// OBSERVER       //
//################//

/*! \brief Called after all Observers have been notified.
*
* Resets the change flags to 0x0.
*/
void
LaneBorder::notificationDone()
{
    laneBorderChanges_ = 0x0;
    DataElement::notificationDone();
}



//###################//
// Visitor Pattern   //
//###################//

/*! Accepts a visitor for this element.
*/
void
LaneBorder::accept(Visitor *visitor)
{
    visitor->visit(this);
}
