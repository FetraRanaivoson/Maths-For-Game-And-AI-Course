#pragma once
#include <SDL_rect.h>
#include "../lib_Point/Point.h"
#include <random>
#include <cstdlib>
#include <time.h>
#include "Node2.h"
#include "IActor.h"



class Droid : public IActor
{
private:
	Point position;
	Color color;
	double alpha = 1;
	double speed = .2;
	double size = 10;

	Uint32 currentTime = 0;
	Uint32 lastUpdate = 0;
	int deltaTime = 150;

	Point randomPlace;
	Uint32 currentTimeRandomPointCreation = 0;
	Uint32 lastTimeRandomPointSpawned= 0;
	int timeToSpawnRandomPoint = 2000; //ms

	std::vector<Node*>pathNodes;
	int pathSteps = 0;
	bool hasPathToGo = false;

	double awarenessRange = 50;
	Point resourcePointFound;

	bool arrivedAtTarget = false;
	bool refreshPath = false;

	//bool checkResource = true;

public:
	Droid(Point position, Color color);
	void draw(SDL_Renderer* renderer);
	Point& getPosition();
	
	void setPosition(Point newPosition);
	void incrementSize(double increment);

	void goTo(Point start, Point destination);
	void update();
	void moveToPosition(int posX, int posY);

	bool isAtDestinationNode();

	void setPathSteps(int numbOfSteps);
	void setPath(std::vector<Node*>pathNodes);
	void clearPath();

	NodeState action(int idAction) override; //event handler

	void setAwarenessRange(double newAwarenessRange);

	bool foundResource();
};

