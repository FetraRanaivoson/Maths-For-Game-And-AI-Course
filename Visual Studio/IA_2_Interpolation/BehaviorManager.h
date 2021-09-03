#pragma once
#include <vector>
#include "Object.h"

class BehaviorManager
{
private:

	//std::vector <Boid*> boids;
	Point sumOfPositions;
	Point meanPointCohesion;


public:

	BehaviorManager();
	void aware();//Put this inside the main loop

	Point& getMeanPointCohesion();




};

