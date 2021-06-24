/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCCENTER_H
#define OSCCENTER_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscCenter : public oscObjectBase
{
public:
oscCenter()
{
        OSC_ADD_MEMBER(x, 0);
        OSC_ADD_MEMBER(y, 0);
        OSC_ADD_MEMBER(z, 0);
    };
        const char *getScope(){return "/OSCBoundingBox";};
    oscDouble x;
    oscDouble y;
    oscDouble z;

};

typedef oscObjectVariable<oscCenter *> oscCenterMember;
typedef oscObjectVariableArray<oscCenter *> oscCenterArrayMember;


}

#endif //OSCCENTER_H
