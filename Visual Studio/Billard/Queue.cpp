#include "Queue.h"
#include <time.h>
#include <iostream>
using namespace std;

__time64_t Queue::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
	//Nombre de nanosec écoulé depuis le 1er janvier 70
}

Queue::Queue(double mass, double turn, Point tip, Point handle ,int width, int height)
	: tip(tip), handle(handle), queueSpeed(0, 0), propulsion(0, 0), acceleration(0, 0), friction(0, 0)
{
	this->mass = mass;
	this->turn = turn;
	this->lastUpdate = this->getTimeInNanoSeconds();
}


void Queue::update(SDL_Event& event) {

	__time64_t currentUpdate = this->getTimeInNanoSeconds();

	this->handle.update(event);
	this->tip.update(event);

	//	compute elapsed time (in seconds) since last position change
	double deltaT = (currentUpdate - this->lastUpdate) / 1e9;
	this->lastUpdate = currentUpdate;

	//	compute new position of object now
	this->tip= this->tip + this->queueSpeed * deltaT;
	this->handle = this->handle + this->queueSpeed * deltaT;
	
	//	compute new speed of object now
	this->queueSpeed = this->queueSpeed + this->acceleration * deltaT;

	this->controllQueue(event);

	//	compute sum of forces pallyed on object
	//Vector gravity(0, 0 * this->mass);		  //Activate gravity
	Vector propulsion(this->getPropulsion()); //Increase acceleration

	Vector friction(this->computeFriction()); //Decrease the speed gradually if no acceleration
	Vector sum = propulsion + friction;

	// compute new acceleration of object now
	this->acceleration.x = sum.x / mass;
	this->acceleration.y = sum.y / mass;

}

void Queue::controllQueue(SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_s) { 
			
		}
		if (event.key.keysym.sym == SDLK_z) {	
			
		}
		if (event.key.keysym.sym == SDLK_q) {//ROTATE LEFT
			this->incrementTurn(5);	//en radian
		}
		if (event.key.keysym.sym == SDLK_d) {//ROTATE RIGHT
			this->incrementTurn(-5);	//en radian
		}
		if (event.key.keysym.sym == SDLK_LEFT) { 
			Vector newPropulsion(-1500, 0);
			this->setPropulsion(newPropulsion);
		}
		if (event.key.keysym.sym == SDLK_RIGHT) { 
			Vector newPropulsion(1500, 0);
			this->setPropulsion(newPropulsion);
		}
		if (event.key.keysym.sym == SDLK_UP) { //GO UP PARALLEL TO QUEUE ORIENTATION
			this->setPropulsion(1500 * this->getNormalizedQueueVector());
		}
		if (event.key.keysym.sym == SDLK_DOWN) { //GO DOWN PARALLEL TO QUEUE ORIENTATION
			this->setPropulsion(-1500 * this->getNormalizedQueueVector());
		}
	}
	if (event.type == SDL_KEYUP) { //RELEASE KEY
		this->setPropulsion(Vector(0, 0));
	}
}

void Queue::incrementTurn(double newTurn)
{
	this->turn += newTurn * M_PI / 10;
}

double& Queue::getTurn()
{
	return this->turn;
}

void Queue::setPropulsion(Vector newPropulsion)
{
	this->propulsion = newPropulsion;
}

Vector& Queue::getPropulsion()
{
	return this->propulsion;
}

Vector Queue::getNormalizedQueueVector()
{
	Vector u(this->handle, this->tip);
	return u/u.magnitude();
}

Vector Queue::getVectorPerpendicularToQueue()
{
	Point perp(this->tip.x + cos(M_PI / 2 + this->turn* M_PI/180), this->tip.y - sin(M_PI / 2 + this->turn) * 250);
	Vector p(this->tip, perp);
	return p;
}

Point& Queue::getQueueTip()
{
	return this->tip;
}

Point& Queue::getQueueHandle()
{
	return this->handle;
}


Vector& Queue::computeFriction()
{
	this->friction = 5 * - this->queueSpeed;   
	return this->friction;
}

void Queue::drawQueue(SDL_Renderer* renderer, const Color& color)
{
	this->tip.draw(renderer, color, 5);
	this->handle.x = this->tip.x + cos(this->turn * M_PI / 180) * 250;
	this->handle.y = this->tip.y - sin(this->turn * M_PI / 180) * 250;
	this->handle.draw(renderer, color, 10);

	SDL_RenderDrawLine(renderer, this->tip.x, this->tip.y, this->handle.x, this->handle.y);
	SDL_RenderDrawLine(renderer, this->tip.x, this->tip.y, this->tip.x + cos(M_PI/2 + this->turn * M_PI/180) *250,
		this->tip.y - sin(M_PI/2 + this->turn *M_PI/180) * 250);

}

void Queue::draw(SDL_Renderer* renderer, Color color, SDL_Event& event) {

	drawQueue(renderer, color);
	//visualizeVectors(renderer);

	this->update(event);
}








