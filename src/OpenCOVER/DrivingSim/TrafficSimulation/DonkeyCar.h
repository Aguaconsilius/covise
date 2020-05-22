/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#ifndef DonkeyCar_h
#define DonkeyCar_h

#include "Vehicle.h"
#include "CarGeometry.h"

#include <osg/MatrixTransform>

namespace TrafficSimulation
{
    class TRAFFICSIMULATIONEXPORT  DonkeyCar : public Vehicle
    {
    public:
        DonkeyCar(std::string, vehicleUtil::Road*, double, double, double, double);
        ~DonkeyCar();

        vehicleUtil::Road* getRoad() const;

        double getU() const;
        double getDu() const;

        void move(double);
        void setPosition(osg::Vec3& pos, osg::Vec3& direction);

        VehicleGeometry* getVehicleGeometry();
        double getBoundingCircleRadius();

    protected:
        vehicleUtil::Road* currentRoad;
        double u, v, du, h;
        int currentLane;
        int direction;

        VehicleGeometry* geometry;

        vehicleUtil::Transform carTransform;
    };
}

#endif
