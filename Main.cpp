#include "Vehicle.h"
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

using namespace std;

const int WindowWidth = 1800;
const int WindowHeight = 1200;

float Dist(sf::Vector2f a, sf::Vector2f b) 
{
	return sqrtf(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

string ButtonText(int forceOption, int index) 
{
	if (forceOption == index)
	{
		return "On";
	}
	else
	{
		return "Off";
	}
}

int main() 
{
	sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Flocking");

	VehicleSystem vs(WindowWidth, WindowHeight);

	vs.setup(20);
	int toggleForce = 0;

	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text amountText;

	amountText.setFont(font);
	amountText.setCharacterSize(30);
	amountText.setPosition(WindowWidth / 35, WindowHeight / 35);
	amountText.setFillColor(sf::Color::Red);

	while (window.isOpen()) 
	{
		for (size_t i = 0; i < vs.vehicles.size(); i++)
		{
			vs.vehicles[i].collideVehicles.clear();
			vs.vehicles[i].collideVehiclesList.clear();

			list<Vehicle *> resObjects;
			vs.vehicles[i].collideVehicles = resObjects;

			for (auto iter = vs.vehicles[i].collideVehicles.begin(); iter != vs.vehicles[i].collideVehicles.end(); iter++)
			{
				Vehicle * t = *iter;
				vs.vehicles[i].collideVehiclesList.push_back(Dist(vs.vehicles[i].location, t->location));
			}
		}

		sf::Event event;
		while (window.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == Event::MouseButtonPressed) 
			{
				vs.addVehicle(sf::Vector2f((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y));
			}

			// bits in "toggleforce" control each force's on/off. from right: 1st bit separate, 2ed bit align, 3rd bit cohesion
			if (event.type == Event::KeyReleased) 
			{
				if (event.key.code == sf::Keyboard::S) 
					toggleForce = 1;
				if (event.key.code == sf::Keyboard::A) 
					toggleForce = 2;
				if (event.key.code == sf::Keyboard::C) 
					toggleForce = 3;
			}

		}

		vs.applyBehavior(toggleForce);
		vs.draw();
		window.clear();

		for (size_t i = 0; i < vs.vehicles.size(); i++)
		{
			window.draw(vs.vehicles[i].triangle);
		}
		
		amountText.setString("Vehicle: " + to_string(vs.vehicles.size()) + "\n" + "Velocity: " + to_string(vs.vehicles[0].getVelocity()) + " \n" + "Separation is " + ButtonText(toggleForce, 1) + "\n" + "Alignment is " + ButtonText(toggleForce, 2) + "\n" + "Cohesion is " + ButtonText(toggleForce, 3));
		window.draw(amountText);
		window.display();
	}


	return 0;

}