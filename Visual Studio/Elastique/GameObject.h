#pragma once

#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"

class GameObject {
private:
	double mass;

	Point positionA;
	Point positionB;

	Vector speedA;		//	in pixels per second
	Vector speedB;
	Vector accelerationA;	//	in pixels per second per second
	Vector accelerationB;

	__time64_t lastUpdate;	//	date in nanoseconds since last update

	int width, height;	//	limit of box

	__time64_t getTimeInNanoSeconds();


	double sgn(double speed);

public:
	//	constructor
	GameObject(double mass, Point positionA, Point positionB, Vector speedA, Vector speedB, int width, int height);
	void update(SDL_Event& event, double& initialDistanceInX, double& initialDistanceInY);

	//	getters and setters
	Point getPositionA();
	Point getPositionB();
	double ABInitialDistance(double& initialDistance);
	double getABDistanceInX();
	double getABDistanceInY();
	double getABDistance();

	double getSpeedX();
	double getSpeedY();

	void setSpeed(Vector newSpeed);
	void setSpeed(Slider* speedX, Slider* speedY);

	void setAcceleration(Vector newAcceleration);



	void draw(SDL_Renderer* renderer, Color color, SDL_Event& event, double& initialDistanceInX, double& initialDistanceInY);
};

