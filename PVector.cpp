#include "PVector.h"

PVector::PVector(float x_, float y_)
{
	x = x_;
	y = y_;
}

void PVector::add(PVector v)
{
	y = y + v.y;
	x = x + v.x;
}

void PVector::sub(PVector v)
{
	y = y - v.y;
	x = x - v.x;
}

void PVector::mult(float n) 
{
	x = x * n;
	y = y * n;
}

void PVector::div(float n) 
{
	x = x / n;
	y = y / n;
}

float PVector::mag() 
{
	return sqrt(x*x + y*y);
}

float PVector::heading()
{
	return atan2f(y, x);
}

void PVector::normalize() 
{
	float m = mag();
	if (m != 0) 
	{
		div(m);
	}
}

void PVector::limit(float maxforce) 
{
	float m = mag();
	if (m > maxforce) 
	{
		normalize();
		mult(maxforce);
	}
}

PVector::~PVector()
{
}
