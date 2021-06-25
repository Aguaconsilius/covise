/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCTIMING_H
#define OSCTIMING_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT Enum_domain_absolute_relativeType : public oscEnumType
{
public:
static Enum_domain_absolute_relativeType *instance();
    private:
		Enum_domain_absolute_relativeType();
	    static Enum_domain_absolute_relativeType *inst; 
};
class OPENSCENARIOEXPORT oscTiming : public oscObjectBase
{
public:
oscTiming()
{
        OSC_ADD_MEMBER(domain, 0);
        OSC_ADD_MEMBER(scale, 0);
        OSC_ADD_MEMBER(offset, 0);
        domain.enumType = Enum_domain_absolute_relativeType::instance();
    };
        const char *getScope(){return "/OSCPrivateAction/Routing/FollowTrajectory/LongitudinalParams";};
    oscEnum domain;
    oscDouble scale;
    oscDouble offset;

    enum Enum_domain_absolute_relative
    {
absolute,
relative,

    };

};

typedef oscObjectVariable<oscTiming *> oscTimingMember;
typedef oscObjectVariableArray<oscTiming *> oscTimingArrayMember;


}

#endif //OSCTIMING_H
