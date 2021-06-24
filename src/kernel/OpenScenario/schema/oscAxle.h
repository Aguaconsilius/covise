/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCAXLE_H
#define OSCAXLE_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscAxle : public oscObjectBase
{
public:
oscAxle()
{
        OSC_ADD_MEMBER(maxSteering, 0);
        OSC_ADD_MEMBER(wheelDiameter, 0);
        OSC_ADD_MEMBER(trackWidth, 0);
        OSC_ADD_MEMBER(positionX, 0);
        OSC_ADD_MEMBER(positionZ, 0);
    };
        const char *getScope(){return "";};
    oscDouble maxSteering;
    oscDouble wheelDiameter;
    oscDouble trackWidth;
    oscDouble positionX;
    oscDouble positionZ;

};

typedef oscObjectVariable<oscAxle *> oscAxleMember;
typedef oscObjectVariableArray<oscAxle *> oscAxleArrayMember;


}

#endif //OSCAXLE_H
