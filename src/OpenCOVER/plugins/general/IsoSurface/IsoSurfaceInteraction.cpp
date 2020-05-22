/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#include <util/common.h>

#include "IsoSurfaceInteraction.h"
#include "IsoSurfacePoint.h"
#include "IsoSurfacePlugin.h"

#include <cover/coInteractor.h>
#include <cover/coVRPluginSupport.h>

#include <cover/ui/Slider.h>
#include <cover/ui/Button.h>
#include <cover/ui/Menu.h>

using namespace opencover;

// match these with COVISE and Vistle IsoSurface modules
const char *IsoSurfaceInteraction::ISOVALUE = "isovalue";
const char *IsoSurfaceInteraction::ISOPOINT = "isopoint";
const char *IsoSurfaceInteraction::INTERACTOR = "point_or_value";

IsoSurfaceInteraction::IsoSurfaceInteraction(const RenderObject *container, coInteractor *inter, const char *pluginName, IsoSurfacePlugin *p)
    : ModuleInteraction(container, inter, pluginName)
{
    inter->getFloatSliderParam(ISOVALUE, minValue_, maxValue_, isoValue_);
    plugin = p;

    isoPoint_ = new IsoSurfacePoint(inter, p);

    createMenu();
    updateMenu();
}

IsoSurfaceInteraction::~IsoSurfaceInteraction()
{
    delete isoPoint_;
    deleteMenu();
}

// called when a new object has arrived:
// keep menu widgets updated
void
IsoSurfaceInteraction::update(const RenderObject *container, coInteractor *inter)
{
    ModuleFeedbackManager::update(container, inter);

    inter->getFloatSliderParam(ISOVALUE, minValue_, maxValue_, isoValue_);
    updateMenu();
    isoPoint_->update(inter);
}

void IsoSurfaceInteraction::updateInteractorVisibility()
{
    // if geometry is hidden, hide also interactor
    if (!hideCheckbox_->state() && showPickInteractorCheckbox_->state())
        isoPoint_->showPickInteractor();
    else
        isoPoint_->hidePickInteractor();

    if (!hideCheckbox_->state() && showDirectInteractorCheckbox_ && showDirectInteractorCheckbox_->state())
        isoPoint_->showDirectInteractor();
    else
        isoPoint_->hideDirectInteractor();
}

void
IsoSurfaceInteraction::preFrame()
{
    isoPoint_->preFrame();
}

void
IsoSurfaceInteraction::createMenu()
{
    // create the value poti
    inter_->getFloatSliderParam(ISOVALUE, minValue_, maxValue_, isoValue_);
    valueSlider_ = new ui::Slider(menu_, "IsoValue");
    valueSlider_->setBounds(minValue_, maxValue_);
    valueSlider_->setValue(isoValue_);
    valueSlider_->setText("Isovalue");
    valueSlider_->setCallback([this](double value, bool released){
        if (!released)
            return;
        inter_->getFloatSliderParam(ISOVALUE, minValue_, maxValue_, isoValue_);
        isoValue_ = value;
        plugin->getSyncInteractors(inter_);
        plugin->setSliderParam(ISOVALUE, minValue_, maxValue_, isoValue_);
        plugin->setChoiceParam(INTERACTOR, INTERACTOR_VALUE);
        plugin->executeModule();
    });
}

void
IsoSurfaceInteraction::deleteMenu()
{
    delete valueSlider_;
}

void
IsoSurfaceInteraction::updateMenu()
{
    // now set menu items accordingly
    valueSlider_->setBounds(minValue_, maxValue_);
    valueSlider_->setValue(isoValue_);
}

void
IsoSurfaceInteraction::updatePickInteractors(bool show)
{
    updateInteractorVisibility();
}

void
IsoSurfaceInteraction::updateDirectInteractors(bool show)
{
    updateInteractorVisibility();
}
