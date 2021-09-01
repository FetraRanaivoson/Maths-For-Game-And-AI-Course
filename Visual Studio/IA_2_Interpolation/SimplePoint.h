#pragma once
#include "../lib_Point/Point.h"
#include <iostream>
class SimplePoint
{
private:
	Vector position;
	Color pointColor;
	int size;
	double alpha;

	Vector vitesse;
	Vector acceleration;
	double maxSpeed;
	double targetSpeed;

	Uint32 currentTime = 0;
	Uint32 latUpdate = 0;
	int deltaTime = 10;

	

public:
	SimplePoint(Vector position, int size, double alpha, Color pointColor);


	void lerp(Vector initialPosition, Vector destinationPosition, double alpha);
	void followSimple(SDL_Renderer* renderer, int clickPosX, int clickPosY);
	void followRealistic(SDL_Renderer* renderer, int clickPosX, int clickPosY);
	void draw(SDL_Renderer* renderer);

	Point& getPosition();

};

