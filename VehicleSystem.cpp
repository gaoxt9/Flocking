#include "VehicleSystem.h"
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

VehicleSystem::VehicleSystem(int windowWidth, int windowLength) 
{
	width = windowWidth;
	length = windowLength;
}

void VehicleSystem::setup(int number) 
{
	for (size_t i = 0; i < number; i++)
	{
		vehicles.push_back(Vehicle(rand() % width, rand() % length));
	}
}

void VehicleSystem::addVehicle(Vector2f pos)
{
	vehicles.push_back(Vehicle(pos.x, pos.y));
}

void VehicleSystem::applyBehavior(int toggleForce) 
{
	for (size_t i = 0; i < vehicles.size(); i++)
	{
		Vector2f separate(0, 0);
		Vector2f align(0, 0);
		Vector2f cohesion(0, 0);

		if (toggleForce == 1) separate = vehicles[i].separationForce();
		if (toggleForce == 2) align = vehicles[i].alignForce();
		if (toggleForce == 3) cohesion = vehicles[i].cohesionForce();

		//switch (toggleForce)
		//{
		//case 1:
		//	separate = vehicles[i].separationForce();
		//	break;
		//case 2:
		//	align = vehicles[i].alignForce();
		//	break;
		//case 3:
		//	cohesion = vehicles[i].cohesionForce();
		//	break;
		//default:
		//	break;
		//}

		separate *= 1.5f;
		align *= 1.f;
		cohesion *= 1.1f;

		Vector2f appliedForce = separate + align + cohesion;

		vehicles[i].applyForce(appliedForce);
	}
}

void VehicleSystem::draw()
{
	for (size_t i = 0; i < vehicles.size(); i++)
	{

		vehicles[i].update(width, length);
		vehicles[i].display();
	}
}

VehicleSystem::~VehicleSystem() {}