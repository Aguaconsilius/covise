/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

 /**************************************************************************
 ** ODD: OpenDRIVE Designer
 **   Frank Naegele (c) 2010
 **   <mail@f-naegele.de>
 **   15.03.2010
 **
 **************************************************************************/

#include "typesection.hpp"

#include "src/data/roadsystem/rsystemelementroad.hpp"

SpeedRecord::SpeedRecord()
{
    maxSpeed = 0;
    maxUnit = "m/s";
}
SpeedRecord::SpeedRecord(QString &max, QString &unit)
{
    if (max == "no limit")
        maxSpeed = 0;
    if (max == "undefined")
        maxSpeed = -1;
    maxSpeed = max.toFloat();
    float factor = 1.0;
    if (unit == "m/s")
        factor = 1.0;
    if (unit == "km/h")
        factor = 0.277777778f;
    if (unit == "mph")
        factor = 0.44704f;
    maxSpeed *= factor;
    maxUnit = unit;
}

//####################//
// Constructors       //
//####################//

TypeSection::TypeSection(double s, TypeSection::RoadType type)
    : RoadSection(s)
    , typeSectionChanges_(0x0)
    , type_(type)
{
    speedRecord = NULL;
}
TypeSection::~TypeSection()
{
    delete speedRecord;
}

//####################//
// TypeSection        //
//####################//

void TypeSection::setSpeedRecord(SpeedRecord *sr)
{
    speedRecord = sr;
}
SpeedRecord *TypeSection::getSpeedRecord()
{
    return speedRecord;
}

/*! \brief Sets the RoadType of this section.
*
*/
void
TypeSection::setRoadType(TypeSection::RoadType roadType)
{
    if (type_ != roadType)
    {
        type_ = roadType;
        addTypeSectionChanges(TypeSection::CTS_TypeChange);
    }
}

//####################//
// RoadSection        //
//####################//

/*! \brief Returns the end coordinate of this section.
*
* In road coordinates [m].
*/
double
TypeSection::getSEnd() const
{
    return getParentRoad()->getTypeSectionEnd(getSStart());
}

/*! \brief Returns the length coordinate of this section.
*
* In [m].
*/
double
TypeSection::getLength() const
{
    return getParentRoad()->getTypeSectionEnd(getSStart()) - getSStart();
}

//##################//
// Observer Pattern //
//##################//

/*! \brief Called after all Observers have been notified.
*
* Resets the change flags to 0x0.
*/
void
TypeSection::notificationDone()
{
    typeSectionChanges_ = 0x0;
    RoadSection::notificationDone();
}

/*! \brief Add one or more change flags.
*
*/
void
TypeSection::addTypeSectionChanges(int changes)
{
    if (changes)
    {
        typeSectionChanges_ |= changes;
        notifyObservers();
    }
}

//###################//
// Prototype Pattern //
//###################//

/*! \brief Creates and returns a deep copy clone of this object.
*
*/
TypeSection *
TypeSection::getClone()
{
    return new TypeSection(getSStart(), type_);
}

//###################//
// Visitor Pattern   //
//###################//

/*!
* Accepts a visitor for this section.
*
* \param visitor The visitor that will be visited.
*/
void
TypeSection::accept(Visitor *visitor)
{
    visitor->visit(this);
}

//###################//
// Static Functions  //
//###################//

TypeSection::RoadType
TypeSection::parseRoadType(const QString &type)
{
    if (type == "unknown")
        return TypeSection::RTP_UNKNOWN;
    else if (type == "rural")
        return TypeSection::RTP_RURAL;
    else if (type == "motorway")
        return TypeSection::RTP_MOTORWAY;
    else if (type == "town")
        return TypeSection::RTP_TOWN;
    else if (type == "lowSpeed")
        return TypeSection::RTP_LOWSPEED;
    else if (type == "pedestrian")
        return TypeSection::RTP_PEDESTRIAN;
    else
    {
        qDebug("WARNING 1003151045! Unknown road type: %s", type.toUtf8().constData());
        return TypeSection::RTP_UNKNOWN;
    }
};

QString
TypeSection::parseRoadTypeBack(TypeSection::RoadType type)
{
    if (type == TypeSection::RTP_UNKNOWN)
        return QString("unknown");
    else if (type == TypeSection::RTP_RURAL)
        return QString("rural");
    else if (type == TypeSection::RTP_MOTORWAY)
        return QString("motorway");
    else if (type == TypeSection::RTP_TOWN)
        return QString("town");
    else if (type == TypeSection::RTP_LOWSPEED)
        return QString("lowSpeed");
    else if (type == TypeSection::RTP_PEDESTRIAN)
        return QString("pedestrian");
    else
    {
        qDebug("WARNING 1006211728: unknown road type.");
        return QString("unknown");
    }
};
