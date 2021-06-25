/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCBYENTITY_H
#define OSCBYENTITY_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"
#include "oscTriggeringEntities.h"
#include "oscEntityCondition.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscByEntity : public oscObjectBase
{
public:
oscByEntity()
{
        OSC_OBJECT_ADD_MEMBER(TriggeringEntities, "oscTriggeringEntities", 0);
        OSC_OBJECT_ADD_MEMBER(EntityCondition, "oscEntityCondition", 0);
    };
        const char *getScope(){return "/OSCCondition";};
    oscTriggeringEntitiesMember TriggeringEntities;
    oscEntityConditionMember EntityCondition;

};

typedef oscObjectVariable<oscByEntity *> oscByEntityMember;
typedef oscObjectVariableArray<oscByEntity *> oscByEntityArrayMember;


}

#endif //OSCBYENTITY_H
