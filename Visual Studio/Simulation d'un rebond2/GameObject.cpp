#include "..\Elastique\GameObject.h"
#include "..\Elastique\GameObject.h"
#include "..\Elastique\GameObject.h"
#include "GameObject.h"
#include <time.h>

__time64_t GameObject::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
	//Nombre de nanosec écoulé depuis le 1er janvier 70
}

GameObject::GameObject(double mass, Point position, Vector speed, int width, int height)
	: positionA(position), speed(speed), acceleration(0, 0) {
	this->mass = mass;
	this->lastUpdate = this->getTimeInNanoSeconds();
	this->width = width;
	this->height = height;
}

void GameObject::update(Slider* accelerationX, Slider* accelerationY) {
	__time64_t currentUpdate = this->getTimeInNanoSeconds();

	//	compute elapsed time (in seconds) since last position change
	double deltaT = (currentUpdate - this->lastUpdate) / 1e9;

	this->lastUpdate = currentUpdate;

	//	compute new position of object now
	this->positionA.x = this->positionA.x + this->speed.x * deltaT;
	this->positionA.y = this->positionA.y + this->speed.y * deltaT;
	//	OR this->position = this->position + this->speed * deltaT;		//	using library operators

	//	compute new speed of object now
	this->speed.x = this->speed.x + this->acceleration.x * deltaT;
	this->speed.y = this->speed.y + this->acceleration.y * deltaT;
	//	OR this->speed = this->speed + this->acceleration * deltaT;		//	using library operators

	//	compute sum of forces pallyed on object
	Vector gravity(0, 9.81 * this->mass);
	Vector wind(-50, 0);
	//Vector spring(0, -this->position.y / 5);
	Vector propulsion(accelerationX->getValue(), accelerationY->getValue()); //////////////
	Vector sum = gravity + propulsion;
	
	// compute new acceleration of object now
	this->acceleration.x = sum.x / mass;
	this->acceleration.y = sum.y / mass;
}

double GameObject::sgn(double speed)
{
	if (speed < 0)
		return -1;
	else
		return 1;
}

Point GameObject::getPosition() {
	return this->positionA;
}

double GameObject::getSpeedX() {
	return this->speed.x;
}

double GameObject::getSpeedY()
{
	return this->speed.y;
}

void GameObject::setSpeed(Vector newSpeed) {
	this->speed = newSpeed;
}

void GameObject::setSpeed(Slider* speedX, Slider* speedY)
{
	this->speed.x = sgn(getSpeedX()) * speedX->getValue();
	this->speed.y = sgn(getSpeedY()) * speedY->getValue();

}



void GameObject::draw(SDL_Renderer* renderer, Color color, Slider* accelerationX, Slider* accelerationY) {
	this->update(accelerationX, accelerationY);

	//	check borders of screen
	if ((this->positionA.x > width && this->speed.x > 0) ||
		(this->positionA.x < 0 && this->speed.x < 0)) {
		this->speed.x *= -0.9;
	}
	if ((this->positionA.y > height && this->speed.y > 0) ||
		(this->positionA.y < 0 && this->speed.y < 0)) {
		this->speed.y *= -0.9;
	}

	this->positionA.draw(renderer, color, 6);
	//this->position.drawCircle(renderer, 10, color, true);
}
