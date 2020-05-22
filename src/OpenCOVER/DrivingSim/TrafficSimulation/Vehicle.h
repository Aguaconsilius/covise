/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#ifndef Vehicle_h
#define Vehicle_h

#include "VehicleGeometry.h"

#include <string>
#include <list>
#include <set>
#include <deque>
#include <osg/Group>
#include <osg/Node>
#include <osg/Matrix>
namespace vehicleUtil
{
	class Road;
	class RoadTransition;
}

namespace TrafficSimulation
{
    class TRAFFICSIMULATIONEXPORT Vehicle
    {
    public:
        Vehicle(std::string);
        virtual ~Vehicle()
        {
        }

        void setName(std::string);
        std::string getName() const;
        int getVehicleID()
        {
            return vehicleID;
        }

        virtual vehicleUtil::Road* getRoad() const = 0;

        virtual double getU() const = 0;
        virtual double getDu() const = 0;
        virtual vehicleUtil::RoadTransition getRoadTransition() const;

        virtual int getLane() const
        {
            return 0;
        }
        //virtual std::set<int> getLaneSet() const { return std::set<int>(); }
        virtual bool isOnLane(int) const
        {
            return false;
        }

        virtual void move(double) = 0;
        virtual void setPosition(osg::Vec3& pos, osg::Vec3& direction) = 0;
        virtual void makeDecision() {};
        virtual bool canPass()
        {
            return true;
        }

        virtual VehicleGeometry* getVehicleGeometry() = 0;
        virtual vehicleUtil::Transform getVehicleTransform() = 0;
        virtual double getBoundingCircleRadius() = 0;

        double getSquaredDistanceTo(vehicleUtil::Vector3D point);

        static bool compare(const Vehicle*, const Vehicle*);

    protected:
        std::string name;

    private:
        static int vehicleIDs;
        int vehicleID; // unique ID for the vehicle
    };

    struct TRAFFICSIMULATIONEXPORT VehicleCompare
    {
        bool operator()(const Vehicle* veha, const Vehicle* vehb) const
        {
            return veha->getU() < vehb->getU();
        }
    };
    typedef std::list<Vehicle*> VehicleList;

    inline bool Vehicle::compare(const Vehicle* veha, const Vehicle* vehb)
    {
        return veha->getU() < vehb->getU();
    }

    typedef std::deque<Vehicle*> VehicleDeque;
}
#endif
