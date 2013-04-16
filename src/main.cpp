/*
  206 Skyscrapers

  Version 1.2
  Richard Konecny & Arron Fitt
*/
#include <vector>
#include "tfBlocks.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "XMLDocument.h"
#include "Bitmap.h"
#include <iostream>

using namespace std;

double scale=1, //map scale 1=512x512
	scale2=scale/3, //unit scale 1 unit represents 1/3 of meter
	mapWidth,
	mapHeight;
int limitsForRoads[]={0,20,100,150}, //population limits for roads width
	limitsForBuildings[]={0,70,130,180}; //population limits for building types
/*
  ====================
  ImportXML

  Author: Richard Konecny
  -RETURN: vector of blocks
  Import data from XML file, create blocks from nodes and relationships between them, set population
  to every block according to population density map
  ====================
*/
vector<tfBlocks> ImportXML(){
	/*
	  ===========================================================================
	  localNode

	  -public variables: coords, angle
	  -public methods: CalculateAngle()
	  Nodes imported from XML contain coordinates and class also stores angle
	  that is used for calculation of angle between adjacent nodes
	  ===========================================================================
	*/
	class localNode{
	public:
		tfPoints coords; //X and Y coordinates of the node
		double angle; //angle between adjacent nodes
		//constructor
		localNode(tfPoints coords){
			this->coords=coords;
		}
		//default constructor
		localNode(){}
		//calculate deflection angle of line, connecting current node and entered node, from X axis
		//(angle range is -180 - +180)
		void CalculateAngle(localNode node){
			angle=atan2(this->coords.y-node.coords.y,
						this->coords.x-node.coords.x)*(180/PI);
		};
	};

	vector<tfBlocks> blocks;
	double minAngle, //minimal angle
		angle, //angle
		angleSum=0.0f; //total sum of angles, used to determine whether the vertices are in
	//clockwise or anticlockwise order
	bool foundAllBlocks=false; //have all the blocks been already found?
	int idOfMinAngle, //id of node with minimal angle
		pivot, //it temporarily stores id of node
		prevPivot, //id of previous pivot
		firstPivot, //id of the first pivot
		currentNode; //id of node, for which program is currently creating blocks
	vector<int> tempVector; //temporary vector of ids used to calculate and compare angles between
	//nodes
	vector<tfPoints> points; //store all vertices for each block
	vector<tfVector> vectors; //vectors of all block sides
	vector<vector<int>> nodesRelations; //all the adjacent nodes for every node
	vector<localNode> nodes; //all the nodes
	LibTerra::tfXMLDocument doc; //xml document

	//import xml and create list of nodes and relations
	bool success = doc.Parse("road_map.xml");
	//test if file exists
	if (!success) {
		std::cerr << "XML Parse failed with error: " << doc.LastError() << std::endl;
	}
	else
	{
		//load file
		LibTerra::tfXMLNode root = doc.Root();
		//search for all xml tags used in this file
		for (LibTerra::tfXMLNode parameter = root.FirstChild(); parameter.Valid();
			 parameter = parameter.NextSibling()) {
			//read and set global width
			if(strcmp(parameter.Name(),"width")==0){
				mapWidth=strtod(parameter.ChildValue(),NULL)*scale;
			}
			//read and set global heights
			else if(strcmp(parameter.Name(),"height")==0){
				mapHeight=strtod(parameter.ChildValue(),NULL)*scale;
			}
			//read nodes
			else if(strcmp(parameter.Name(),"nodes")==0){
				int id; float x,y;
				for (LibTerra::tfXMLNode subParameter = parameter.FirstChild();
					 subParameter.Valid(); subParameter = subParameter.NextSibling()) {
					//save id and coordinates to nodes vector
					id=atoi(subParameter.Attribute("id").Value());
					x=atof(subParameter.Attribute("absX").Value())*scale;
					y=atof(subParameter.Attribute("absY").Value())*scale;
					if(nodes.size()<=id) nodes.resize(id+1);
					nodes[id]=tfPoints(x,y);
				}
			}
			//read relations between nodes
			else if(strcmp(parameter.Name(),"relations")==0){
				int id,a,b;
				for (LibTerra::tfXMLNode subParameter = parameter.FirstChild();
					 subParameter.Valid(); subParameter = subParameter.NextSibling()) {
					//save relations to nodesRelations vector in both directions
					//(A->B and B->A)
					a=atof(subParameter.Attribute("nodeA").Value());
					b=atof(subParameter.Attribute("nodeB").Value());
					if(nodesRelations.size()<=a) nodesRelations.resize(a+1);
					nodesRelations[a].push_back(b);
					if(nodesRelations.size()<=b) nodesRelations.resize(b+1);
					nodesRelations[b].push_back(a);
				}
			}
		}
	}
	vector<vector<int>>::iterator iIt=nodesRelations.begin();
	iIt++;
	currentNode=iIt-nodesRelations.begin();
	//create blocks from vector of nodes and sorted relations between them
	while(!foundAllBlocks){
		//clear points vector
		points.clear();
		//save the first node in points
		points.push_back(nodes[currentNode].coords);
		//set firstPivot and prevPivot to the first node and pivot to the first adjacent node of the
		//first node
		firstPivot=currentNode;
		prevPivot=currentNode;
		pivot=nodesRelations[currentNode][0];
		//remove the first adjacent node of node 0 from nodesRelations
		nodesRelations[prevPivot].erase(nodesRelations[prevPivot].begin());
		//create block by saving all the adjacent nodes in clockwise order
		while(pivot!=firstPivot){
			double minAngle=360;
			idOfMinAngle=0;
			//calculate angle between x axis and line segment from pivot to previous pivot
			nodes[prevPivot].CalculateAngle(nodes[pivot]);
			//find adjacent node that is next in anticlockwise order (because blocks are being
			//created in clockwise order)
			for(int i=0;i<nodesRelations[pivot].size();i++){
				//skip the previous node (which the new pivot came from)
				if(nodesRelations[pivot][i]==prevPivot) continue;
				//calculate angle between x axis and line segment from pivot to node at i-th
				//position
				nodes[nodesRelations[pivot][i]].CalculateAngle(nodes[pivot]);
				//calculate difference bewteen angle to previous node and angle to i-th node
				double difference=nodes[nodesRelations[pivot][i]].angle-nodes[prevPivot].angle;
				//we need to have only positive values, so we can add 360 degrees since it is circle
				if(difference<0) difference+=360;
				//check if the difference is lower that the lowes one found so far
				if(difference<minAngle) {
					//if the current angle is lower then minAngle replace it
					minAngle=difference;
					//save id of minimal angle to idOfMinAngle
					idOfMinAngle=i;
				}
			}
			//save pivot coordinations to vector of vertices for new block
			points.push_back(nodes[pivot].coords);
			//mark id of current pivot as id of previous pivot
			prevPivot=pivot;
			//new pivot is next id in vector of adjacent nodes
			//(since this vector is sorted)
			pivot=nodesRelations[pivot][idOfMinAngle];
			foundAllBlocks=true;
			//remove id that has already been saved as point of new block
			nodesRelations[prevPivot]
				.erase(nodesRelations[prevPivot].begin()+idOfMinAngle);
			//search for the first node, which vector of adjacent nodes is not empty, if vectors of
			//all the nodes are empty, it means that, all the blocks has already been created
			for(vector<vector<int>>::iterator iIt=nodesRelations.begin();
				iIt != nodesRelations.end(); ++iIt){
				int i=iIt-nodesRelations.begin();
				if(nodesRelations[i].size()>0){
					currentNode=i;
					foundAllBlocks=false;
					break;
				}
			}
			if(foundAllBlocks) break;
		}
		//search for exterior blocks (which points are in anticlockwise order). When this block is
		//found, it won't be saved
		vectors.resize(points.size());
		for(int i=0;i<points.size();i++){
			//create vectors of block sides
			vectors[i]=tfVector(points[i].x-points[(i+1)%points.size()].x,
								points[i].y-points[(i+1)%points.size()].y, 0);
		}
		for(int i=0;i<points.size();i++){
			//calculate angles between sides and X axis(range -180 - +180)
			angle=((atan2(vectors[(i+1)%points.size()].y,
						  vectors[(i+1)%points.size()].x)
					-(atan2(vectors[i].y, vectors[i].x)))*(180/PI));
			//change range to 0 - 360
			if(angle<0) angle+=360;
			angleSum+=angle;
		}
		if(angleSum<points.size()*180) {
			continue;
		}
		//save new block
		blocks.push_back(tfBlocks(points));
	}
	// Load the bitmap
	LibTerra::tfBitmap b("population_density_map.bmp");
	//set block population density for all blocks, block population density is equal to average of
	//population densities in all block vertices
	for(vector<tfBlocks>::iterator it=blocks.begin();it != blocks.end(); ++it){
		int sum=0; //sum of all population densities in vertices
		double xRel,yRel; //coordinates in bitmap coordinate system
		for (int i = 0; i < it->points.size(); i++)
		{
			//change roadmap coordinates to bitmap coordinates since dimensions, origin and y axis
			//of these coordinate systems are different
			xRel=(b.Width()/2)+((it->points[i].x/(mapWidth+1))*b.Width());
			yRel=(b.Height()/2)+((it->points[i].y/(mapHeight+1))*b.Height());
			//calculate sum of all population densities in vertices
			sum+=b.At(floor(xRel+0.5),floor(yRel+0.5));
		}
		//save average population density to block object
		it->population=floor((double(sum)/double(it->points.size()))+0.5);
	}
	return blocks;
}

/*
  ====================
  ExportXML

  Author: Richard Konecny
  Set Z coordinates according to imported elevation map and export XML file from vector of buildings
  ====================
*/
void ExportXML(vector<tfBuildings> buildings){
	//buffer for storing int value
	char buffer[33];
	LibTerra::tfXMLDocument doc; //XML document
	//create XML document
	LibTerra::tfXMLNode root = doc.CreateRoot("buildingmap");
	//write version and dimensions to XML doc
	root.AppendAttribute("version").SetValue("0.1.0");
	root.AppendChild("width").SetValue(itoa(mapWidth,buffer,10));
	root.AppendChild("height").SetValue(itoa(mapHeight,buffer,10));
	LibTerra::tfXMLNode nodeBuildings = root.AppendChild("buildings");
	//open elevation map
	LibTerra::tfBitmap b("elevation_map.bmp");
	//set Z coordinate to every corner of every building, all corners within the same building will
	//have the same value, that equals the minimal imported value at locations of bulding corners
	for(vector<tfBuildings>::iterator it=buildings.begin();it != buildings.end(); ++it){
		float minZ=1;
		double xRel,yRel;
		for (int i = 0; i < it->points.size(); i++)
		{
			//change buildingmap coordinates to bitmap coordinates since dimensions, origin and y
			//axis of these coordinate systems are different
			xRel=(b.Width()/2)+((it->points[i].x/(mapWidth+1))*b.Width());
			yRel=(b.Height()/2)+((it->points[i].y/(mapHeight+1))*b.Height());
			//read Z value at specified location from elevation map
			float currZ=b.At(floor(xRel+0.5),floor(yRel+0.5));
			//change range from 0-255 to 0-1
			currZ/=255;
			//find the smallest value of Z coordinate within all building corners
			if(currZ<minZ) minZ=currZ;
		}
		//set the smallest Z coordinate within all building corners to all building vertices
		for (int i = 0; i < it->points.size(); i++)
		{
			it->points[i].z=minZ;
		}
	}
	//save all buildings to XML file
	for (vector<tfBuildings>::iterator it = buildings.begin(); it != buildings.end(); ++it) {
		//create nodes and their attributes
		LibTerra::tfXMLNode nodeBuilding = nodeBuildings.AppendChild("building");
		nodeBuilding.AppendAttribute("id").SetValue(itoa(it-buildings.begin(),buffer,10));
		nodeBuilding.AppendAttribute("type").SetValue(it->ReturnType());
		nodeBuilding.AppendAttribute("numberOfFloors").SetValue(itoa(it->floors,buffer,10));
		//save all buildings from vector of tfBuildings
		for (vector<tfPoints>::iterator it2 = it->points.begin(); it2 != it->points.end(); ++it2) {
			LibTerra::tfXMLNode nodeBasePoint = nodeBuilding.AppendChild("basepoint");
			nodeBasePoint.AppendAttribute("id").SetValue(itoa(it2-it->points.begin(),buffer,10));
			nodeBasePoint.AppendAttribute("absX").SetValue(itoa(it2->x,buffer,10));
			nodeBasePoint.AppendAttribute("absY").SetValue(itoa(it2->y,buffer,10));
			char cVal[32];
			sprintf(cVal,"%f",it2->z);
			nodeBasePoint.AppendAttribute("absZ").SetValue(cVal);
		}
	}
	//save file
	if (!doc.Write("building_map.xml")) {
		std::cerr << "unable to write document" << doc.LastError() << std::endl;
	}
}

//Author: Richard Konecny
int main(int argc, char **argv)
{
	vector <tfPoints> points;
	//import XML
	vector<tfBlocks>blocks=ImportXML();
	cout<<"[0%] importing\n";
	vector<tfBuildings> buildings;
	int progress=0;
	for(vector<tfBlocks>::iterator it=blocks.begin();it!=blocks.end();++it){
		//scale blok area
		it->ScaleBlockArea(limitsForRoads,scale2);
		//set area type according to limits specified above, and block average population density
		it->SetAreaType(limitsForBuildings,scale2);
		if(it->areaType==TOO_SMALL || it->areaType==PARK)
			continue;
		vector<tfBuildings> localBuildings;
		//create building bases according to block average population density and area size
		localBuildings=it->CreateBuildingBases(scale2);
		//draw buildings from eye view
		for(vector<tfBuildings>::iterator it2=localBuildings.begin();it2!=localBuildings.end();++it2){
			//set number of floors according to building type, average population density and limits
			it2->SetNumberOfFloors(it->population,limitsForBuildings);
			buildings.push_back(localBuildings[it2-localBuildings.begin()]);
		}
		if(progress<floor((float)(it-blocks.begin())/(float)blocks.size()*100)){
			progress=floor((float)(it-blocks.begin())/(float)blocks.size()*100);
			cout<<"["<<progress<<"%] done: "<<it-blocks.begin()+1<<"/"<<blocks.size()<<" blocks\n";
		}
	}
	//export buildings to XML file
	ExportXML(buildings);
	cout<<"[100%] Completed: "<<buildings.size()<<" buildings generated";
	cin.ignore();
	return 0;
}
