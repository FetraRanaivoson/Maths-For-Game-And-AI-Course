#include "Boid.h"
#include "BehaviorManager.h"


int Boid::nbBoids = 0;

std::vector<Boid*> Boid::getBoidsInRange(double radius)
{
	std::vector<Boid*> foundedBoids;
	for (Boid* boidToTest : BehaviorManager::boids) {
		if (boidToTest != this) {
			Vector thisBoidToNeighbour = { this->boidPosition, boidToTest->getPosition() };
			if (thisBoidToNeighbour.magnitude() <= radius) {
				foundedBoids.push_back(boidToTest);
			}
		}
	}
	return foundedBoids;
}
Vector Boid::getCohesionVector()
{
	std::vector<Boid*>neighbourBoids = this->getBoidsInRange(this->attractionRadiusRange);
	if (neighbourBoids.size() > 0) {
		for (Boid* boid : neighbourBoids) {
			this->sumOfPositions.operator+=(boid->getPosition());
		}
		Vector cohesionVector = { (this->sumOfPositions.x),(this->sumOfPositions.y) };
		return cohesionVector / neighbourBoids.size();
	}
	return Vector(0, 0);
}

void Boid::setCohesionWeight(double newCohesionWeight)
{
	this->cohesionWeight = newCohesionWeight;
}

//Vector Boid::getCohesionDirection(double cohesionCoef)
//{
//	return Vector(boidPosition, getCohesionPoint());
//}


Vector Boid::getAlignementVector()
{
	std::vector<Boid*>neighbourBoids = this->getBoidsInRange(this->alignmentRadiusRange);
	if (neighbourBoids.size() > 0) {
		for (Boid* boid : neighbourBoids) {
			this->sumOfSpeeds.operator+=(boid->getSpeed());
		}
		Vector alignmentVector = { (this->sumOfSpeeds.x),(this->sumOfSpeeds.y) };
		return alignmentVector / neighbourBoids.size();
	}
	return Vector(0, 0);
}

void Boid::setAlignmentWeight(double newAlignmentWeight)
{
	this->alignmentWeight = newAlignmentWeight;
}

//Vector Boid::getRepulsionVector()
//{
//	std::vector<Boid*>neighbourBoids = this->getBoidsInRange(this->repulsionRadiusRange);
//	if (neighbourBoids.size() > 0) {
//		for (Boid* boid : neighbourBoids) {
//			this->sumOfVectorDistance.operator+=(Vector(this->boidPosition, boid->getPosition()));
//		}
//		Vector repulsionVector = { (this->sumOfVectorDistance.x),(this->sumOfVectorDistance.y) };
//		return repulsionVector / neighbourBoids.size();
//	}
//	return Vector(0, 0);
//}

void Boid::setSeparationWeight(double newSeparationWeight)
{
	this->separationWeight = newSeparationWeight;
}


void Boid::lerp(Vector initialPosition, Vector destinationPosition, double alpha)
{
	if (alpha >= 0 && alpha <= 1) {
		boidPosition = (1 - alpha) * initialPosition + alpha * destinationPosition;
	}
}

Boid::Boid(Vector position, int size, double alpha, double maxSpeed, double maxAcceleration, Color pointColor)
	: boidPosition(position), pointColor(pointColor), speed(0, 0), acceleration(0, 0), /*direction(0, 0),*/
	/*targetAcceleration(0, 0),*/ cohesionAcceleration(0, 0), alignmentAcceleration(0, 0), separationAcceleration(0, 0),
	targetSpeed(0, 0), /*cohesionSpeed(0, 0), alignmentSpeed(0, 0), separationSpeed(0, 0), */
	sumOfSpeeds(0, 0), sumOfVectorDistance(0, 0) /*repulsionVector(0,0)*/
{
	this->size = size;
	this->alpha = alpha;

	this->maxSpeed = maxSpeed;
	this->maxAcceleration = maxAcceleration;

	//this->maxSpeed = 300;
	//this->maxAcceleration = 175;

	Boid::nbBoids++;
}

void Boid::setTargetWeight(double newTargetWeight)
{
	this->targetWeight = newTargetWeight;
}



void Boid::followSimple(SDL_Renderer* renderer, int clickPosX, int clickPosY)
{
	Vector destination(clickPosX, clickPosY);
	this->lerp(boidPosition, destination, alpha);
}

void Boid::update()
{
	currentTime = SDL_GetTicks();
	//if (currentTime - latUpdate > deltaTime) {
	boidPosition += speed * (double)(currentTime - latUpdate) / 1000;
	speed += acceleration * (double)(currentTime - latUpdate) / 1000;


	if (speed.magnitude() > maxSpeed) {
		this->speed = this->speed / this->speed.magnitude() * maxSpeed;
	}

	latUpdate = currentTime;

	if (this->boidPosition.x > 1200) {
		this->boidPosition.x -= 1200;
	}
	if (this->boidPosition.y > 700) {
		this->boidPosition.y -= 700;
	}
	if (this->boidPosition.x < 0) {
		this->boidPosition.x += 1200;
	}
	if (this->boidPosition.y < 0) {
		this->boidPosition.y += 700;
	}
	//}
}


Vector Boid::getTargetAcceleration(SDL_Renderer* renderer, Point target, Object* area, std::vector <Object*> props)
{
	//Step 1: get the direction
	Vector direction = { 0,0 };
	/*if (this->isAround(area)) {
		this->direction = { this->boidPosition, area->getPosition() };
		this->direction = -this->direction / 1;
	}*/
	//else {
	direction = { this->boidPosition, target };
	//}

	//Step 1.5: Just in case in front of props
	//this->avoidObstacle(props);

	//Step 2: limit this direction because it's too big
	if (direction.magnitude() != 0) {
		this->targetSpeed = direction / direction.magnitude() * maxSpeed; // =======> vecteur v * L / norme(v)  where L is the new magnitude
	}

	//Step 3: desiredAcceleration = targetSpeed - currentSpeed 
	Vector desiredAcceleration = targetSpeed - this->speed;

	//Step 4: limit it because it's too big
	Vector targetAcceleration = { 0,0 };
	if (desiredAcceleration.magnitude() != 0) {
		targetAcceleration = desiredAcceleration / desiredAcceleration.magnitude() * this->maxAcceleration;
	}
	return targetAcceleration;

}
Vector Boid::getCohesionAcceleration(SDL_Renderer* renderer, Object* area, std::vector<Object*> props)
{
	//Step 1: get the direction
	/*if (this->isAround(area)) {
		this->direction = { this->boidPosition, area->getPosition() };
		this->direction = -direction / 1;
	}*/
	//else {
	std::vector<Boid*>neighbourBoids = this->getBoidsInRange(this->attractionRadiusRange);
	Vector direction = { 0,0 };
	if (neighbourBoids.size() > 0) {
		for (Boid* boid : neighbourBoids) {
			this->sumOfPositions += (boid->getPosition());
		}
		Vector cohesionVector = { (this->sumOfPositions.x),(this->sumOfPositions.y) };
		direction = cohesionVector / neighbourBoids.size();


		//if (neighbourBoids.size() > 0) {
		//	Vector cohesionVector = { (this->sumOfPositions.x),(this->sumOfPositions.y) };
		//	this->direction = Vector(this->boidPosition, cohesionVector);
		//	this->direction = this->direction / neighbourBoids.size();
		//}
		//}
		//Step 1.5: Just in case in front of props
		//this->avoidObstacle(props);

		//Step 2: 
		//if (this->direction.magnitude() != 0) {
		Vector cohesionSpeed = direction;
		if (direction.magnitude() != 0) {
			cohesionSpeed *= this->maxSpeed * direction.magnitude(); // =======> vecteur v * L / norme(v)  where L is the new magnitude
		}

		//}
			//Step 3: 
		Vector desiredAcceleration = cohesionSpeed - this->speed;

		//Step 4: limit it because it's too big
		//Vector cohesionAcceleration = { 0,0 };
		if (desiredAcceleration.magnitude() != 0) {
			cohesionAcceleration = desiredAcceleration / desiredAcceleration.magnitude() * this->maxAcceleration;
		}
	}
	return cohesionAcceleration;
}
Vector Boid::getAlignmentAcceleration(SDL_Renderer* renderer, Object* area, std::vector<Object*> props)
{
	//Step 1
	//if (this->isAround(area)) {
	//	this->direction = { this->boidPosition, area->getPosition() };
	//	this->direction = -this->direction / 1;
	//}
	//else {
	Vector direction = { 0,0 };
	std::vector<Boid*>neighbourBoids = this->getBoidsInRange(this->alignmentRadiusRange);
	if (neighbourBoids.size() > 0) {
		for (Boid* boid : neighbourBoids) {
			this->sumOfSpeeds += boid->getSpeed();
		}
		direction = this->sumOfSpeeds / neighbourBoids.size();

		//}
		//Step 1.5: Just in case in front of props
		//this->avoidObstacle(direction, props);

		//Step 2: 
		Vector alignmentSpeed = direction;
		if (direction.magnitude() != 0) {
			alignmentSpeed *= this->maxSpeed / direction.magnitude();
		}

		//Step 3:
		Vector desiredAcceleration = alignmentSpeed - this->speed;

		//Step 4: 
		//Vector alignmentAcceleration = { 0,0 };
		if (desiredAcceleration.magnitude() != 0) {
			alignmentAcceleration = desiredAcceleration / desiredAcceleration.magnitude() * this->maxAcceleration;
		}
	}
	return alignmentAcceleration;
}
Vector Boid::getSeparationAcceleration(SDL_Renderer* renderer, Object* movingObject, std::vector<Object*> props)
{
	//Step 1: get the direction
	//if (this->isAround(movingObject)) {
	//	this->direction = { this->boidPosition, movingObject->getPosition() };
	//	this->direction = -this->direction / 1;
	//}
	//else {
	std::vector<Boid*>neighbourBoids = this->getBoidsInRange(this->repulsionRadiusRange);
	Vector direction = { 0,0 };
	Vector sumOfVectorDistance = { 0,0 };

	if (neighbourBoids.size() > 0) {
		for (Boid* boid : neighbourBoids) {
			sumOfVectorDistance += Vector(this->boidPosition, boid->getPosition());
		}
		direction = sumOfVectorDistance;
		direction = -direction / neighbourBoids.size();


		//Step 1.5: Just in case in front of props
		//this->avoidObstacle(direction, props);

		//Step 2: limit this direction because it's too big
		Vector separationSpeed = direction;
		if (direction.magnitude() != 0) {
			separationSpeed *= this->maxSpeed / direction.magnitude();  // =======> vecteur v * L / norme(v)  where L is the new magnitude
		}

		//Step 3: desiredAcceleration = targetSpeed - currentSpeed 
		Vector desiredAcceleration = separationSpeed - this->speed;

		//Step 4: limit it because it's too big
		if (desiredAcceleration.magnitude() != 0) {
			separationAcceleration = desiredAcceleration / desiredAcceleration.magnitude() * this->maxAcceleration;
		}
	}
	//}
	return separationAcceleration;
}


void Boid::follow(SDL_Renderer* renderer, Point target, Object* area, std::vector<Object*> props)
{

	this->acceleration = this->targetWeight * this->getTargetAcceleration(renderer, target, area, props) +
		this->cohesionWeight * this->getCohesionAcceleration(renderer, area, props) +
		this->alignmentWeight * this->getAlignmentAcceleration(renderer, area, props) +
		this->separationWeight * this->getSeparationAcceleration(renderer, area, props);

	if (this->acceleration.magnitude() != 0)
		this->acceleration = this->acceleration / this->acceleration.magnitude() * maxAcceleration;
}


void Boid::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	boidPosition.draw(renderer, pointColor, size);
	boidPosition.drawCircle(renderer, attractionRadiusRange, Color(120, 100, 203, 255), true);
	boidPosition.drawCircle(renderer, alignmentRadiusRange, Color(10, 120, 10, SDL_ALPHA_OPAQUE), true);
	boidPosition.drawCircle(renderer, repulsionRadiusRange, Color(255, 0, 0, SDL_ALPHA_OPAQUE), true);

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, boidPosition.x, boidPosition.y, boidPosition.x + speed.x / 5, boidPosition.y + speed.y / 5);


}
void Boid::avoidObstacle(Vector direction, std::vector <Object*> props)
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
//Vector& Boid::getDirection()
//{
//	return direction;
//}
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
	return distance <= area->getRadius() + this->speed.magnitude() / 2;
}


