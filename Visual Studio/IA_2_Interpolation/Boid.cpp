#include "Boid.h"


int Boid::nbBoids = 0;
Point Boid::sumOfPositions = { 0,0 };
Point Boid::meanPointCohesion = { 0,0 };

Vector Boid::sumOfSpeeds = { 0,0 };
Vector Boid::meanTargetSpeed = { 0,0 };

Point Boid::getMeanPointCohesion(double cohesionCoef)
{
	meanPointCohesion = { cohesionCoef * (sumOfPositions.x / nbBoids),  cohesionCoef * (sumOfPositions.x / nbBoids) };
	return meanPointCohesion;
}

Vector Boid::getMeanTargetSpeed(double alignmentCoef)
{
	//meanTargetSpeed = {alignmentCoef * (sumOfSpeeds.x / nbBoids), alignmentCoef * (sumOfSpeeds.y / nbBoids) };
	meanTargetSpeed = alignmentCoef * sumOfSpeeds / nbBoids;
	return meanTargetSpeed;
}


void Boid::lerp(Vector initialPosition, Vector destinationPosition, double alpha)
{
	if (alpha >= 0 && alpha <= 1) {
		boidPosition = (1 - alpha) * initialPosition + alpha * destinationPosition;
	}
}



Boid::Boid(Vector position, int size, double alpha, double maxSpeed, double maxAcceleration, Color pointColor)
	: boidPosition(position), pointColor(pointColor), speed(0, 0), acceleration(0, 0), direction(0, 0)
{
	this->size = size;
	this->alpha = alpha;

	this->maxSpeed = maxSpeed;
	this->maxAcceleration = maxAcceleration;

	//this->maxSpeed = 300;
	//this->maxAcceleration = 175;

	Boid::nbBoids++;
}


void Boid::followSimple(SDL_Renderer* renderer, int clickPosX, int clickPosY)
{
	Vector destination(clickPosX, clickPosY);
	this->lerp(boidPosition, destination, alpha);
}

void Boid::update()
{
	currentTime = SDL_GetTicks();
	if (currentTime - latUpdate > deltaTime) {
		boidPosition += speed * (double)(currentTime - latUpdate) / 1000;
		speed += acceleration * (double)(currentTime - latUpdate) / 1000;
		latUpdate = currentTime;
	}
}

void Boid::followRealistic(SDL_Renderer* renderer, Point target, Object* area, std::vector <Object*> props)
{
	//Step 1: get the direction
	if (this->isAround(area)) {
		direction = { this->boidPosition, area->getPosition() };
		direction = -direction / 1;
	}
	else {
		//direction = Vector(boidPosition, target);
		double targetWeight = .95;
		double cohesionWeight = .05;
		direction = targetWeight * Vector(boidPosition, target) + cohesionWeight * Vector(boidPosition, getMeanPointCohesion(11 / 12));
	}

	//Step 1.5: Just in case in front of props
	this->avoidObstacle(props);

	//Step 2: limit this direction because it's too big
	//Vector targetSpeed = direction / direction.magnitude() * maxSpeed; // =======> vecteur v * L / norme(v)  where L is the new magnitude
	double alignmentWeight = .15;
	double targetWeight = .85;
	Vector alignmentSpeed = alignmentWeight * getMeanTargetSpeed(11 / 12);
	Vector targetSpeed = targetWeight * direction / direction.magnitude() * maxSpeed + alignmentSpeed.magnitude();


	//Step 3: desiredAcceleration = targetSpeed - currentSpeed 
	Vector desiredAcceleration = targetSpeed - speed;
	//Step 4: limit it because it's too big
	this->acceleration = desiredAcceleration / desiredAcceleration.magnitude() * maxAcceleration;

}


void Boid::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	boidPosition.draw(renderer, pointColor, size);

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, boidPosition.x, boidPosition.y, boidPosition.x + speed.x / 5, boidPosition.y + speed.y / 5);
}

void Boid::avoidObstacle(std::vector <Object*> props)
{
	for (Object* prop : props) {
		if (this->isAround(prop)) {
			direction = { this->boidPosition, prop->getPosition() };
			direction = -direction / 1;
		}
	}
}

Vector& Boid::getPosition()
{
	return boidPosition;
}

Vector& Boid::getDirection()
{
	return direction;
}


Vector& Boid::getSpeed()
{
	return speed;
}

bool Boid::isAround(Object* area)
{
	double distance = sqrt((area->getPosition().x - this->boidPosition.x)
		* (area->getPosition().x - this->boidPosition.x) +
		(area->getPosition().y - this->boidPosition.y)
		* (area->getPosition().y - this->boidPosition.y));

											//The greater the speed, the greater the speed to check
	return distance <= area->getRadius() +  this->speed.magnitude() / 2;
}


