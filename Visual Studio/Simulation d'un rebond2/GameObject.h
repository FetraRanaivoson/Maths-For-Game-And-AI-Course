#pragma once

#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"

class GameObject {
private:
	double mass;

	Point positionA;
	Vector speed;		//	in pixels per second
	Vector acceleration;	//	in pixels per second per second

	__time64_t lastUpdate;	//	date in nanoseconds since last update

	int width, height;	//	limit of box

	__time64_t getTimeInNanoSeconds();


	double sgn(double speed);

public:
	//	constructor
	GameObject(double mass, Point position, Vector speed, int width, int height);
	void update(Slider* accelerationX, Slider* accelerationY);

	//	getters and setters
	Point getPosition();

	double getSpeedX();
	double getSpeedY();

	void setSpeed(Vector newSpeed);
	void setSpeed(Slider* speedX, Slider* speedY);

	void setAcceleration(Slider* accelerationX, Slider* accelerationY);

	void draw(SDL_Renderer* renderer, Color color, Slider* accelerationX, Slider* accelerationY);
};

