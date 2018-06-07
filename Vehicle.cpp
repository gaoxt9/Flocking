#include "Vehicle.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <time.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <iostream>

using namespace sf;


float Mag(Vector2f input)
{
	return sqrtf(input.x * input.x + input.y * input.y);
}

float Vehicle::getVelocity()
{
	return Mag(velocity);
}

Vector2f Limit(Vector2f input, float maxNum) 
{
	float mag = Mag(input);
	if (mag > maxNum && mag != 0)
	{
		return Vector2f(maxNum / mag * input.x, maxNum / mag * input.y);
	}
	else 
	{
		return input;
	}
}

Vector2f Normalize(Vector2f input) 
{
	float mag = Mag(input);
	if (mag != 0) 
	{
		return Vector2f(1.f / mag * input.x, 1.f / mag * input.y);
	}
	else 
	{
		return input;
	}
}


Vehicle::Vehicle(float posX, float posY) 
{
	acceleration = Vector2f(0, 0);
	velocity = Vector2f(-rand() % 10 + 5, -rand() % 10 + 5);
	location = Vector2f(posX, posY);

	r = 3; 
	maxspeed = 3.f;
	maxforce = 0.03f;

	triangle.setPointCount(3);
	triangle.setOutlineColor(Color::Yellow);
	triangle.setOutlineThickness(5);
	triangle.setFillColor(Color::Yellow);

	x = posX - r;
	y = posY - r;
}

Vehicle::~Vehicle() {}

void Vehicle::update(int windowWidth, int windowLength) 
{
	velocity += acceleration;
	velocity = Limit(velocity, maxspeed);
	location += velocity;

	// boundary check
	if (location.x < 0) 
		location.x = windowWidth;
	if (location.x > windowWidth) 
		location.x = 0;
	if (location.y < 0) 
		location.y = windowLength;
	if (location.y > windowLength) 
		location.y = 0;
	acceleration = Vector2f(0, 0);

	x = location.x - r;
	y = location.y - r;
}

void Vehicle::applyForce(Vector2f force) 
{
	acceleration += force;
}

Vector2f Vehicle::seek(Vector2f target) 
{
	Vector2f desired = target - location;

	float dNag = Mag(desired);
	desired = Normalize(desired);
	
	if (dNag < 80)
	{
		float m = dNag / 80 * maxspeed;
		desired *= m;
	}
	else 
	{
		desired *= maxspeed;
	}

	Vector2f steer = desired - velocity;
	steer = Limit(steer, maxforce);
	return steer;

}

void Vehicle::display() 
{
	float multipiler = 1.6f;
	Vector2f unit = Normalize(velocity) * r;

	triangle.setPoint(0, unit * multipiler);
	triangle.setPoint(1, Vector2f(-unit.x * multipiler + unit.y, -unit.y * multipiler - unit.x));
	triangle.setPoint(2, Vector2f(-unit.x * multipiler - unit.y, -unit.y * multipiler + unit.x));

	triangle.setPosition(location);
}

Vector2f Vehicle::separationForce() 
{
	float desiredSeparation = r * 30;
	Vector2f sum(0, 0);
	int count = 0;

	auto beginner = collideVehiclesList.begin();
	for (auto iter = collideVehicles.begin(); iter != collideVehicles.end(); iter++)
	{

		Vehicle * t = *iter;

		if (*beginner > 0 && *beginner < desiredSeparation) {
			Vector2f diff = location - t->location;
			diff = Normalize(diff);
			diff /= *beginner;
			sum += diff;
			count++;
		}
		beginner++;
	}

	Vector2f steer(0, 0);
	if (count > 0) 
	{
		steer = Normalize(sum / (float)count) * maxspeed - velocity;
		steer = Limit(steer, maxforce);
	}
	return steer;
}

Vector2f Vehicle::alignForce() 
{
	float desiredAlign = r * 30;
	Vector2f sum(0, 0);
	Vector2f steer(0, 0);
	int count = 0;

	auto beginner = collideVehiclesList.begin();
	for (auto iter = collideVehicles.begin(); iter != collideVehicles.end(); iter++)
	{
		Vehicle * t = *iter;
		if (*beginner > 0 && *beginner < desiredAlign)
		{
			sum += t->velocity;
			count++;
		}
		beginner++;
	}
	
	if (count > 0) 
	{
		steer = Normalize(sum / (float)count) * maxspeed - velocity;
		steer = Limit(steer, maxforce);
	}
	return steer;
}

Vector2f Vehicle::cohesionForce() 
{
	float desiredCohesion = r * 30;
	Vector2f sum(0, 0);
	int count = 0;

	auto beginner = collideVehiclesList.begin();
	for (auto iter = collideVehicles.begin(); iter != collideVehicles.end(); iter++)
	{
		Vehicle * t = *iter;
		if (*beginner > 0 && *beginner < desiredCohesion)
		{
			sum += t->location;
			count++;
		}
		beginner++;
	}

	Vector2f steer(0, 0);
	if (count > 0) 
	{
		sum /= (float)count;
		steer = seek(sum);
	}
	return steer;
}