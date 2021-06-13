#include "VaisseauObject.h"
#include <time.h>
#include <iostream>
using namespace std;

__time64_t VaisseauObject::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
	//Nombre de nanosec écoulé depuis le 1er janvier 70
}

//VaisseauObject::VaisseauObject()
//	: mass(0),
//	turn(0),
//	radius(0),
//	width(0),
//	height(0),
//	isLaserAlive(false),
//	lastUpdate(0),
//	speed(0, 0), acceleration(0, 0), laserSpeed(0, 0), propulsion(0, 0), friction(0, 0)
//{
//}

VaisseauObject::VaisseauObject(double mass, double turn, double radius, Point centerPosition, int width, int height)
	: centerPosition(centerPosition), speed(0, 0), acceleration(0, 0), laserSpeed(0, 0), propulsion(0, 0), friction(0, 0) {
	this->mass = mass;
	this->turn = turn;
	this->radius = radius;
	this->lastUpdate = this->getTimeInNanoSeconds();
	this->width = width;
	this->height = height;
	this->isLaserAlive = false;
}

double VaisseauObject::sgn(double speed)
{
	if (speed < 0)
		return -1;
	else
		return 1;
}

void VaisseauObject::update(SDL_Event& event) {

	__time64_t currentUpdate = this->getTimeInNanoSeconds();

	this->centerPosition.update(event);

	//	compute elapsed time (in seconds) since last position change
	double deltaT = (currentUpdate - this->lastUpdate) / 1e9;

	this->lastUpdate = currentUpdate;

	//	compute new position of object now
	this->centerPosition.x = this->centerPosition.x + this->speed.x * deltaT;
	this->centerPosition.y = this->centerPosition.y + this->speed.y * deltaT;
	//	OR this->position = this->position + this->speed * deltaT;		//	using library operators

	//	compute new speed of object now
	this->speed.x = this->speed.x + this->acceleration.x * deltaT;
	this->speed.y = this->speed.y + this->acceleration.y * deltaT;
	//	OR this->speed = this->speed + this->acceleration * deltaT;		//	using library operators

	controlVaisseau(event);

	//	compute sum of forces pallyed on object
	Vector gravity(0, 0 * this->mass);		  //Activate gravity
	Vector propulsion(this->getPropulsion()); //Increase acceleration

	Vector friction(this->computeFriction()); //Decrease the speed gradually if no acceleration
	Vector sum = gravity + propulsion + friction;

	// compute new acceleration of object now
	this->acceleration.x = sum.x / mass;
	this->acceleration.y = sum.y / mass;



	// compute new position of laser
	this->laser = this->laser + this->laserSpeed * deltaT;


	//While the laser is inside the window, we can't fire any more lasers

	if (this->laser.x > this->width || this->laser.y > this->height || this->laser.x < 0 || this->laser.y < 0) {
		this->isLaserAlive = false;

	}

	//cout << this->getPropulsion().x << ", " << this->getPropulsion().y << endl;
	//cout << this->speed.x << ", " << this->speed.y << "   " << this->acceleration.x << ", " << this->acceleration.y << endl;

}

Vector& VaisseauObject::computeFriction()
{

	this->friction.x = 500 * -this->getSpeed().x;     // y = Yo * exp (-k * speed() * speed() ) NB: greater k => quicker the stop
	this->friction.y = 500 * -this->getSpeed().y;


	/*if (this->friction.magnitude() < 1000) {
		Vector stop(0, 0);
		this->setSpeed(stop);
	}*/
	return this->friction;
}


Point VaisseauObject::getPosition() {
	return this->centerPosition;
}


Point* VaisseauObject::getPoints()
{
	return this->points;
}

Vector& VaisseauObject::getSpeed() {
	return this->speed;
}


void VaisseauObject::setSpeed(Vector newSpeed) {
	this->speed = newSpeed;
}

void VaisseauObject::setPropulsion(Vector newPropulsion)
{
	this->propulsion = newPropulsion;
}

Vector& VaisseauObject::getPropulsion()
{
	return this->propulsion;
}

Vector VaisseauObject::getVectorCenterToTip()
{
	Vector v(this->centerPosition, this->points[0]);
	return v / v.magnitude();
}


void VaisseauObject::setAcceleration(Vector newAcceleration)
{
	this->acceleration = newAcceleration;
}

void VaisseauObject::incrementTurn(double turn)
{
	this->turn += turn * M_PI / 200;
}

void VaisseauObject::controlVaisseau(SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN) {

		if (event.key.keysym.sym == SDLK_s) { //GO DOWN PARALLEL TO VAISSEAU ORIENTATION
			this->setPropulsion(-250000 * this->getVectorCenterToTip());
		}
		if (event.key.keysym.sym == SDLK_z) {	//GO UP PARALLEL TO VAISSEAU ORIENTATION
			this->setPropulsion(250000 * this->getVectorCenterToTip());
		}
		if (event.key.keysym.sym == SDLK_q) {//ROTATE LEFT
			this->incrementTurn(5);	//en radian
		}
		if (event.key.keysym.sym == SDLK_d) {//ROTATE RIGHT
			this->incrementTurn(-5);	//en radian
		}
		if (event.key.keysym.sym == SDLK_LEFT) { //SLIDE LEFT
			Vector newPropulsion(-250000, 0);
			this->setPropulsion(newPropulsion);
		}
		if (event.key.keysym.sym == SDLK_RIGHT) { //SLIDE RIGHT
			Vector newPropulsion(250000, 0);
			this->setPropulsion(newPropulsion);
		}
		if (event.key.keysym.sym == SDLK_UP) { //SLIDE UP
			//Vector newPropulsion(0, -250000);
			//this->setPropulsion(newPropulsion);
			this->fireLaser();
		}
		if (event.key.keysym.sym == SDLK_DOWN) { //SLIDE DOWN
			Vector newPropulsion(0, 250000);
			this->setPropulsion(newPropulsion);
		}
		if (event.key.keysym.sym == SDLK_x) { //ALLUMEZ LE FEU !!!
			this->fireLaser();
		}
	}
	if (event.type == SDL_KEYUP) { //RELEASE KEY
		Vector stop = (0, 0);
		this->setPropulsion(stop);
	}
}

void VaisseauObject::DrawVaisseau(SDL_Renderer* renderer, const Color& color)
{

	this->centerPosition.draw(renderer, color, 2);
	this->centerPosition.drawCircle(renderer, this->radius, color ,true);

	for (int i = 0; i < 3; i++) {
		this->points[i].x = this->centerPosition.x + cos(this->angles[i] * M_PI / 180 + this->turn) * this->radius;
		this->points[i].y = this->centerPosition.y - sin(this->angles[i] * M_PI / 180 + this->turn) * this->radius;
		this->points[i].draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 2);
	}

	SDL_RenderDrawLine(renderer, points[0].x, points[0].y, points[1].x, points[1].y);
	SDL_RenderDrawLine(renderer, points[1].x, points[1].y, points[2].x, points[2].y);
	SDL_RenderDrawLine(renderer, points[2].x, points[2].y, points[0].x, points[0].y);
}

void VaisseauObject::visualizeVectors(SDL_Renderer* renderer)
{
	//Visualize speed vector (reverse)
	SDL_SetRenderDrawColor(renderer, 255, 125, 2, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, this->centerPosition.x, this->centerPosition.y,
		-this->speed.x + this->centerPosition.x, -this->speed.y + this->centerPosition.y);
	//Visualize propulsion vector (reverse)
	SDL_SetRenderDrawColor(renderer, 255, 225, 2, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, this->centerPosition.x, this->centerPosition.y,
		-this->propulsion.x / 1000 + this->centerPosition.x,
		-this->propulsion.y / 1000 + this->centerPosition.y);
}

void VaisseauObject::modifyPositionInCase()
{
	if (this->centerPosition.x > this->width) {
		this->centerPosition.x = 0;
	}
	if (this->centerPosition.y > this->height) {
		this->centerPosition.y = 0;
	}
	if (this->centerPosition.x < 0) {
		this->centerPosition.x = this->width;
	}
	if (this->centerPosition.y < 0) {
		this->centerPosition.y = this->height;
	}
}

void VaisseauObject::draw(SDL_Renderer* renderer, Color color, SDL_Event& event) {

	modifyPositionInCase();
	DrawVaisseau(renderer, color);
	visualizeVectors(renderer);

	if (this->isLaserAlive) {
		this->laser.draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), 10);
	}

	this->update(event);
}

void VaisseauObject::fireLaser()
{
	if (!this->isLaserAlive) {
		//Prepare the laser
		for (int i = 0; i < 7; i++) {
			this->laser = this->centerPosition;
			this->laserSpeed = 10000 * Vector(-turn);
			//Make this true so that in we can draw it in draw function
			this->isLaserAlive = true;
		}
	}
}

Point& VaisseauObject::getLaserPosition()
{
	return this->laser;
	//return nullptr;
}


