#pragma once
#include "../lib_Point/Point.h"
#include <iostream>
#include "Object.h"


class Boid
{
private:
	Vector boidPosition;
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

	Vector direction;

public:
	Boid(Vector position, int size, double alpha, double maxSpeed, double maxAcceleration, Color pointColor);

	//COHESION
	static int nbBoids;
	static Point sumOfPositions;
	static Point meanPointCohesion;
	static Point getMeanPointCohesion(double cohesionCoef);

	//ALIGNMENT
	static Vector sumOfSpeeds;
	static Vector meanTargetSpeed;
	static Vector getMeanTargetSpeed(double alignmentCoef);


	void lerp(Vector initialPosition, Vector destinationPosition, double alpha);
	void followSimple(SDL_Renderer* renderer, int clickPosX, int clickPosY);
	void followRealistic(SDL_Renderer* renderer, Point target, Object* area, std::vector <Object*> props);
	void update();
	void draw(SDL_Renderer* renderer);

	void avoidObstacle(std::vector <Object*> props);


	//Getters
	Vector& getPosition();
	Vector& getDirection();
	Vector& getSpeed();
	

	bool isAround(Object* area);


};

