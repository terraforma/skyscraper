#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>
#include <string>
#define PI 3.14159265

class tfVector
{
public:
	double x,y,z;
	tfVector();
	tfVector(double x, double y, double z);

	double Magnitude();
	double DotProduct(tfVector v);
	tfVector UnitVector();
	tfVector Invert();
	tfVector Add(tfVector v);
	tfVector Subtract(tfVector v);
	tfVector CrossProduct(tfVector v);
	double ScalarTripleProduct(tfVector b, tfVector c);
	double AngleRad(tfVector v);
	double AngleDeg(tfVector v);
	tfVector Multiply(double d);
	tfVector Normal(tfVector v);
};

#endif /* _VECTOR_H_ */
