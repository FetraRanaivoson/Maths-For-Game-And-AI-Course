#pragma once

#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"



class VaisseauObject {
private:
	double mass;
	double turn; //angle en radian
	double angles[3] = {0,150,210};
	double radius;
	Point centerPosition;
	Point points[3];


	Vector speed;		//	in pixels per second
	Vector propulsion;	//in pixels per second
	Vector acceleration;	//	in pixels per second per second
	Vector friction;	//	in pixels per second per second


	bool isLaserAlive;
	Point laser;
	Vector laserSpeed;


	__time64_t lastUpdate;	//	date in nanoseconds since last update
	__time64_t getTimeInNanoSeconds();

	int width, height;	//	limit of box


	double sgn(double speed);

	bool isBallHit = false;

public:
	//	constructor
	//VaisseauObject();
	VaisseauObject(double mass, double turn, double radius, Point centerPosition, int width, int height);
	void update(SDL_Event& event);


	void visualizeVectors(SDL_Renderer* renderer);

	void modifyPositionInCase();

	void draw(SDL_Renderer* renderer, Color color, SDL_Event& event);
	void DrawVaisseau(SDL_Renderer* renderer, const Color& color);


	Point getPosition();
	Point* getPoints();
	Vector& getSpeed();
	

	Vector& getPropulsion();
	Vector getVectorCenterToTip();
	Vector& computeFriction();

	void controlVaisseau(SDL_Event& event);
	void setSpeed(Vector newSpeed);
	void setAcceleration(Vector newAcceleration);/**/
	void setPropulsion(Vector newPropulsion);
	void incrementTurn(double turn); 


	void fireLaser();
	Point& getLaserPosition();


};

