#include "Droid2.h"
#include <iostream>
#include "Labyrinth2.h"
Droid::Droid(Point position, Color color)
	: color(color)
{
	position.x = ((int)(position.x / 8)) * 8.0;
	position.y = ((int)(position.y / 8)) * 8.0;
	this->position = position;
}

void Droid::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, this->color.a);
	SDL_Rect rect = { this->position.x, this->position.y, this->size, this->size };
	SDL_RenderFillRect(renderer, &rect);
	position.drawCircle(renderer, awarenessRange, Color(255, 0, 0, SDL_ALPHA_OPAQUE), true);
}

Point& Droid::getPosition()//
{
	return this->position;
}

void Droid::setPosition(Point newPosition)
{
	this->position = newPosition;
}

void Droid::incrementSize(double increment) {
	this->size += increment;
}

void Droid::goTo(Point start, Point destination)
{
	this->currentTime = SDL_GetTicks();
	//if (this->currentTime - this->lastUpdate > this->deltaTime) {
		//this->alpha += .3;
		//std::cout << this->alpha << std::endl;
	Vector startPosition(start.x, start.y);
	Vector destinationPosition(destination.x, destination.y);
	//if (this->alpha <= 1.0) {
	startPosition = (1 - this->alpha) * startPosition + this->alpha * destinationPosition;
	this->position.x = startPosition.x;
	this->position.y = startPosition.y;
	this->lastUpdate = this->currentTime;
	//}
//}
}

void Droid::update(int posX, int posY)
{
	//if (this->pathIsRefreshed) {
	posX = ((int)(posX * 8.0)) / 8;
	posY = ((int)(posY * 8.0)) / 8;
	this->setPositionToGo(posX, posY);
	//this->pathIsRefreshed = false;
//}

	if (this->pathNodes.size() > 0 && this->pathSteps != -1) {
		this->pathSteps = this->pathNodes.size() - 2;
		Point start = Point(this->pathNodes[(long long)this->pathSteps + 1]->getPosition().x, this->pathNodes[(long long)this->pathSteps + 1]->getPosition().y);
		Point destination = Point(this->pathNodes[this->pathSteps]->getPosition().x, this->pathNodes[this->pathSteps]->getPosition().y);
		this->goTo(start, destination);

		double epsilon = 0.1;
		if (abs(this->position.x - destination.x) < epsilon && abs(this->position.y - destination.y) < epsilon) {
			this->pathSteps--;
			if (this->pathSteps == -1) {
				this->pathNodes.clear();
			}
		}
	}
}

void Droid::setPositionToGo(int posX, int posY) {
	Point exitPosition(posX, posY);
	//Labyrinth::getExitNode()->setPosition(exitPosition);
	std::vector<Node*> pathNodes = Labyrinth::getPathNodes(this->position, exitPosition);
	if (pathNodes.size() > 0)
		this->pathNodes = pathNodes;
		/*if (!Labyrinth::exitNodeInsideWall()) {
			Labyrinth::getPathNodes(Labyrinth::getDroid()->getPosition(), exitPosition);
		}*/
}

//void Droid::moveToPosition(int posX, int posY) {
//	Point exitPosition(posX, posY);
//	if (!Labyrinth::exitNodeInsideWall()) {
//		Labyrinth::executeAstar(Labyrinth::getDroid()->getPosition(), exitPosition);
//	}
//}

bool Droid::isAtDestinationNode()
{
	return this->pathSteps == -1;
}

void Droid::setPathSteps(int numbOfSteps)
{
	this->pathSteps = numbOfSteps;
}

void Droid::setPath(std::vector<Node*> pathNodes)
{
	/*for (Node* node : this->pathNodes) {
		delete node;
	}*/
	this->pathNodes = pathNodes;
	this->pathSteps = this->pathNodes.size() - 2;
}

void Droid::clearPath()
{
	this->pathNodes.clear();
}

NodeState Droid::action(int idAction, SDL_Renderer* renderer)
{
	if (idAction == Action::checkResourcesInRange) {
		if (this->foundResource(renderer)) {
			//std::cout << "Droid found nearby resources" << std::endl;
			return NodeState::success;
		}
		return NodeState::failed;
	}
	else if (idAction == Action::goToResource) {
		if (this->isAtDestinationNode() && !this->pathIsRefreshed) {
			this->arrivedAtTarget = true;
			this->pathIsRefreshed = true;
			//this->awarenessRange += 10;
			this->incrementSize(5);
			return NodeState::success;
		}

		if (!this->arrivedAtTarget) {
			/*if (this->pathIsRefreshed) {
				this->setPositionToGo(this->resourcePointFound.x, this->resourcePointFound.y);
				this->pathIsRefreshed = false;
			}*/
			this->update(this->resourcePointFound.x, this->resourcePointFound.y);
			//std::cout << "Droid is going to nearby resources" << std::endl;
			return NodeState::progress;
		}
	}
	//else if (idAction == Action::wander) {
	/*this->currentTimeRandomPointCreation = SDL_GetTicks();
	if (this->currentTimeRandomPointCreation - this->lastTimeRandomPointSpawned > this->timeToSpawnRandomPoint) {
		Point randomPosition(rand() % 1200, rand() % 800);
		this->setPositionToGo(randomPosition.x, randomPosition.y);
		this->lastTimeRandomPointSpawned = this->currentTimeRandomPointCreation;
		return NodeState::success;
	}*/
	//return NodeState::failed;
//}
	return NodeState::progress;
}

void Droid::setAwarenessRange(double newAwarenessRange)
{
	this->awarenessRange = newAwarenessRange;
}

bool Droid::foundResource(SDL_Renderer* renderer)
{
	//double distance = 10000;
	for (int i = 0; i < Labyrinth::resources.size(); i++) {
		double distance = Labyrinth::resources[i].sqrDist(this->position);

		if (distance < 1) {
			Labyrinth::resources.erase(Labyrinth::resources.cbegin() + i);
		}
		else if (distance <= this->awarenessRange * this->awarenessRange) {
			this->resourcePointFound = Labyrinth::resources[i];

			this->arrivedAtTarget = false;
			this->pathIsRefreshed = true;
			return true;
		}
	}
	return false;
}

