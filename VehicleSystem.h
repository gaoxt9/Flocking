#pragma once
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

class VehicleSystem {


	Vector2f alignmentForce;
	Vector2f cohisionForce;
	int width;
	int length;

public:
	std::vector<Vehicle> vehicles;
	VehicleSystem(int, int);
	void addVehicle(Vector2f);
	void setup(int);
	void draw();
	void applyBehavior(int);
	~VehicleSystem();
};