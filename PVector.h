#pragma once
#include <math.h>

class PVector
{
public:
	float x;
	float y;

	PVector(float, float);

	void add(PVector);

	void sub(PVector);

	void mult(float);

	void PVector::div(float);

	float mag();

	void normalize();

	float heading();

	void limit(float max);

	~PVector();
};

