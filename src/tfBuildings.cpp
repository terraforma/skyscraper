#include "tfBuildings.h"
#include <random>

tfBuildings::tfBuildings(vector<tfPoints> points, BuildingType_t buildingType){
	this->points=points;
	this->buildingType=buildingType;
	this->floors=0;
}

/*
====================
SetNumberOfFloors

Authors: Arron Fitt & Richard Konecny
Calculates number building floors according to building type, defined limits and population
====================
*/
void tfBuildings::SetNumberOfFloors(int population, int limits[]){
	//calculation of number of floors for residental buildings
	if(population < limits[1] && population >= limits[0]){
		int min = 1;
		float max = 8;
		int FloorNum;
		int l1=limits[0],l2=limits[1];
		//random range is 0-3 floors
		double random=rand()%4;
		FloorNum=min+((population-l1)/((l2-l1)/(max-min)))+random;
		this->floors=FloorNum;
	}
	//calculation of number of floors for commercial buildings
	else if(population < limits[2] && population >= limits[1]){
		int min = 20;
		float max = 70;	//Tallest commerial building in New York										
		int FloorNum;
		int l1=limits[1],l2=limits[2];
		//random range is 0-9 floors
		double random=rand()%10;
		FloorNum=min+((population-l1)/((l2-l1)/(max-min)))+random;
		this->floors=FloorNum;
	}
	//calculation of number of floors for skyscrapers
	else if(population >= limits[2]) {
		int min = 75; //Minimum height for modern buildings to be considered skyscrapers
		float max = 102; //Tallest skyscraper in New York
		int FloorNum;
		int l1=limits[2],l2=limits[3];
		FloorNum=min+((population-l1)/((l2-l1)/(max-min)));	
		this->floors=FloorNum;
	}
}

/*
====================
ReturnType

Returns building type in char* format
====================
*/
char* tfBuildings::ReturnType(){
	switch (this->buildingType){
	case RESIDENTAL:
		return "residental";
	case SKYSCRAPER:
		return "skyscraper";
	case COMMERCIAL:
		return "commercial";
	}
}
