/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

 /**************************************************************************
 ** ODD: OpenDRIVE Designer
 **   Frank Naegele (c) 2010
 **   <mail@f-naegele.de>
 **   11/2/2010
 **
 **************************************************************************/

#include "oscobjectsettingsstack.hpp"

#include "src/mainwindow.hpp"

 // Data //
 //
#include "src/data/oscsystem/oscelement.hpp"

// Settings //
//
#include "src/settings/widgets/oscobjectsettings.hpp"


// Qt //
//
#include <QStackedWidget>
#include <QVBoxLayout>

//################//
// CONSTRUCTOR    //
//################//

OSCObjectSettingsStack::OSCObjectSettingsStack(ProjectSettings *projectSettings, SettingsElement *parentSettingsElement, OSCElement *element)
    : SettingsElement(projectSettings, parentSettingsElement, element)
    , init_(false)
    , projectSettings_(projectSettings)
    , element_(element)
{
    uiInit();

    init_ = true;

}

OSCObjectSettingsStack::~OSCObjectSettingsStack()
{
    for (int i = 0; i < stack_->count(); i++)
    {
        QWidget *widget = stack_->widget(i);
        stack_->removeWidget(widget);
        delete widget;
    }

}

//################//
// FUNCTIONS      //
//################//

// Create generic interface //
//
void
OSCObjectSettingsStack::uiInit()
{
    // Widget/Layout //
    //
    objectBoxLayout_ = new QVBoxLayout();

    stack_ = new QStackedWidget();

    OSCObjectSettings *oscSettings = new OSCObjectSettings(projectSettings_, this, element_, NULL, NULL);


}

int
OSCObjectSettingsStack::getStackSize()
{
    return stack_->count();
}

void
OSCObjectSettingsStack::addWidget(QWidget *widget)
{
    if (widget)
    {
        stack_->addWidget(widget);
        stack_->setCurrentWidget(widget);
    }

    objectBoxLayout_->addWidget(stack_);
    setLayout(objectBoxLayout_);
    widget->setFocus();
}

QWidget *
OSCObjectSettingsStack::getLastWidget()
{
    /* int stackSize = stack_->count();
        return stack_->widget(stackSize - 1); */

    return stack_->currentWidget();
}

void
OSCObjectSettingsStack::removeWidget(QWidget *widget)
{
    stack_->removeWidget(widget);
}


//################//
// SLOTS          //
//################//
void
OSCObjectSettingsStack::stackRemoveWidget()
{
    int i = stack_->currentIndex();
    QWidget *widget = stack_->widget(i - 1);
    if (widget)
    {
        stack_->setCurrentWidget(widget);
        static_cast<OSCObjectSettings *>(widget)->updateProperties();
    }
    stack_->removeWidget(stack_->widget(i));
}


//##################//
// Observer Pattern //
//##################//

void
OSCObjectSettingsStack::updateObserver()
{

    // Parent //
    //
    SettingsElement::updateObserver();
    if (isInGarbage())
    {
        return; // no need to go on
    }


    // oscObject //
    //
/*    int changes = object_->getoscObjectChanges();

    if ((changes & Bridge::CEL_ParameterChange))
    {
        updateProperties();
    }*/
}
