/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */


#include "FlightGear.h"
#include "UDPComm.h"
#include <config/CoviseConfig.h>
#include "Bicycle.h"
#include <util/byteswap.h>
#include <util/unixcompat.h>

static float zeroAngle = 1152.;


FlightGear::FlightGear(BicyclePlugin* bicycle)
    : udp(NULL)
    , bicycle(bicycle)
{
    thermal = 1;
    init();

    for (unsigned i = 0; i < 3; ++i)
    {
        fgdata.position[i] = 0.0;
        fgdata.orientation[i] = 0.0;
    }
}
FlightGear::~FlightGear()
{
    delete udp;
}

void FlightGear::init()
{
    delete udp;

    const std::string host = covise::coCoviseConfig::getEntry("value", "FlightGear.serverHost", "141.58.8.212");
    unsigned short serverPort = covise::coCoviseConfig::getInt("FlightGear.serverPort", 5253);
    unsigned short localPort = covise::coCoviseConfig::getInt("FlightGear.localPort", 5252);
    std::cerr << "FlightGear config: UDP: serverHost: " << host << ", localPort: " << localPort << ", serverPort: " << serverPort << std::endl;
    udp = new UDPComm(host.c_str(), serverPort, localPort);

    fgcontrol.pause = 1.0;
    fgcontrol.up = 0;
    upVal = 0;
    return;
}

void
FlightGear::run()
{
    running = true;
    doStop = false;
    while (running && !doStop)
    {
        usleep(5000);
        this->update();

    }
}

void
FlightGear::stop()
{
    doStop = true;
}

void FlightGear::doUp() {
    upVal++;
    fgcontrol.up = upVal;
    byteSwap(fgcontrol.up);
};

void FlightGear::doPause(double s) {
    fgcontrol.pause = s;
    byteSwap(fgcontrol.pause);
};
double FlightGear::getPause() { 
    double tmp = fgcontrol.pause;
    byteSwap(tmp);
    return tmp; 
};


void FlightGear::update()
{
    if (udp)
    {
        char tmpBuf[10000];
        int status = udp->receive(&tmpBuf, 10000);


        if (status > 0 && status > sizeof(FGData))
        {

            {
                OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mutex);
                memcpy(&fgdata, tmpBuf, sizeof(FGData));
            }
            for (unsigned i = 0; i < 3; ++i)
            {
                byteSwap(fgdata.position[i]);
                byteSwap(fgdata.orientation[i]);
            }


            fprintf(stderr, "\r");
            /*            fprintf(stderr, "Model: %s ", fgdata.Model);
                    for (unsigned i = 0; i < 3; ++i)
                        fprintf(stderr, "Pos: %6f ", fgdata.position[i]);*/
        }
        else if (status == -1)
        {
            std::cerr << "FlightGear::update: error while reading data" << std::endl;
            //init();
            return;
        }
        else
        {
            std::cerr << "FlightGear::update: received invalid no. of bytes: recv=" << status << ", got=" << status << std::endl;
            //init();
            return;
        }
        if (BicyclePlugin::plugin->isParaglider)
        {
            if (thermal) // ParagliderThrottleInput.xml
            {
                fgcontrol.magnetos = 0;
                fgcontrol.starter = true; // /sim/model/MRX13/engine_running
                fgcontrol.throttle = 1.0; // /fdm/jsbsim/fcs/throttle-generic-engine-norm
                fgcontrol.parkingBrake = BicyclePlugin::plugin->wingArea->getValue(); // Wingarea
            }
            else
            {
                fgcontrol.magnetos = 0;
                fgcontrol.starter = false; // /sim/model/MRX13/engine_running
                fgcontrol.throttle = 0.0; // /fdm/jsbsim/fcs/throttle-generic-engine-norm
                fgcontrol.parkingBrake = BicyclePlugin::plugin->wingArea->getValue(); // Wingarea
            }
        }
        else
        {
            if (bicycle->power > 5)
            {
                fgcontrol.magnetos = 1;
                fgcontrol.starter = true;
                fgcontrol.throttle = bicycle->power / 10000;
                fgcontrol.parkingBrake = 0.0;
            }
            else
            {
                fgcontrol.magnetos = 1;
                fgcontrol.starter = false;
                fgcontrol.throttle = 0.0;
                fgcontrol.parkingBrake = 1.0;
            }
        }
        fprintf(stderr, "Sent throttle data0: %6f ", fgcontrol.throttle);
        fprintf(stderr, "Sent wingarea: %6f ", fgcontrol.parkingBrake);
        fprintf(stderr, "Engine running: %i ", fgcontrol.starter);
        //byteSwap(fgcontrol.magnetos);
            //byteSwap(fgcontrol.starter);
        byteSwap(fgcontrol.throttle);
        byteSwap(fgcontrol.parkingBrake);
        ret = udp->send(&fgcontrol, sizeof(FGControl));
        fgcontrol.up = 0;
        upVal = 0;

    }
}

osg::Vec3d FlightGear::getPosition()
{
OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mutex); 
return osg::Vec3d(fgdata.position[0],fgdata.position[1],fgdata.position[2]);
}

osg::Vec3d FlightGear::getOrientation()
{
OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mutex); 
return osg::Vec3d(fgdata.orientation[0],fgdata.orientation[1],fgdata.orientation[2]);
}

void FlightGear::setThermal(bool thermalActivity)
{
thermal =thermalActivity;
}
