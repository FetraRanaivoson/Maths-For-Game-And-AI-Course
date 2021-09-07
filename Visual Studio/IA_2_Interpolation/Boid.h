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
	int deltaTime = 20;

	//Vector direction;

	Vector targetSpeed;
	//Vector targetAcceleration;
	Vector cohesionAcceleration;
	Vector alignmentAcceleration;
	Vector separationAcceleration;


	std::vector <Boid*> getBoidsInRange(double cohesionRadius);
	double attractionRadiusRange =70;
	double alignmentRadiusRange = 50;
	double repulsionRadiusRange = 30;
	//Vector repulsionVector;

	double targetWeight = 0.0;
	double cohesionWeight = 0.0;
	double separationWeight = 0.0;
	double alignmentWeight = 0.0;

public:
	Boid(Vector position, int size, double alpha, double maxSpeed, double maxAcceleration, Color pointColor);

	void setTargetWeight(double newTargetWeight);

	//COHESION (ATTRACTION)
	static int nbBoids;
	Point sumOfPositions;
	Vector getCohesionVector();
	void setCohesionWeight(double newCohesionWeight);

	//ALIGNMENT (ORIENTATION)
	Vector sumOfSpeeds;
	Vector getAlignementVector();
	void setAlignmentWeight(double newAlignmentWeight);

	//REPULSION (SEPARATION)
	Vector sumOfVectorDistance;
	//Vector getRepulsionVector();
	void setSeparationWeight(double newSeparationWeight);


	void lerp(Vector initialPosition, Vector destinationPosition, double alpha);
	void followSimple(SDL_Renderer* renderer, int clickPosX, int clickPosY);


	Vector getTargetAcceleration(SDL_Renderer* renderer, Point target, Object* area, std::vector <Object*> props);
	Vector getCohesionAcceleration(SDL_Renderer* renderer, Object* area, std::vector <Object*> props);
	Vector getAlignmentAcceleration(SDL_Renderer* renderer, Object* area, std::vector <Object*> props);
	Vector getSeparationAcceleration(SDL_Renderer* renderer,  Object* area, std::vector <Object*> props);
	
	void follow(SDL_Renderer* renderer, Point target, Object* area, std::vector<Object*> prop);


	void update();
	void draw(SDL_Renderer* renderer);

	void avoidObstacle(Vector direction, std::vector <Object*> props);


	//Getters
	Vector& getPosition();
	//Vector& getDirection();
	Vector& getSpeed();
	

	bool isAround(Object* area);


};

