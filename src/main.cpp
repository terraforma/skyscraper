/*
206 Skyscrapers

Version 1.0: startup structure
Richard Konecny & Arron Fitt
*/
#include <iostream>
#include <vector>

using namespace std;
enum AreaType_t :		char { TOO_SMALL, BUILDING, PARK};
enum BuildingType_t :	char { SKYSCRAPER, COMMERCIAL, RESIDENTAL};

/*
===========================================================================
tfPoints

-public variables: x,y,z
Points (or nodes) imported from XML with x, y, z coordinates

===========================================================================
*/
class tfPoints{
public:
	tfPoints(float,float,float);
	float	x,y,z;						//point coordinates
};

tfPoints::tfPoints(float x,float y, float z){
	this->x=x;
	this->y=y;
	this->z=z;
}

/*
===========================================================================
tfBuildings

-public variables: points, buildingType
-public methods: Height()
Buildings generated from blocks consist of points (stored in vector),
building tybe (assigned by block class) and height of building (needs
to be calculated within the class)

===========================================================================
*/
class tfBuildings{
public:
	tfBuildings(vector<tfPoints>,BuildingType_t);
	vector<tfPoints>	points;			//vertices of building base
	BuildingType_t		buildingType;
	int					Height();
};

tfBuildings::tfBuildings(vector<tfPoints> points, BuildingType_t buildingType){
	this->points=points;
	this->buildingType=buildingType;
}

/*
====================
Height

Calculates the height of the building according to building type and 
population map
====================
*/
int tfBuildings::Height(){
	//TODO
	return 0;
}

/*
===========================================================================
tfBlocks

-public variables: points
-public methods: ScaleBlockArea(), Area(), AreaType(), BuildingsType(),
CreateBuildingBases()
Blocks imported from XML consist of points (stored in vector) and need
to be modified and then used to create buildings
===========================================================================
*/
class tfBlocks{
public:
	tfBlocks(vector<tfPoints>);
	vector<tfPoints>	points;			//vertices of block
	void				ScaleBlockArea();
	float				Area();
	AreaType_t			AreaType();
	BuildingType_t		BuildingsType();
	vector<tfBuildings> CreateBuildingBases();
};

tfBlocks::tfBlocks(vector<tfPoints> points){
	this->points=points;
}

/*
====================
ScaleBlockArea

Shortens sides of block so there will stay enough place for roads. 
NOTE: roads in the city centre should be wider than roads located further
from centre of the city (/map?)
====================
*/
void tfBlocks::ScaleBlockArea(){
	//TODO
}

/*
====================
Area

Calculates the area of block (considering only x and y coordinates)
====================
*/
float tfBlocks::Area(){
	//TODO
	return 0.0f;
}

/*
====================
AreaType

-RETURN: TOO_SMALL or BUILDING or PARK
Decides whether the block area is suitable for building or park
====================
*/
AreaType_t tfBlocks::AreaType(){
	//TODO
	return BUILDING;
}

/*
====================
BuildingsType

Decides the type of the building according to area size, distance 
from the city centre and population map
====================
*/
BuildingType_t tfBlocks::BuildingsType(){
	//TODO
	return SKYSCRAPER;
}

/*
====================
CreateBuildingBases

-RETURN: vector of building
Splits block into building bases according to area size and type of 
buildings, removes bases without street access
====================
*/
vector<tfBuildings> tfBlocks::CreateBuildingBases(){
	vector<tfBuildings> buildings;
	//TODO
	return buildings;
}


/*
====================
ImportXML

-RETURN: vector of blocks
Imports data from XML file, creates blocks from nodes and relationships 
between them
====================
*/
vector<tfBlocks> ImportXML(){
	vector<tfBlocks> blocks;
	//TODO
	return blocks;
}

/*
====================
ExportXML

Exports XML file from vector of buildings
====================
*/
void ExportXML(vector<tfBuildings> buildings){
	//TODO
}

int main(int _argc, char *_argv[])
{
	//TODO
	return 0;
}
