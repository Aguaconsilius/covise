/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCRELATIVEDISTANCE_H
#define OSCRELATIVEDISTANCE_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"
#include "oscTimeHeadway.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT Enum_RelativeDistance_typeType : public oscEnumType
{
public:
static Enum_RelativeDistance_typeType *instance();
    private:
		Enum_RelativeDistance_typeType();
	    static Enum_RelativeDistance_typeType *inst; 
};
class OPENSCENARIOEXPORT oscRelativeDistance : public oscObjectBase
{
public:
oscRelativeDistance()
{
        OSC_ADD_MEMBER(entity, 0);
        OSC_ADD_MEMBER(type, 0);
        OSC_ADD_MEMBER(value, 0);
        OSC_ADD_MEMBER(freespace, 0);
        OSC_ADD_MEMBER(rule, 0);
        type.enumType = Enum_RelativeDistance_typeType::instance();
        rule.enumType = Enum_ruleType::instance();
    };
        const char *getScope(){return "/OSCCondition/ByEntity/EntityCondition";};
    oscString entity;
    oscEnum type;
    oscDouble value;
    oscBool freespace;
    oscEnum rule;

    enum Enum_RelativeDistance_type
    {
longitudinal,
lateral,
inertial,

    };

};

typedef oscObjectVariable<oscRelativeDistance *> oscRelativeDistanceMember;
typedef oscObjectVariableArray<oscRelativeDistance *> oscRelativeDistanceArrayMember;


}

#endif //OSCRELATIVEDISTANCE_H
