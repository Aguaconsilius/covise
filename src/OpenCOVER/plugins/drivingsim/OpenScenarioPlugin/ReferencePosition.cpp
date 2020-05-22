#include "ReferencePosition.h"
#include <iostream>
#include <cmath>
#include "Entity.h"

using namespace vehicleUtil;

ReferencePosition::ReferencePosition():
    road(NULL),
    s(0.0),
    t(0.0),
    hdg(0.0),
    laneId(0),
    roadLength(0.0),
    LS(NULL),
    roadId(""),
	//offset(0.0),
    isUp2Date(false)
{
}

ReferencePosition::~ReferencePosition()
{}

// copy constructor
ReferencePosition::ReferencePosition(const ReferencePosition* oldRefPos)
{
    // Road
    roadId = oldRefPos->roadId;
    road = oldRefPos->road;
    roadLength = oldRefPos->roadLength;
    system = oldRefPos->system;

    s = oldRefPos->s;
    t = oldRefPos->t;
    hdg = oldRefPos->hdg;
	//offset = oldRefPos->offset;
    // Lane
    laneId = oldRefPos->laneId;
    LS = oldRefPos->LS;

    // Absolute
    xyz = oldRefPos->xyz;
}

void ReferencePosition::init(std::string init_roadId, int init_laneId, double init_s, vehicleUtil::RoadSystem* init_system, double init_offset)
// init from Lane
{
    roadId = init_roadId;
    laneId = init_laneId;
    s = init_s;
    system = init_system;
	offset = init_offset;

    road = system->getRoad(roadId);
	if(road)
	{
		hdg = road->getHeading(s);


		LS = road->getLaneSection(s);
		Vector2D laneCenter = LS->getLaneCenter(laneId, s);

		t = laneCenter[0]+offset;

		Transform vtrans = road->getRoadTransform(s, t);
		xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());

		roadLength = road->getLength();
		
	}
	else
	{
		std::cerr << "Did not find road with id: " << roadId << std::endl;
	}
    isUp2Date = true;
}

void ReferencePosition::init(std::string init_roadId,double init_s,double init_t, vehicleUtil::RoadSystem* init_system)
// init from Road
{
    roadId = init_roadId;
    t = init_t;
    s = init_s;
    system = init_system;

    road = system->getRoad(roadId);
    if(road)
    {
        hdg = road->getHeading(s);

        LS = road->getLaneSection(s);
        laneId = LS->searchLane(s,t);
		Vector2D laneCenter = LS->getLaneCenter(laneId, s);
		offset = t - laneCenter[0];
        Transform vtrans = road->getRoadTransform(s, t);
        xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());

        roadLength = road->getLength();
    }
    else
    {
        std::cerr << "Did not find road with id: " << roadId << std::endl;
    }
    isUp2Date = true;
}

void ReferencePosition::init(osg::Vec3 initPos, double init_hdg, vehicleUtil::RoadSystem* init_system)
// init from World
{
    hdg = init_hdg;
    xyz = initPos;
    system = init_system;

    //Vector2D searchHere = Vector2D(x,y); road->isOnRoad(searchHere);
    const Vector3D newPoint = Vector3D(xyz[0],xyz[1],xyz[2]);
    Vector2D output = system->searchPosition(newPoint, road, s);

    if(road != NULL){

        Vector2D stNew = road->searchPosition(newPoint, s);

        s = stNew[0];
        t = stNew[1];

        LaneSection* newLS = road->getLaneSection(s);
        LS = newLS;

        laneId = LS->searchLane(s,t);
		Vector2D laneCenter = LS->getLaneCenter(laneId, s);
		offset = t - laneCenter[0];

        Transform vtrans = road->getRoadTransform(s, t);
        xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());

        roadLength = road->getLength();
    }
    isUp2Date = true;

}

void ReferencePosition::move(double ds, double dt, float step)
{
    double ds1 = ds/sqrt(ds*ds+dt*dt);
    double dt1 = dt/sqrt(ds*ds+dt*dt);

    s = s+ds1*step;
    t = t+dt1*step;

    double phi = atan(dt1/ds1);

    if(s>roadLength)
    {
        this->getSuccessor();
    }
    else if(s<0)
    {
        this->getPredecessor();
    }
    if (s < 0)
    {
        s = 0;
    }
    hdg = phi + road->getHeading(s);

    LaneSection* newLS = road->getLaneSection(s);

    if (newLS != LS)
    {
        laneId =  road->searchLane(s,t);
        LS = newLS;
    }
	Vector2D laneCenter = LS->getLaneCenter(laneId, s);
	offset = t - laneCenter[0];

    Transform vtrans = road->getRoadTransform(s,t);
    xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());
    isUp2Date = true;
}

void ReferencePosition::move(osg::Vec3 dirVec, float step_distance)
{
	xyz = xyz+(dirVec*step_distance);
}



osg::Vec3 ReferencePosition::getPosition()
{
    return xyz;
}

int ReferencePosition::getLane()
{
	auto actualLane = road->searchLane(s, t);
	return actualLane;
	
	
	
}

void ReferencePosition::update()
{
    if(isUp2Date)
    {
        return;
    }
    if(road == NULL)
    {
        const Vector3D newPoint = Vector3D(xyz[0],xyz[1],xyz[2]);
        Vector2D output = system->searchPosition(newPoint, road, s);

        if (road != NULL)
        {
            Vector3D newPoint = Vector3D(xyz[0],xyz[1],xyz[2]);
            Vector2D stNew = road->searchPosition(newPoint, s);
            roadLength = road->getLength();
            roadId = system->getRoadId(road);

            s = stNew[0];
            t = stNew[1];
            LaneSection* newLS = road->getLaneSection(s);
            LS = newLS;

            laneId = LS->searchLane(s,t);
        }
    }
    else
    {
        Vector3D newPoint = Vector3D(xyz[0],xyz[1],xyz[2]);
        Vector2D stNew = road->searchPosition(newPoint, s);

        s = stNew[0];
        t = stNew[1];
        if(!std::isnan(s))
        {
            LaneSection* newLS = road->getLaneSection(s);
            LS = newLS;

            laneId = LS->searchLane(s,t);
            roadLength = road->getLength();
            roadId = system->getRoadId(road);
			Vector2D laneCenter = LS->getLaneCenter(laneId, s);
			offset = t - laneCenter[0];
        }
        else
        {
            const Vector3D newPoint = Vector3D(xyz[0],xyz[1],xyz[2]);
            Vector2D output = system->searchPosition(newPoint, road, s);

            if (road != NULL)
            {
                Vector3D newPoint = Vector3D(xyz[0],xyz[1],xyz[2]);
                Vector2D stNew = road->searchPosition(newPoint, s);
                roadLength = road->getLength();
                roadId = system->getRoadId(road);

                s = stNew[0];
                t = stNew[1];

                LaneSection* newLS = road->getLaneSection(s);
                LS = newLS;

                laneId = LS->searchLane(s,t);
				Vector2D laneCenter = LS->getLaneCenter(laneId, s);
				offset = t - laneCenter[0];
            }

        }
        isUp2Date = true;
    }
}

void ReferencePosition::update(std::string init_roadId, double init_s, int init_laneId)
{
    s = init_s;
    laneId = init_laneId;

    if(init_roadId != roadId)
    {
        road = system->getRoad(init_roadId);
        roadLength = road->getLength();
    }
    roadId = init_roadId;
    hdg = road->getHeading(s);

    LaneSection* newLS = road->getLaneSection(s);
    LS = newLS;

    Vector2D laneCenter = LS->getLaneCenter(laneId, s);
    t = laneCenter[0]+offset;

    Transform vtrans = road->getRoadTransform(s,t);
    xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());
    isUp2Date = true;
}

void ReferencePosition::update(std::string init_roadId, double init_s, double init_t)
{
    s = init_s;
    t = init_t;

    if(init_roadId != roadId)
    {
        road = system->getRoad(init_roadId);
        roadLength = road->getLength();
    }
    roadId = init_roadId;
    hdg = road->getHeading(s);

    LaneSection* newLS = road->getLaneSection(s);
    LS = newLS;

    laneId = LS->searchLane(s,t);
	Vector2D laneCenter = LS->getLaneCenter(laneId, s);
	offset = t - laneCenter[0];

    Transform vtrans = road->getRoadTransform(s,t);
    xyz = osg::Vec3(vtrans.v().x(), vtrans.v().y(), vtrans.v().z());
    isUp2Date = true;
}

void ReferencePosition::update(double x, double y, double z,double init_hdg)
{
    xyz = osg::Vec3(x,y,z);
    hdg = init_hdg;

    update();
}

void ReferencePosition::getSuccessor()
{
    TarmacConnection* connection;

    connection = road->getSuccessorConnection();
	if (connection)
	{
		road = dynamic_cast<vehicleUtil::Road *>(connection->getConnectingTarmac());

		s = s - roadLength;
		roadId = system->getRoadId(road);
		roadLength = road->getLength();
	}
}

void ReferencePosition::getPredecessor()
{
    TarmacConnection* connection;

    connection = road->getPredecessorConnection();
    if(connection)
    {
        vehicleUtil::Road *newRoad = dynamic_cast<vehicleUtil::Road *>(connection->getConnectingTarmac());
        if(newRoad !=nullptr)
        {
            road = newRoad;
            roadId = system->getRoadId(road);
            roadLength = road->getLength();
            s = roadLength + s;
        } // else we are on an intersection or the road ends TODO handle this case
    }
}
void ReferencePosition::resetStatus()
{
    isUp2Date = false;
}
