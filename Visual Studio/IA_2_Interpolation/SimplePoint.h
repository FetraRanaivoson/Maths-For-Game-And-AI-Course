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

	Vector speed;
	Vector acceleration;
	double maxSpeed;
	double maxAcceleration;

	Uint32 currentTime = 0;
	Uint32 latUpdate = 0;
	int deltaTime = 10;

	

public:
	SimplePoint(Vector position, int size, double alpha, double maxSpeed, double maxAcceleration, Color pointColor);


	void lerp(Vector initialPosition, Vector destinationPosition, double alpha);
	void followSimple(SDL_Renderer* renderer, int clickPosX, int clickPosY);
	void update();
	void followRealistic(SDL_Renderer* renderer, Point target);
	void draw(SDL_Renderer* renderer);

	Point& getPosition();

};

