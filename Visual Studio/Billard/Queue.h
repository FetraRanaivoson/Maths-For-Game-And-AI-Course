#pragma once

#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"



class Queue {
private:
	double mass;
	double turn; //angle en degré
	Point tip;
	Point handle;

	Vector queueSpeed;		//	in pixels per second
	Vector propulsion;	//in pixels per second
	Vector acceleration;	//	in pixels per second per second
	Vector friction;	//	in pixels per second per second

	__time64_t lastUpdate;	//	date in nanoseconds since last update
	__time64_t getTimeInNanoSeconds();

	int width, height;	//	limit of box


public:
	//	constructor
	Queue(double mass, double turn, Point tip, Point handle, int width, int height);
	void update(SDL_Event& event);


	void drawQueue(SDL_Renderer* renderer, const Color& color);
	void controllQueue(SDL_Event& event);
	void incrementTurn(double newTurn);
	double& getTurn();
	Point& getQueueTip();
	Point& getQueueHandle();

	Vector getNormalizedQueueVector();
	Vector getVectorPerpendicularToQueue();

	void setPropulsion(Vector newPropulsion);
	Vector& getPropulsion();

	
	Vector& computeFriction();

	//void visualizeVectors(SDL_Renderer* renderer);
	

	void draw(SDL_Renderer* renderer, Color color, SDL_Event& event);

};

