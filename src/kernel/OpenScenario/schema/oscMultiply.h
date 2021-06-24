/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCMULTIPLY_H
#define OSCMULTIPLY_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscMultiply : public oscObjectBase
{
public:
oscMultiply()
{
        OSC_ADD_MEMBER(value, 0);
    };
        const char *getScope(){return "/OSCGlobalAction/ActionParameter/Modify/Rule";};
    oscDouble value;

};

typedef oscObjectVariable<oscMultiply *> oscMultiplyMember;
typedef oscObjectVariableArray<oscMultiply *> oscMultiplyArrayMember;


}

#endif //OSCMULTIPLY_H
