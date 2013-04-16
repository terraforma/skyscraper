#ifndef _POINTS_H_
#define _POINTS_H_

/*
===========================================================================
tfPoints

-public variables: x,y,z
Points (or nodes) imported from XML with x, y, z coordinates

===========================================================================
*/
class tfPoints{
public:
	tfPoints(float,float);
	tfPoints();
	float	x,y,z;						//point coordinates
};

#endif /* _POINTS_H_ */
