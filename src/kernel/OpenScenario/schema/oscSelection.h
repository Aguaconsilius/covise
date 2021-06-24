/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCSELECTION_H
#define OSCSELECTION_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"
#include "oscMembers.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscSelection : public oscObjectBase
{
public:
oscSelection()
{
        OSC_ADD_MEMBER(name, 0);
        OSC_OBJECT_ADD_MEMBER(Members, "oscMembers", 0);
    };
        const char *getScope(){return "/OpenSCENARIO/Entities";};
    oscString name;
    oscMembersMember Members;

};

typedef oscObjectVariable<oscSelection *> oscSelectionMember;
typedef oscObjectVariableArray<oscSelection *> oscSelectionArrayMember;


}

#endif //OSCSELECTION_H
