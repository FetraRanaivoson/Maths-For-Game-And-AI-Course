#include "Droid3.h"
#include "Labyrinth2.h"

Droid3::Droid3(Point position, Color color, int windowWidth, int windowHeight)
	:position(position), color(color), windowWidth(windowWidth), windowHeight(windowHeight)
{
	this->position.x = ((int)(this->position.x / 8)) * 8.0;
	this->position.y = ((int)(this->position.y / 8)) * 8.0;

	this->randomPoint = { rand() % windowWidth + 0.0, rand() % windowHeight + 0.0 };
}

void Droid3::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, this->color.a);
	SDL_Rect rect = { this->position.x, this->position.y, this->size, this->size };
	SDL_RenderFillRect(renderer, &rect);
	this->position.drawCircle(renderer, awarenessRange, Color(255, 0, 0, SDL_ALPHA_OPAQUE), true);

	this->currentRandomPointCreationTime = SDL_GetTicks();
	if (this->currentRandomPointCreationTime - this->lastTimeRandomPointUpdated > this->timeToUpdateRandomPoint) {
		this->randomPoint = { rand() % windowWidth + 0.0, rand() % windowHeight + 0.0 };
		this->lastTimeRandomPointUpdated = this->currentRandomPointCreationTime;
	}
	this->randomPoint.draw(renderer, Color(255, 255, 255, .5), 10);
}

void Droid3::goTo(Point destination)
{
	Vector dest(destination.x, destination.y);
	Vector st(this->position.x, this->position.y);
	Vector direction = dest.operator-(st);
	direction.normalize();

	this->currentTime = SDL_GetTicks();
	this->position.x += direction.x * this->speed * (this->currentTime - this->lastUpdate) / 1000;
	this->position.y += direction.y * this->speed * (this->currentTime - this->lastUpdate) / 1000;
	this->lastUpdate = this->currentTime;

	double epsilon = .1;
	if (abs(this->position.x - destination.x) < epsilon && abs(this->position.y - destination.y) < epsilon) {
		this->position = { destination.x + epsilon, destination.y + epsilon };
	}

}

NodeState Droid3::action(int idAction, SDL_Renderer* renderer)
{
	if (idAction == Action::checkResourcesInRange) {
		if (this->foundResource(renderer)) {
			std::cout << "Droid found nearby resource" << std::endl;
			return NodeState::success;
		}
		return NodeState::failed;
	}
	else if (idAction == Action::goToResource) {
	/*	this->pathNodes = Labyrinth::getPathNodes(this->position, this->resourcePointFound);
		if (this->pathNodes.size() > 0 && this->currentStep != -1) {
			this->currentStep = this->pathNodes.size() - 1;
			this->goTo(pathNodes[currentStep]->getPosition());
			double epsilon = 1.1;
			if (abs(this->position.x - this->resourcePointFound.x) < epsilon && abs(this->position.y - this->resourcePointFound.y) < epsilon) {
				this->currentStep--;
				if (this->currentStep == -1) {
					this->pathNodes.clear();
					return NodeState::success;
				}
				return NodeState::success;
			}
			return NodeState::success;
		}
		return NodeState::success;*/
		this->goTo(this->resourcePointFound);
		return NodeState::success;//TODO: SUCCES WHEN ARRIVED //ELSE IN PROGRESS
	}
	else if (idAction == Action::wander) {
		this->goTo(randomPoint);
		return NodeState::success;
	}
	return NodeState::progress;
}

Point& Droid3::getPosition()
{
	return this->position;
}

void Droid3::incrementAwarenessRange(double value)
{
	this->awarenessRange += value;
}

bool Droid3::foundResource(SDL_Renderer* renderer) {
	for (int i = 0; i < Labyrinth::resources.size(); i++) {
		double distance = Labyrinth::resources[i].sqrDist(this->position);
		if (distance < 1) {
			Labyrinth::resources.erase(Labyrinth::resources.cbegin() + i);
			this->incrementAwarenessRange(1);
		}
		else if (distance <= this->awarenessRange * this->awarenessRange) {
			this->resourcePointFound = Labyrinth::resources[i];
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 1);
			SDL_RenderDrawLine(renderer, this->position.x, this->position.y,
				this->resourcePointFound.x, this->resourcePointFound.y);
			return true;
		}
	}
	return false;
}
