/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCREACHPOSITION_H
#define OSCREACHPOSITION_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"
#include "oscPosition.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscReachPosition : public oscObjectBase
{
public:
oscReachPosition()
{
        OSC_ADD_MEMBER(tolerance, 0);
        OSC_OBJECT_ADD_MEMBER(Position, "oscPosition", 0);
    };
        const char *getScope(){return "/OSCCondition/ByEntity/EntityCondition";};
    oscDouble tolerance;
    oscPositionMember Position;

};

typedef oscObjectVariable<oscReachPosition *> oscReachPositionMember;
typedef oscObjectVariableArray<oscReachPosition *> oscReachPositionArrayMember;


}

#endif //OSCREACHPOSITION_H
