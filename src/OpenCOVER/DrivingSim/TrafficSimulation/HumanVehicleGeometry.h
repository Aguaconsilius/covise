/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#ifndef HumanVehicleGeometry_h
#define HumanVehicleGeometry_h

#include "VehicleGeometry.h"
#include <osg/Group>
#include <osg/Node>
#include <osg/Matrix>
#include <osg/Transform>
#include <osg/MatrixTransform>

namespace TrafficSimulation
{
    class TRAFFICSIMULATIONEXPORT HumanVehicleGeometry : public VehicleGeometry
    {
    public:
        HumanVehicleGeometry(std::string);

        ~HumanVehicleGeometry();

        void setTransform(vehicleUtil::Transform&, double);

        double getBoundingCircleRadius();

        const osg::Matrix& getVehicleTransformMatrix();

        vehicleUtil::Transform getVehicleTransformation();

        osg::Node* getVehicleNode()
        {
            return vehicleNode;
        }

    protected:
        std::string vehicleNodeName;

        osg::Node* vehicleNode;

        double boundingCircleRadius;

        vehicleUtil::Transform vrmlTransform;
        vehicleUtil::Transform vehicleTransform;

        void findVehicleNode();
        osg::Node* searchGroupByVehicleNodeName(osg::Group*);
    };
}

#endif
