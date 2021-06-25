/* This file is part of COVISE.

You can use it under the terms of the GNU Lesser General Public License
version 2.1 or later, see lgpl-2.1.txt.

* License: LGPL 2+ */

#include "oscArrayMember.h"
#include "oscObjectBase.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/util/XMLString.hpp>


using namespace OpenScenario;


oscArrayMember::oscArrayMember() :
        oscMember(),
        std::vector<oscObjectBase *>()
{

}

oscArrayMember::~oscArrayMember()
{

}



int
oscArrayMember::findObjectIndex(oscObjectBase *object)
{
	int i = 0;
	for (auto it = cbegin(); it != cend(); ++it)
	{
		if (*it == object)
		{
			return i;
		}
		i++;
	}

	return -1;
}

unsigned char
oscArrayMember::validate()
{
	if (size() < 1)
	{
		if (optional)
		{
			return oscObjectBase::VAL_optional;
		}

		return oscObjectBase::VAL_invalid;
	}

	for(oscArrayMember::iterator it =begin();it != end();it++)
	{
		oscObjectBase *obj = *it;
		if (!obj->validate())
		{
			return oscObjectBase::VAL_invalid;
		}
	}

	return oscObjectBase::VAL_valid;
}
