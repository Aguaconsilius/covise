/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCENTITY_H
#define OSCENTITY_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscEntity : public oscObjectBase
{
public:
oscEntity()
{
        OSC_ADD_MEMBER(name, 0);
    };
        const char *getScope(){return "/OSCCondition/ByEntity/TriggeringEntities";};
    oscString name;

};

typedef oscObjectVariable<oscEntity *> oscEntityMember;
typedef oscObjectVariableArray<oscEntity *> oscEntityArrayMember;


}

#endif //OSCENTITY_H
