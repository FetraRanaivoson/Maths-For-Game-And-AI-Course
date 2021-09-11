#pragma once
#include <vector>
#include "Object.h"
#include "Boid.h"

class BehaviorManager
{
private:

	Point sumOfPositions;
	Point meanPointCohesion;


public:
	static std::vector <Boid*> boids;
	BehaviorManager();
	void addBoids(std::vector <Boid*> boids);


	void aware();//Put this inside the main loop

	Point& getMeanPointCohesion();




};

