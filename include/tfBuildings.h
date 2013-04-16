#ifndef _BUILDINGS_H_
#define _BUILDINGS_H_

#include <vector>
#include "tfPoints.h"
#include "tfVector.h"

using namespace std;

enum BuildingType_t {
	SKYSCRAPER,
	COMMERCIAL,
	RESIDENTAL
};

/*
===========================================================================
tfBuildings

-public variables: points, buildingType
-public methods: Height()
Buildings generated from blocks consist of points (stored in vector in the
same order as they are connected), building type (assigned by block class)
and height of building (needs to be calculated within the class)

===========================================================================
*/
class tfBuildings{
public:
	tfBuildings(vector<tfPoints>,BuildingType_t);
	vector<tfPoints>	points;			//vertices of building base
	BuildingType_t		buildingType;
	int					floors;
	void				SetNumberOfFloors(int,int[]);
	char*				ReturnType();
};

#endif /* _BUILDINGS_H_ */
