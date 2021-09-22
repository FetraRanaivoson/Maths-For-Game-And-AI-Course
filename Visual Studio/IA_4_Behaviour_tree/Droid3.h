#pragma once
#include <SDL_rect.h>
#include "../lib_Point/Point.h"
#include <random>
#include <cstdlib>
#include <time.h>
#include "Node2.h"
#include "IActor.h"

class Droid3 : public IActor
{
private:
	Point position;
	Color color;
	double size = 10;
	double awarenessRange = 50;

	double speed = 100; //px per second
	Uint32 currentTime = 0;
	Uint32 lastUpdate = 0;

	bool foundResource(SDL_Renderer* renderer);
	Point resourcePointFound;
	std::vector<Node*> pathNodes;
	int currentStep = -1;

	int windowWidth, windowHeight;

	Point randomPoint;
	Uint32 currentRandomPointCreationTime = 0;
	Uint32 lastTimeRandomPointUpdated= 0;
	int timeToUpdateRandomPoint = 6000; //ms


public:
	Droid3(Point position, Color color, int windowWidth,int windowHeight);
	void draw(SDL_Renderer* renderer);

	void goTo(Point destination);

	NodeState action(int idAction, SDL_Renderer* renderer) override; //event handler

	//Getters
	Point& getPosition();

	void incrementAwarenessRange(double value);
};

