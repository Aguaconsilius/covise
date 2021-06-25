/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl - 2.1.txt.

* License: LGPL 2 + */

#include "oscLateralParams.h"

using namespace OpenScenario;
Enum_Lateral_purposeType::Enum_Lateral_purposeType()
{
addEnum("position", oscLateralParams::position);
addEnum("steering", oscLateralParams::steering);
}
Enum_Lateral_purposeType *Enum_Lateral_purposeType::instance()
{
	if (inst == NULL)
	{
		inst = new Enum_Lateral_purposeType();
	}
	return inst;
}
Enum_Lateral_purposeType *Enum_Lateral_purposeType::inst = NULL;
