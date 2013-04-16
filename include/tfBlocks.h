#ifndef _BLOCKS_H_
#define _BLOCKS_H_

#include <vector>
#include "tfBuildings.h"

using namespace std;

enum AreaType_t {
	BSKYSCRAPER,
	BCOMMERCIAL,
	BRESIDENTAL,
	TOO_SMALL,
	PARK
};

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
	AreaType_t			areaType;
	int					population;		//average population density (0-255)
	void				ScaleBlockArea(int[],double);
	float				Area();
	void				SetAreaType(int[],double);
	vector<tfBuildings> CreateBuildingBases(double);
};

#endif /* _BLOCKS_H_ */
