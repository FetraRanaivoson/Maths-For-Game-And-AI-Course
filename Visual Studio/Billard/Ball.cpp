#include "Ball.h"
#include <time.h>
#include <iostream>

Ball::Ball(double mass, Point center, double radius, int width, int height)
	: center(center), speed(0, 0), acceleration(0, 0), propulsion(0, 0), friction(0, 0)
{
	this->mass = mass;
	this->radius = radius;
	this->width = width;
	this->height = height;
	this->ballIsMoving = false;
	this->lastUpdate = this->getTimeInNanoSeconds();
}

void Ball::update(SDL_Event& event, Queue queue, Vector queueSpeed)
{
	__time64_t currentUpdate = this->getTimeInNanoSeconds();


	//	compute elapsed time (in seconds) since last position change
	double deltaT = (currentUpdate - this->lastUpdate) / 1e9;
	this->lastUpdate = currentUpdate;

	//	compute new position of object now
	this->center.x = this->center.x + this->speed.x * deltaT;
	this->center.y = this->center.y + this->speed.y * deltaT;

	//	compute new speed of object now
	this->speed.x = this->speed.x + this->acceleration.x * deltaT;
	this->speed.y = this->speed.y + this->acceleration.y * deltaT;

	//	compute sum of forces pallyed on object
	Vector gravity(0, 0 * this->mass);		  //Activate gravity
	Vector propulsion(this->getPropulsion()); //Increase acceleration

	Vector friction(this->computeFriction()); //Decrease the speed gradually if no acceleration
	Vector sum = propulsion + friction;

	// compute new acceleration of object now
	this->acceleration.x = sum.x / mass;
	this->acceleration.y = sum.y / mass;

	/*double distance = sqrt((queue.getQueueTip().x - this->center.x)
		* (queue.getQueueTip().x - this->center.x) +
		(queue.getQueueTip().y - this->center.y)
		* (queue.getQueueTip().y - this->center.y));

	std::cout << distance << std::endl;*/

	listenForHit(queue, queueSpeed);

}

Point& Ball::getCenter()
{
	return this->center;
}

void Ball::listenForHit(Queue queue, Vector queueSpeed)
{
	if (this->isHitBy(queue)) {
		this->setSpeed(queueSpeed); //Transfer speed to the ball
	}
}

void Ball::listenForHit(Ball* ball, Vector ballSpeed)
{
	if (this->isHitBy(ball)) {
		//Vitesses après collision v1prim = v2prim et v2prim = v1
		//Vector v1primVec(v1prim * sin(45 * M_PI / 180), v2prim * cos(45 * M_PI / 180));
		//Vector v2primVec(v2prim * sin(45 * M_PI / 180), v2prim * cos(45 * M_PI / 180));

		this->setSpeed(ball->speed);
		ball->setSpeed(this->speed/2);

		
	}
}

void Ball::setPropulsion(Vector newPropulsion)
{
	this->propulsion = newPropulsion;
}

Vector& Ball::getPropulsion()
{
	return this->propulsion;
}

void Ball::setSpeed(Vector queueSpeed)
{
	this->speed = queueSpeed;
}

Vector Ball::getSpeed()
{
	return this->speed;
}

Vector& Ball::computeFriction()
{
	//The higher the constant, the quicker the ball will stop
	this->friction.x = .25 * -this->getSpeed().x;
	this->friction.y = .25 * -this->getSpeed().y;

	return this->friction;
}

bool Ball::isHitBy(Queue queue)
{
	double distance = sqrt((queue.getQueueTip().x - this->center.x)
		* (queue.getQueueTip().x - this->center.x) +
		(queue.getQueueTip().y - this->center.y)
		* (queue.getQueueTip().y - this->center.y));

	//std::cout << distance << std::endl;
	return distance <= this->radius;

}

bool Ball::isHitBy(Ball* ball)
{
	double distance = sqrt((ball->center.x - this->center.x)
		* (ball->center.x - this->center.x) +
		(ball->center.y - this->center.y)
		* (ball->center.y - this->center.y));

	return distance <= 2 * this->radius;
}

void Ball::visualizePath(Queue& queue, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 70, 100, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, this->center.x, 
								 this->center.y, 
		this->center.x + cos( (180 + queue.getTurn()) * M_PI / 180) * 250,
		this->center.y - sin( (180 + queue.getTurn()) * M_PI / 180) *250);
}

void Ball::checkBordersAndDrawBall(SDL_Renderer* renderer, const Color& color)
{
	//	check borders of screen //Friction is function to speed so compute speed before friction
	if ((this->center.x + this->radius > this->width && this->speed.x > 0) ||
		(this->center.x - this->radius < 0 && this->speed.x < 0)) {
		this->speed.x *= -0.5;
	}
	if ((this->center.y + this->radius > this->height && this->speed.y > 0) ||
		(this->center.y - this->radius < 0 && this->speed.y < 0)) {
		this->speed.y *= -0.5;
	}

	//this->center.draw(renderer, color, 3);
	this->center.drawCircle(renderer, this->radius, color, true);
}

void Ball::visualizeVectorSpeed(SDL_Renderer* renderer)
{
	SDL_RenderDrawLine(renderer, this->center.x, this->center.y,
		-this->speed.x + this->center.x, -this->speed.y + this->center.y);
}

void Ball::draw(SDL_Renderer* renderer, Color color, SDL_Event& event, Queue queue, Vector queueSpeed)
{
	this->update(event, queue, queueSpeed);
	checkBordersAndDrawBall(renderer, color);
	visualizeVectorSpeed(renderer);
}

__time64_t Ball::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
	//Nombre de nanosec écoulé depuis le 1er janvier 70
}