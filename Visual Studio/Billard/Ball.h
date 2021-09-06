#pragma once
#include "../lib_Point/Point.h"
#include "Queue.h"

class Ball
{
private:
	double mass; //kg
	Point center;
	double radius; //pixel
	Vector speed;
	Vector propulsion;
	Vector acceleration;
	Vector friction; //Friction de la table probablement

	bool ballIsMoving;


	__time64_t lastUpdate;	//	date in nanoseconds since last update
	__time64_t getTimeInNanoSeconds();

	int width, height;

public:
	Ball(double mass, Point center, double radius, int width, int height);
	void update(SDL_Event& event, Queue queue, Vector queueSpeed);


	Point& getCenter();


	void listenForHit(Queue queue, Vector queueSpeed);
	void listenForHit(Ball* ball, Vector ballSpeed);


	void setPropulsion(Vector newPropulsion);
	Vector& getPropulsion();


	void setSpeed(Vector queueSpeed);
	Vector getSpeed();

	
	Vector& computeFriction();


	bool isHitBy(Queue queue);
	bool isHitBy(Ball* ball);

	
	void checkBordersAndDrawBall(SDL_Renderer* renderer, const Color& color);
	

	void visualizeVectorSpeed(SDL_Renderer* renderer);
	void visualizePath(Queue& queue, SDL_Renderer* renderer);


	void draw(SDL_Renderer* renderer, Color color, SDL_Event& event, Queue queue, Vector queueSpeed);

	
};

