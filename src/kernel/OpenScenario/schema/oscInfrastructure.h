/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCINFRASTRUCTURE_H
#define OSCINFRASTRUCTURE_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"
#include "oscInfrastructureSignal.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscInfrastructure : public oscObjectBase
{
public:
oscInfrastructure()
{
        OSC_OBJECT_ADD_MEMBER(Signal, "oscInfrastructureSignal", 0);
    };
        const char *getScope(){return "/OSCGlobalAction";};
    oscInfrastructureSignalMember Signal;

};

typedef oscObjectVariable<oscInfrastructure *> oscInfrastructureMember;
typedef oscObjectVariableArray<oscInfrastructure *> oscInfrastructureArrayMember;


}

#endif //OSCINFRASTRUCTURE_H
