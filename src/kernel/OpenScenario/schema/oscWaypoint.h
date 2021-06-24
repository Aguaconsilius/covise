/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCWAYPOINT_H
#define OSCWAYPOINT_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"

namespace OpenScenario
{
   class oscPosition;
class OPENSCENARIOEXPORT Enum_Route_strategyType : public oscEnumType
{
public:
static Enum_Route_strategyType *instance();
    private:
		Enum_Route_strategyType();
	    static Enum_Route_strategyType *inst; 
};
class OPENSCENARIOEXPORT oscWaypoint : public oscObjectBase
{
public:
oscWaypoint();
        const char *getScope(){return "/OSCRoute";};
    oscEnum strategy;
    oscObjectVariable<oscPosition *> Position;

    enum Enum_Route_strategy
    {
fastest,
shortest,
leastIntersections,
random,

    };

};

typedef oscObjectVariable<oscWaypoint *> oscWaypointMember;
typedef oscObjectVariableArray<oscWaypoint *> oscWaypointArrayMember;


}

#endif //OSCWAYPOINT_H
