#pragma once
#include "VaisseauObject.h"
#include "../lib_Point/Point.h"


class Asteroid
{
private:
	Point center;
	double radius;
	double mass;
	Vector speed;
	Vector propulsion;	
	Vector acceleration;

	__time64_t lastUpdate;	//	date in nanoseconds since last update
	__time64_t getTimeInNanoSeconds();


	int width, height;	//	limit of box

public:
	Asteroid(Point center, Vector speed, double radius, double mass, int width, int height);

	void modifyPositionInCase();

	void draw(SDL_Renderer* renderer, Color color, SDL_Event& event);
	void update(SDL_Event& event);

	void setPropulsion(Vector newPropulsion);
	void setSpeed(Vector newSpeed); 
	void decrementRadius(double radius);

	Vector& getPropulsion();
	double& getRadius();
	Point& getCenter();

	void moveAsteroid(SDL_Event& event);


	bool isHitBy(VaisseauObject vaisseau);
	bool Hit(VaisseauObject vaisseau);
};

