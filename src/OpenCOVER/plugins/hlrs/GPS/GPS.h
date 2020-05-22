/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#ifndef _GPS_PLUGIN_H
#define _GPS_PLUGIN_H
/****************************************************************************\ 
 **                                                            (C)2008 HLRS  **
 **                                                                          **
 ** Description: GPS OpenCOVER Plugin (is polite)                            **
 **                                                                          **
 **                                                                          **
 ** Author: U.Woessner, T.Gudat		                                     **
 **                                                                          **
 ** History:  								     **
 ** June 2008  v1	    				       		     **
 **                                                                          **
 **                                                                          **
\****************************************************************************/
#include <cover/coVRPlugin.h>
#include <cover/ui/Menu.h>
#include <cover/ui/Button.h>
#include <cover/ui/ButtonGroup.h>
#include <cover/ui/Action.h>
#include <cover/ui/Slider.h>
#include <cover/ui/SelectionList.h>
#include <cover/ui/Label.h>
#include <cover/ui/EditField.h>
#include <cover/ui/Owner.h>
#include <proj_api.h>
#include <gdal_priv.h>
#include <xercesc/dom/DOM.hpp>
#include <vrml97/vrml/Player.h>



#include "File.h"
#include "PointSensor.h"
#include "GPSPoint.h"


using namespace opencover;
using namespace covise;
namespace opencover
{
class coVRLabel;

}
class GPSPoint;
class Track;



class GPSPlugin : public opencover::coVRPlugin, public ui::Owner
{
public:
    GPSPlugin();
    ~GPSPlugin();
    static GPSPlugin *instance(){return plugin;};

    float getAlt(double x, double y);
    void addFile(File *f);
    bool checkBounds (double x, double y);

    static int SloadGPX(const char *filename, osg::Group *parent, const char *);
    static int SunloadGPX(const char *filename, const char *);

    ui::Menu *GPSTab = nullptr;
    ui::Label *infoLabel = nullptr;
    ui::Button *TracksOn = nullptr;
    ui::Action *PointsOff = nullptr;
    ui::Action *ToggleLOD = nullptr;
    ui::Action *ShowSigns = nullptr;
    ui::Button *ToggleGood = nullptr;
    ui::Button *ToggleMedium = nullptr;
    ui::Button *ToggleBad = nullptr;
    ui::Button *ToggleAngst = nullptr;
    ui::Button *ToggleText = nullptr;
    ui::Button *ToggleFoto = nullptr;
    ui::Button *ToggleSprachaufnahme = nullptr;
    ui::Button *ToggleBarriere = nullptr;
    ui::Slider *TrackSizeSlider=nullptr;
    ui::Slider *PointSizeSlider=nullptr;

    osg::ref_ptr<osg::Group> OSGGPSPlugin;

    osg::ref_ptr<osg::Image> iconGood;
    osg::ref_ptr<osg::Image> iconMedium;
    osg::ref_ptr<osg::Image> iconBad;
    osg::ref_ptr<osg::Image> iconAngst;
    osg::ref_ptr<osg::Image> iconText;
    osg::ref_ptr<osg::Image> iconFoto;
    osg::ref_ptr<osg::Image> iconSprachaufnahme;
    osg::ref_ptr<osg::Image> iconBarriere;
    osg::ref_ptr<osg::Image> textbackground;

    coVRLabel *Label;
    float zOffset=1.0;


    std::string dir;//Coordinates
    projPJ pj_from, pj_to;//Coordinates
    static vrml::Player *player;


    
private:
    static GPSPlugin *plugin;
    bool update();
    void closeImage();
    void toggleDetail(GPSPoint::pointType type, ui::Button *button);
    void openImage(std::string &name);
    int loadGPX(const char *filename, osg::Group *parent);
    int unloadGPX(const char *filename);
    void GPSTab_create();
    void GPSTab_delete();
    void toggleButtonStatus(bool status);
    std::list<File*> fileList;
    std::list<ui::Button *> ButtonList;
    bool detailView = true;
    bool showPoints = true;

    float *rasterData=NULL;
    double xOrigin; // origin of the height map
    double yOrigin;
    double pixelWidth; // size of each pixel
    double pixelHeight;
    int cols; // number of pixel rows and columns
    int rows;
    GDALDataset  *heightDataset;
    GDALRasterBand  *heightBand;


};

#endif
