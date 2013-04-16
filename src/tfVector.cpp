#include "tfVector.h"
#include <math.h>
#include <sstream>   
using namespace std;

tfVector::tfVector(){
	this->x=0;
	this->y=0;
	this->z=0;
}
tfVector::tfVector(double x, double y, double z){
	this->x=x;
	this->y=y;
	this->z=z;
}

//REQUIRED
//returns magnitude of the vector
double tfVector::Magnitude(){
	tfVector v(x,y,z);
	return sqrt(this->DotProduct(v));
}

//returns dot-product of the vector
double tfVector::DotProduct(tfVector v){
	return x*v.x+y*v.y+z*v.z;
}

//returns unit vector
tfVector tfVector::UnitVector(){
	double magnitude=Magnitude();
	double x,y,z;
	x=this->x/magnitude;
	y=this->y/magnitude;
	z=this->z/magnitude;
	return tfVector(x,y,z);
}

//returns inverted vector
tfVector tfVector::Invert(){
	tfVector v2(x,y,z);
	return v2.Multiply(-1);
}

//adds vector v to current vector and returns final vector
tfVector tfVector::Add(tfVector v){
	double x,y,z;
	x=this->x+v.x;
	y=this->y+v.y;
	z=this->z+v.z;
	return tfVector(x,y,z);
}

//subtracts vector v from current vector and returns final vector
tfVector tfVector::Subtract(tfVector v){
	tfVector v2=this->Add(v.Invert());
	return v2;
}

//returns corssproduct of current vector and vector v
tfVector tfVector::CrossProduct(tfVector v){
	double x,y,z;
	x=this->y*v.z-this->z*v.y;
	y=this->z*v.x-this->x*v.z;
	z=this->x*v.y-this->y*v.x;
	return tfVector(x,y,z);
}

//ADVANCED
//returns scalar-triple-product of current vector, vector b nd vector c
double tfVector::ScalarTripleProduct(tfVector b, tfVector c){
	tfVector v2(this->x,this->y,this->z);
	return v2.DotProduct(b.CrossProduct(c));
}

//returns angle between current vector and vector v in radians
double tfVector::AngleRad(tfVector v){
	tfVector v2(this->x,this->y,this->z);
	return acos(v.DotProduct(v2)/(v.Magnitude()*v2.Magnitude()));
}

//returns angle between current vector and vector v in degrees
double tfVector::AngleDeg(tfVector v){
	return AngleRad(v)*180/PI;
}

//multiplies current vector by d
tfVector tfVector::Multiply(double d){
	double x,y,z;
	x=this->x*d;
	y=this->y*d;
	z=this->z*d;
	return tfVector(x,y,z);
}

//returns normal vector of current vector and vector v
tfVector tfVector::Normal(tfVector v){
	tfVector v2(this->x,this->y,this->z);
	return v.CrossProduct(v2).UnitVector();
}