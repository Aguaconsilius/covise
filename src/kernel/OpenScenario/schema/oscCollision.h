/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCCOLLISION_H
#define OSCCOLLISION_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"
#include "oscCollisionByEntity.h"
#include "oscByType.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscCollision : public oscObjectBase
{
public:
oscCollision()
{
        OSC_OBJECT_ADD_MEMBER(ByEntity, "oscCollisionByEntity", 1);
        OSC_OBJECT_ADD_MEMBER(ByType , "oscByType", 1);
    };
        const char *getScope(){return "/OSCCondition/ByEntity/EntityCondition";};
    oscCollisionByEntityMember ByEntity;
    oscByTypeMember ByType ;

};

typedef oscObjectVariable<oscCollision *> oscCollisionMember;
typedef oscObjectVariableArray<oscCollision *> oscCollisionArrayMember;


}

#endif //OSCCOLLISION_H
