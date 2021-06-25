/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */


#ifndef OSCOPENSCENARIO_H
#define OSCOPENSCENARIO_H

#include "../oscExport.h"
#include "../oscObjectBase.h"
#include "../oscObjectVariable.h"
#include "../oscObjectVariableArray.h"

#include "../oscVariables.h"
#include "oscFileHeader.h"
#include "oscParameterDeclaration.h"
#include "oscCatalogs.h"
#include "oscRoadNetwork.h"
#include "oscEntities.h"
#include "oscStoryboard.h"

namespace OpenScenario
{
class OPENSCENARIOEXPORT oscOpenSCENARIO : public oscObjectBase
{
public:
oscOpenSCENARIO()
{
        OSC_OBJECT_ADD_MEMBER(FileHeader, "oscFileHeader", 0);
        OSC_OBJECT_ADD_MEMBER_OPTIONAL(ParameterDeclaration, "oscParameterDeclaration", 0);
        OSC_OBJECT_ADD_MEMBER(Catalogs, "oscCatalogs", 0);
        OSC_OBJECT_ADD_MEMBER(RoadNetwork, "oscRoadNetwork", 0);
        OSC_OBJECT_ADD_MEMBER(Entities, "oscEntities", 0);
        OSC_OBJECT_ADD_MEMBER(Storyboard, "oscStoryboard", 0);
    };
        const char *getScope(){return "";};
    oscFileHeaderMember FileHeader;
    oscParameterDeclarationMember ParameterDeclaration;
    oscCatalogsMember Catalogs;
    oscRoadNetworkMember RoadNetwork;
    oscEntitiesMember Entities;
    oscStoryboardMember Storyboard;

};

typedef oscObjectVariable<oscOpenSCENARIO *> oscOpenSCENARIOMember;
typedef oscObjectVariableArray<oscOpenSCENARIO *> oscOpenSCENARIOArrayMember;


}

#endif //OSCOPENSCENARIO_H
