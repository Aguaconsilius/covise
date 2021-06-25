/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCSTORY_H
#define OSCSTORY_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"
#include "oscAct.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscStory : public oscObjectBase
{
public:
oscStory()
{
        OSC_ADD_MEMBER_OPTIONAL(owner, 0);
        OSC_ADD_MEMBER(name, 0);
        OSC_OBJECT_ADD_MEMBER(Act, "oscAct", 0);
    };
        const char *getScope(){return "/OpenSCENARIO/Storyboard";};
    oscString owner;
    oscString name;
    oscActArrayMember Act;

};

typedef oscObjectVariable<oscStory *> oscStoryMember;
typedef oscObjectVariableArray<oscStory *> oscStoryArrayMember;


}

#endif //OSCSTORY_H
