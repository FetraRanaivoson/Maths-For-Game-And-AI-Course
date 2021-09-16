#include "Droid2.h"
#include <iostream>

Droid::Droid(Point position, Color color)
	: color(color)
{
	this->position.x = position.x;
	this->position.y = position.y;
}

void Droid::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, this->color.a);
	SDL_Rect rect = { this->position.x, this->position.y, 10, 10 };
	SDL_RenderFillRect(renderer, &rect);
}

Point& Droid::getPosition()
{
	return this->position;
}

void Droid::setPosition(Point newPosition)
{
	this->position = newPosition;
}

void Droid::goTo(Point destination)
{
	this->currentTime = SDL_GetTicks();
	if (this->currentTime - this->lastUpdate > this->deltaTime) {
		Vector droidPosition(this->position.x, this->position.y);
		Vector destinationPosition(destination.x, destination.y);
		droidPosition = (1 - this->alpha) * droidPosition + this->alpha * destinationPosition;
		this->position.x = droidPosition.x;
		this->position.y = droidPosition.y;
		this->lastUpdate = this->currentTime;
	}
}

void Droid::wander(SDL_Renderer* renderer, int width, int height)
{
	/*randomPlace = { rand() % width + 0.0, rand() % height + 0.0 };
	randomPlace.draw(renderer, Color(255, 0, 0, 1), 10);
	this->goTo(randomPlace);*/
	/*if (this->droid.x == randomPlace.x && this->droid.y == randomPlace.y) {

	}*/

	if (this->pathNodes.size() > 0 && this->pathSteps != -1) {
		
		Point destination = Point(this->pathNodes[this->pathSteps]->getPosition().x, this->pathNodes[this->pathSteps]->getPosition().y);
		this->goTo(destination);
		double epsilon = 0.1;
		if ( abs(this->position.x - destination.x) < epsilon && abs(this->position.y - destination.y) < epsilon) {
			this->pathSteps--;
			std::cout << "Searching" << std::endl;
		}

		if (this->pathSteps == -1) {
			this->pathNodes.clear();
			std::cout << "Found" << std::endl;
		}

	}
	else {
		//std::cout<<"Stop" << std::endl;
	}
}

bool Droid::isAtDestinationNode()
{
	return this->position.x == this->pathNodes[0]->getPosition().x && this->position.y == this->pathNodes[0]->getPosition().y;
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
	this->pathSteps = this->pathNodes.size() - 1;
}

void Droid::clearPath()
{
	this->pathNodes.clear();
}

NodeState Droid::action(int idAction)
{
	return NodeState::failed;
}

