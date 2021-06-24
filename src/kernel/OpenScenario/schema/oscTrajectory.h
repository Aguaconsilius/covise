/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCTRAJECTORY_H
#define OSCTRAJECTORY_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"
#include "oscParameterDeclaration.h"
#include "oscVertex.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT Enum_domain_time_distanceType : public oscEnumType
{
public:
static Enum_domain_time_distanceType *instance();
    private:
		Enum_domain_time_distanceType();
	    static Enum_domain_time_distanceType *inst; 
};
class OPENSCENARIOEXPORT oscTrajectory : public oscObjectBase
{
public:
oscTrajectory()
{
        OSC_ADD_MEMBER(name, 0);
        OSC_ADD_MEMBER(closed, 0);
        OSC_ADD_MEMBER(domain, 0);
        OSC_OBJECT_ADD_MEMBER_OPTIONAL(ParameterDeclaration, "oscParameterDeclaration", 0);
        OSC_OBJECT_ADD_MEMBER(Vertex, "oscVertex", 0);
        domain.enumType = Enum_domain_time_distanceType::instance();
    };
        const char *getScope(){return "";};
    oscString name;
    oscBool closed;
    oscEnum domain;
    oscParameterDeclarationMember ParameterDeclaration;
    oscVertexArrayMember Vertex;

    enum Enum_domain_time_distance
    {
time,
distance,

    };

};

typedef oscObjectVariable<oscTrajectory *> oscTrajectoryMember;
typedef oscObjectVariableArray<oscTrajectory *> oscTrajectoryArrayMember;


}

#endif //OSCTRAJECTORY_H
