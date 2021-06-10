#pragma once
#include "../lib_Point/Point.h"

class Laser {
private:
	double mass;

	Point position;
	Vector speed;		//	in pixels per second
	Vector acceleration;	//	in pixels per second per second

	__time64_t lastUpdate;	//	date in nanoseconds since last update
	__time64_t getTimeInNanoSeconds();

	int width, height;	//	limit of box

	double sgn(double speed);

public:
	//	constructor
	Laser(double mass, Point position, Vector speed, int width, int height);
	void update(SDL_Event& event);

	//	getters and setters
	Point getPosition();
	Vector getSpeed();

	void setSpeed(Vector newSpeed);

	void setAcceleration(Vector newAcceleration);



	void draw(SDL_Renderer* renderer, Color color, SDL_Event& event);
};