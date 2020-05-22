#ifndef REFERENCEPOSITION_H
#define REFERENCEPOSITION_H

#include <string>
#include <osg/Vec3>
#include <VehicleUtil/RoadSystem/RoadSystem.h>
#include <VehicleUtil/RoadSystem/Road.h>

class ReferencePosition
{

public:
    ReferencePosition();
    ReferencePosition(const ReferencePosition *oldRefPos); // copy constructor
    ~ReferencePosition();

    bool isUp2Date;
    // Road
    std::string roadId;
    vehicleUtil::Road* road;
    double roadLength;
    vehicleUtil::RoadSystem* system;
    vehicleUtil::LaneSection* section;

    double s;
    double t;
    double hdg;
	double offset;
    // Lane
    int laneId;
    vehicleUtil::LaneSection *LS;

    // Absolute
    osg::Vec3 xyz;

    // intialize ReferencePosition:
    void init(std::string init_roadId, int init_laneId, double init_s, vehicleUtil::RoadSystem *system, double offset); // via Lane Coordinates
    void init(osg::Vec3 initPos, double init_hdg, vehicleUtil::RoadSystem* init_system); // via World Coordinates
    void init(std::string, double init_s, double init_t, vehicleUtil::RoadSystem* init_system); // via Road Coordinates

    void move(double ds, double dt, float step); // move Reference Position forward
    void move(osg::Vec3 dirVec,float step_distance); // move Reference Position forward (w/o Road)

	void moveto(osg::Vec3 dirVec, float step_distance);

    osg::Vec3 getPosition();
    void update(std::string init_roadId, double init_s, int init_laneId);
    void update(std::string init_roadId, double init_s, double init_t);
    void update(double x, double y, double z, double init_hdg);
    void update();


	//double getLaneWidth(int lane, double s) const;

	int getLane();

    void getSuccessor();
    void getPredecessor();

    void resetStatus();
};


#endif 
