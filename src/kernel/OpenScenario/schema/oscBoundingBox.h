/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCBOUNDINGBOX_H
#define OSCBOUNDINGBOX_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"
#include "oscCenter.h"
#include "oscDimension.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscBoundingBox : public oscObjectBase
{
public:
oscBoundingBox()
{
        OSC_OBJECT_ADD_MEMBER(Center, "oscCenter", 0);
        OSC_OBJECT_ADD_MEMBER(Dimension, "oscDimension", 0);
    };
        const char *getScope(){return "";};
    oscCenterMember Center;
    oscDimensionMember Dimension;

};

typedef oscObjectVariable<oscBoundingBox *> oscBoundingBoxMember;
typedef oscObjectVariableArray<oscBoundingBox *> oscBoundingBoxArrayMember;


}

#endif //OSCBOUNDINGBOX_H
