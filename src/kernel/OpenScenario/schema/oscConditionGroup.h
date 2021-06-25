/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCCONDITIONGROUP_H
#define OSCCONDITIONGROUP_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"
#include "oscCondition.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscConditionGroup : public oscObjectBase
{
public:
oscConditionGroup()
{
        OSC_OBJECT_ADD_MEMBER(Condition, "oscCondition", 0);
    };
        const char *getScope(){return "";};
    oscConditionArrayMember Condition;

};

typedef oscObjectVariable<oscConditionGroup *> oscConditionGroupMember;
typedef oscObjectVariableArray<oscConditionGroup *> oscConditionGroupArrayMember;


}

#endif //OSCCONDITIONGROUP_H
