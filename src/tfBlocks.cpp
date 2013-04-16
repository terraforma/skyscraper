#include "tfBlocks.h"

tfBlocks::tfBlocks(vector<tfPoints> points){
	this->points=points;
}

/*
====================
ScaleBlockArea

Author: Richard Konecny
Shortens sides of block so there will remain enough place for roads. 
NOTE: roads in the locations with higher population density are wider
====================
*/
void tfBlocks::ScaleBlockArea(int limits[],double scale){
	vector<tfVector> vectors(points.size()); //vectors of all the block sides
	vector<double> angles(points.size()); //angles between all the adjacent block sides
	vector<tfPoints> returnPoints(points.size()), //points that will replace current ones in the end
		tempPoints(2); //temporary points used for calculations of returnPoints
	double angleSum=0.0f, //total sum of angles, used to determine if the vertices are in clockwise 
		//or anticlockwise order
		tempAngle, //temporary angle used for calculations of returnPoints
		xDist, //distance that needs to be reducted in X axis
		yDist, //distance that needs to be reducted in Y axis (in case that block side is vertical)
		A1,B1,C1,A2,B2,C2, //parameters of general line equation (Ax+By+C=0) for 2 adjacent block 
		//sides
		reduction; //length to be reduces from each block side
	bool clockwise=false; //are vertices in clockwise or anticlockwise order?
	int tempPosNeg=1, //temporary variable used to change side on which reduction needs to be 
		//applied according to order of block vertices
		intAng; //angle between adjacent sides saved as integer calculate the reduction length based
	//on population density
	if(this->population>=limits[0] && this->population<limits[1])
		reduction=3*scale; //length that needs to be reduced
	else if(this->population>=limits[1] && this->population<limits[2])
		reduction=4*scale; //length that needs to be reduced
	else if(this->population>=limits[2] && this->population<limits[3])
		reduction=5*scale; //length that needs to be reduced
	else if(this->population>=limits[3])
		reduction=6*scale; //length that needs to be reduced
	//calculate vectors of all the block sides
	for(int i=0;i<points.size();i++){
		vectors[i]=tfVector(points[i].x-points[(i+1)%points.size()].x,
			points[i].y-points[(i+1)%points.size()].y, 0);
	}
	for(int i=0;i<points.size();i++){ 
		//calculate angle between vectors, in range (-180 - +180)
		angles[i]=((atan2(vectors[(i+1)%points.size()].y,
			vectors[(i+1)%points.size()].x)
			-(atan2(vectors[i].y, vectors[i].x)))*(180/PI));
		//change range of angles to (0 - 360)
		if(angles[i]==0) angles[i]=180;
		if(angles[i]<0) angles[i]+=360;
		//count total sum of all angles in block
		angleSum+=angles[i];
	}
	//determine order of block vertices (clockwise/anticlockwise)
	if(angleSum>points.size()*180) clockwise=true;
	//calculate new points
	for (int j=0;j<points.size();j++){
		xDist=yDist=0;
		//First line
		//calculate internal angle between two vectors, in range(0 - 180)
		tempAngle=acos(vectors[(j)%points.size()].DotProduct(tfVector(-1,0,0))/
			(vectors[(j)%points.size()].Magnitude()*
			tfVector(-1,0,0).Magnitude()));
		//change range to (-180 - +180)
		if(vectors[(j)%points.size()].y<0) tempAngle-=PI;
		//round angle
		intAng=floor(tempAngle*(180/PI)+0.5);
		//calculate lenght of which should be point shifted in X direction
		if(intAng!=0 && intAng!=180 ) xDist=reduction/sin(tempAngle);
		//if side was vertical, calculate lenght for Y direction shifting
		if(xDist==0){
			if(intAng==90 || intAng==-90) yDist=0;
			else yDist=reduction/cos(tempAngle);
		}
		else yDist=0;
		//change direction of point moving according to vertices order
		if(clockwise) tempPosNeg=-1;
		//shift points by xDist (or yDist)
		tempPoints[0].y=points[(j+0)%points.size()].y+yDist*tempPosNeg;
		tempPoints[0].x=points[(j+0)%points.size()].x+xDist*tempPosNeg;
		tempPoints[1].y=points[(j+1)%points.size()].y+yDist*tempPosNeg;
		tempPoints[1].x=points[(j+1)%points.size()].x+xDist*tempPosNeg;
		//set parameters of general line equasion for first line
		A1=tempPoints[1].y-tempPoints[0].y;
		B1=tempPoints[0].x-tempPoints[1].x ;
		C1=A1*tempPoints[0].x+B1*tempPoints[0].y;
		//Second line
		//the same proccess as it was used for the first line calculation is now being applied to 
		//second line
		tempAngle=acos(vectors[(j+1)%points.size()].DotProduct(tfVector(-1,0,0))/
			(vectors[(j+1)%points.size()].Magnitude()*
			tfVector(-1,0,0).Magnitude()));
		if(vectors[(j+1)%points.size()].y<0) tempAngle-=PI;
		intAng=floor(tempAngle*(180/PI)+0.5);
		if(intAng==0 || intAng==180 ) xDist=0;
		else xDist=reduction/sin(tempAngle);
		if(xDist==0){
			if(intAng==90 || intAng==-90) yDist=0;
			else yDist=reduction/cos(tempAngle);
		}
		else yDist=0;
		tempPoints[0].y=points[(j+1)%points.size()].y+yDist*tempPosNeg;
		tempPoints[0].x=points[(j+1)%points.size()].x+xDist*tempPosNeg;
		tempPoints[1].y=points[(j+2)%points.size()].y+yDist*tempPosNeg;
		tempPoints[1].x=points[(j+2)%points.size()].x+xDist*tempPosNeg;
		A2=tempPoints[1].y-tempPoints[0].y;
		B2=tempPoints[0].x-tempPoints[1].x ;
		C2=A2*tempPoints[0].x+B2*tempPoints[0].y;
		//calculates intersections of two shifted lines
		//if lines are parallel, set new point to their common shifted point
		if((A1*B2-A2*B1)==0){
			returnPoints[j].x=tempPoints[0].x;
			returnPoints[j].y=tempPoints[0].y;
		}
		else{
			returnPoints[j].x=(B2*C1-B1*C2)/(A1*B2-A2*B1);
			returnPoints[j].y=(A1*C2-A2*C1)/(A1*B2-A2*B1);
		}
	}
	//replace block vertices by intersections of lines that have been shifted by required length 
	//(reduction)
	points=returnPoints;
}

/*
====================
Area

Author: Arron Fitt
Calculates the area of block (considering only x and y coordinates)
====================
*/
float tfBlocks::Area(){
	float area=0.0f;
	int j;
	j=points.size()-1;
	for(int i=0;i<points.size();i++){
		area=area+(points[j].x+points[i].x)*(points[j].y-points[i].y);
		j=i;
	}
	area=abs(area/2);
	return area;
}

/*
====================
AreaType

Author: Arron Fitt
-RETURN: SKYSCRAPER or COMMERCIAL or RESIDENTAL or TOO_SMALL or PARK
Decides the type of the area according to area size and population map
====================
*/
void tfBlocks::SetAreaType(int limits[],double scale){
	//check defined conditions for residental buildings
	if(this->Area() >= scale*scale*30  && population >= limits[0] &&  population < limits[1])
		this->areaType=BRESIDENTAL;
	//check defined conditions for commercial buildings
	else if(this->Area() >= scale*scale*60 && population >= limits[1] &&  population < limits[2])
		this->areaType=BCOMMERCIAL;
	//check defined conditions for skyscrapers
	else if(this->Area() >= scale*scale*600 && this->Area() <= scale*scale*2600 && 
		population >= limits[2])	
		this->areaType=BSKYSCRAPER;
	//check defined conditions for parks. Size of smallest park of significance in New York
	//http://en.wikipedia.org/wiki/List_of_New_York_City_parks
	else if(this->Area() >= scale*scale*2600)
		this->areaType=PARK;
	//if none of the conditions above are satisfied, the block is marked as too small (traffic 
	//islands of carparks)
	else{
		this->areaType=TOO_SMALL;
	}
}

/*
====================
CreateBuildingBases

Authors: Richard Konecny & Arron Fitt
-RETURN: vector of building
Splits block into building bases according to area size and type of 
buildings, removes bases without street access
====================
*/
vector<tfBuildings> tfBlocks::CreateBuildingBases(double scale){
	vector<tfBuildings> buildings;
	//for skyscrapers building base is the plot (block) itself 
	if(this->areaType==BSKYSCRAPER){
		buildings.push_back(tfBuildings (this->points,BuildingType_t::SKYSCRAPER));
	}
	//for residental and commercial building generate building bases
	else if(this->areaType==BRESIDENTAL || this->areaType==BCOMMERCIAL){
		int requiredArea; //maximum area size allowed for one building base
		char type; //r=residental, c=commercial building
		if(this->areaType==BRESIDENTAL) {
			requiredArea=scale*scale*150; //150 meters for residental building
			type='r';
		}
		else if (this->areaType==BCOMMERCIAL) {
			requiredArea=scale*scale*500; //500 meters for commercial building
			type='c';
		}
		vector<tfBlocks> localBlocks;
		//local class used mainly to perform recursive method that splits blocks
		class localSplit{
		public:
			vector<tfPoints>* outerPoints; //pointer to corners of the first/largest block
			bool streetAccess; //does building base have a street access?
			int maxArea; //maximum area size allowed for one building base-identical to requiredArea
			char type; //r=residental, c=commercial building
			//constructor
			localSplit(vector<tfPoints>* outerPoints,int maxArea, char type){
				this->outerPoints=outerPoints;
				this->maxArea=maxArea;
				this->type=type;
			}
			//splits block into two smaller blocks. The way of splitting is following: find the 
			//longest side, choose a point on this side, create perpendicular line to the lingest 
			//side going through this point. This perpendicular line splits block into two parts
			void Split(tfBlocks block,vector<tfBlocks>& localBlocks){
				//if area size of block is lower than is required, check if this block have street 
				//access and if so save it into localBlocks
				if(block.Area()<=maxArea){
					int outer=0;
					//find how many points lie in sides of the original block. If there are at least
					//2 point lying there, block has street access
					for(int i=0;i<block.points.size();i++){
						//read corners of original block
						vector<tfPoints> points=*this->outerPoints;
						//find if current point lies in any oforiginal sides
						for(int j=0;j<points.size();j++){
							double locX=block.points[i].x;
							double locY=block.points[i].y;
							//first and secon vertices of side that is being checked
							tfPoints firstVertex(points[j].x,points[j].y);
							tfPoints secondVertex(points[(j+1)%points.size()].x,
								points[(j+1)%points.size()].y);
							//m variable means margin and is used here to set accurancy
							double m=0.0001;
							if(((locX>=firstVertex.x-m && locX<=secondVertex.x+m)
								|| (locX<=firstVertex.x+m && locX>=secondVertex.x-m))
								&&
								((locY>=firstVertex.y-m && locY<=secondVertex.y+m)
								|| (locY<=firstVertex.y+m && locY>=secondVertex.y-m))){
									outer++;
									continue;
							}
						}
					}
					//if block has more than one point lying in some of original block sides, save 
					//it bacaues it has street access, otherwise don't save it
					if(outer>1)
						localBlocks.push_back(block);
				}
				//if area size is still larger that maximum required, continue splitting
				else{
					double maxLength=0;
					int maxId;
					//find the longest side of block and save its id
					for(int i=0;i<block.points.size();i++){
						tfPoints p1=block.points[i];
						tfPoints p2=block.points[(i+1)%block.points.size()];
						double xDelta=p1.x-p2.x;
						double yDelta=p1.y-p2.y;
						double length=sqrt(xDelta*xDelta+yDelta*yDelta);
						if(length>maxLength){
							maxLength=length;
							maxId=i;
						}
					}
					//p1 an p2 are corners of the longest side
					tfPoints p1=block.points[maxId];
					tfPoints p2=block.points[(maxId+1)%block.points.size()];
					tfVector v1(p1.x,p1.y,0);
					//diff is vector going from p2 to p1
					tfVector diff(p2.x-p1.x,p2.y-p1.y,0);
					double random;
					//set random number
					if(type=='r'){
						random=rand()%20;
						random=random/100+0.40;
					}
					else if(type=='c'){
						random=rand()%40;
						random=random/100+0.30;
					}
					//set point lying in the longest side by chosing random point around the middle
					tfVector intSect1=v1.Add(diff.Multiply(random));
					//create vector for perpendicular line to the longest side
					tfVector perp(diff.y*(-1),diff.x,0);
					//find any point in perpendicular line which is used to define line equation
					tfVector pointInPrep=intSect1.Add(perp);
					//line equation of perpendicular line
					double A1,B1,C1,A2,B2,C2;
					A1=pointInPrep.y-intSect1.y;
					B1=intSect1.x-pointInPrep.x ;
					C1=A1*intSect1.x+B1*intSect1.y;
					//find second intersection
					tfVector intSect2;
					int pointer;
					//search the second intersection of perpendicular line in the rest of sides
					for(int i=0;i<block.points.size();i++){
						//if the side is the longest, skip it
						if (i==maxId) continue;
						//set vertices of current side
						tfVector firstVertex(block.points[i].x,block.points[i].y,0);
						tfVector secondVertex(block.points[(i+1)%block.points.size()].x,
							block.points[(i+1)%block.points.size()].y,0);
						//line equation of current side
						A2=secondVertex.y-firstVertex.y;
						B2=firstVertex.x-secondVertex.x ;
						C2=A2*firstVertex.x+B2*firstVertex.y;
						//if current line and perpendicular line to the longest side are not 
						//parallel, find the second intersection
						if((A1*B2-A2*B1)!=0){
							double locX=(B2*C1-B1*C2)/(A1*B2-A2*B1);
							double locY=(A1*C2-A2*C1)/(A1*B2-A2*B1);
							//margin for accurancy
							double m=0.0001;
							//check if intersection lies in line segment of current side
							if(((locX>=firstVertex.x-m && locX<=secondVertex.x+m)
								|| (locX<=firstVertex.x+m && locX>=secondVertex.x-m))
								&&
								((locY>=firstVertex.y-m && locY<=secondVertex.y+m)
								|| (locY<=firstVertex.y+m && locY>=secondVertex.y-m))){
									intSect2.x=locX;
									intSect2.y=locY;
									pointer=i;
									break;
							}
						}
					}
					vector<tfPoints> points;
					//create tfPoints objects of intersections
					tfPoints intSectPoint1(intSect1.x,intSect1.y);
					tfPoints intSectPoint2(intSect2.x,intSect2.y);
					//save points of the first block to points variable
					points.push_back(block.points[maxId]);
					points.push_back(intSectPoint1);
					points.push_back(intSectPoint2);
					int i=(pointer+1)%block.points.size();
					while (i!=maxId){
						points.push_back(block.points[i]);
						i=(i+1)%block.points.size();
					}
					//call recursive method for the first block
					Split(tfBlocks(points),localBlocks);
					//clear points variable
					points.clear();
					//save points of the second block to points variable
					points.push_back(block.points[pointer]);
					points.push_back(intSectPoint2);
					points.push_back(intSectPoint1);
					i=(maxId+1)%block.points.size();
					while (i!=pointer){
						points.push_back(block.points[i]);
						i=(i+1)%block.points.size();
					}
					//call recursive method for the second block
					Split(tfBlocks(points),localBlocks);
				}
			};
		};
		//create new object
		localSplit ls(&this->points,requiredArea,type);
		//call its recursive method that splits block into building bases
		ls.Split(tfBlocks(this->points),localBlocks);
		//save building bases to vector of tfBuildings object
		for(vector<tfBlocks>::iterator it=localBlocks.begin();it!=localBlocks.end();++it){
			if(type=='r')
				buildings.push_back(tfBuildings(it->points,BuildingType_t::RESIDENTAL));
			else if(type=='c')
				buildings.push_back(tfBuildings(it->points,BuildingType_t::COMMERCIAL));
		}
	}
	return buildings;
}