/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#include <config/coConfigLog.h>
#include <config/coConfigEntryString.h>

#include <QRegExp>

using std::ostream;
using namespace covise;

coConfigEntryString::coConfigEntryString(const QString &string, const coConfigConstants::ConfigScope scope,
                                         const QString &configName, bool isListItem)
    : QString(string)
{

    configScope = scope;
    this->configName = configName;
    listItem = isListItem;
}

coConfigEntryString::~coConfigEntryString()
{
}

coConfigConstants::ConfigScope coConfigEntryString::getConfigScope() const
{
    return configScope;
}

coConfigEntryString &coConfigEntryString::setConfigScope(coConfigConstants::ConfigScope scope)
{
    // if (scope == coConfigConstants::Default) {
    //   std::cerr << "coConfigEntryString::setConfigScope warn: Setting default scope on " << *this << std::endl;
    // }
    configScope = scope;
    return *this;
}

const QString &coConfigEntryString::getConfigName() const
{
    return configName;
}

coConfigEntryString &coConfigEntryString::setConfigName(const QString &name)
{
    configName = name;
    return *this;
}

const QString &coConfigEntryString::getConfigGroupName() const
{
    return configGroupName;
}

coConfigEntryString &coConfigEntryString::setConfigGroupName(const QString &name)
{
    configGroupName = name;
    return *this;
}

bool coConfigEntryString::isListItem() const
{
    return listItem;
}

void coConfigEntryString::setListItem(bool on)
{
    listItem = on;
}

coConfigEntryStringList::coConfigEntryStringList()
{
    listType = UNKNOWN;
    COCONFIGDBG("coConfigEntryStringList::<init> info: creating");
}

coConfigEntryStringList::~coConfigEntryStringList()
{
}

coConfigEntryStringList coConfigEntryStringList::merge(coConfigEntryStringList &list)
{

    if (list.size()>0)
    {
        std::list<coConfigEntryString>::iterator iterator = list.begin();

        while (iterator != list.end())
        {
            remove(*iterator);
            push_back(*iterator);
            iterator++;
        }

        this->listType = list.listType;
    }
    return *this;
}

coConfigEntryStringList coConfigEntryStringList::filter(const QRegExp &filter) const
{

    COCONFIGDBG("coConfigEntryStringList::filter info: filtering");

    coConfigEntryStringList list;

    for (coConfigEntryStringList::const_iterator iterator = begin();
         iterator != end(); ++iterator)
    {

        if (filter.exactMatch(*iterator))
        {
            list.push_back(*iterator);
        }
    }

    list.setListType(this->listType);

    return list;
}

coConfigEntryStringList::operator QStringList()
{

    COCONFIGDBG("coConfigEntryStringList::operator QStringList info: casting");

    QStringList list;

    for (coConfigEntryStringList::const_iterator iterator = begin();
         iterator != end(); ++iterator)
    {

        list.append(*iterator);
    }

    return list;
}

coConfigEntryStringList::ListType coConfigEntryStringList::getListType() const
{
    COCONFIGDBG("coConfigEntryStringList::getListType info: type is " << this->listType);
    return listType;
}

void coConfigEntryStringList::setListType(coConfigEntryStringList::ListType listType)
{
    this->listType = listType;
    COCONFIGDBG("coConfigEntryStringList::setListType info: new type is " << this->listType);
}

namespace covise
{

QTextStream &operator<<(QTextStream &out, const coConfigEntryStringList list)
{

    out << "[";

    coConfigEntryStringList::const_iterator iterator = list.begin();
    while (iterator != list.end())
    {

        out << *iterator;
        iterator++;
        if (iterator != list.end())
            out << ",";
    }

    out << "]";

    return out;
}
}
