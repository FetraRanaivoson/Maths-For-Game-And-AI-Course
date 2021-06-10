#pragma once

#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"
#include "Laser.h"

class VaisseauObject {
private:
	double mass;

	Point position;
	Vector speed;		//	in pixels per second
	Vector acceleration;	//	in pixels per second per second
	
	Point laser;
	Vector laserSpeed;

	__time64_t lastUpdate;	//	date in nanoseconds since last update
	__time64_t getTimeInNanoSeconds();

	int width, height;	//	limit of box

	double sgn(double speed);

public:
	//	constructor
	VaisseauObject(double mass, Point position, Vector speed, Vector laserSpeed, int width, int height);
	void update(SDL_Event& event);

	//	getters and setters
	Point getPosition();
	Vector getSpeed();

	void setSpeed(Vector newSpeed);
	void setSpeed(Slider* speedX, Slider* speedY);

	void setAcceleration(Vector newAcceleration);

	void fireLaser(SDL_Renderer* renderer, Color color, SDL_Event& event);



	void draw(SDL_Renderer* renderer, Color color, SDL_Event& event);
};

