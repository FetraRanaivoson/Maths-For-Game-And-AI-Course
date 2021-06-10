#pragma once
#include "../lib_Point/Point.h"
class GameObject
{
private:
	Point positionA;
	Vector speed;			//in px per sec
	Vector acceleration; //in px per sec per sec
	double mass;

	__time64_t lastUpdate; //date in nanoseconds since last update

	__time64_t getTimeInNanoSeconds();

public:
	GameObject(double mass, Point position, Vector speed);
	Point getPosition();
	Vector getSpeed();

	void setSpeed(Vector newSpeed);

	void update();
	void draw(SDL_Renderer* renderer, Color color);


};

