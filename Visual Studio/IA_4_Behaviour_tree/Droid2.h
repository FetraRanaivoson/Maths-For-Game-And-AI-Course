#pragma once
#include <SDL_rect.h>
#include "../lib_Point/Point.h"
#include <random>
#include <cstdlib>
#include <time.h>
#include "Node2.h"
#include "IActor.h"

class Droid : IActor
{
private:
	Point position;
	Color color;
	double alpha = 1;
	double speed = .2;

	Uint32 currentTime = 0;
	Uint32 lastUpdate = 0;
	int deltaTime = 50;

	Point randomPlace;

	std::vector<Node*>pathNodes;
	int pathSteps = 0;
	bool hasPathToGo = false;

public:
	Droid(Point position, Color color);
	void draw(SDL_Renderer* renderer);

	Point& getPosition();
	void setPosition(Point newPosition);

	void goTo(Point destination);
	void wander(SDL_Renderer* renderer, int width, int height);

	bool isAtDestinationNode();

	void setPathSteps(int numbOfSteps);
	void setPath(std::vector<Node*>pathNodes);
	void clearPath();

	NodeState action(int idAction) override;
};

