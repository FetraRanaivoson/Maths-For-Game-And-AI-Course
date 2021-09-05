#include "BehaviorManager.h"

std::vector <Boid*> BehaviorManager::boids;

BehaviorManager::BehaviorManager()	
{
}

void BehaviorManager::addBoids(std::vector<Boid*> boids)
{
	this->boids = boids;
}

void BehaviorManager::aware()
{
	//for (Boid* boid : boids) {
	//	sumOfPositions.operator+=(boid->getPosition());
	//}
	//std::cout << boids[0]->getPosition().x << std::endl;
}

Point& BehaviorManager::getMeanPointCohesion()
{
	/*meanPointCohesion = { sumOfPositions.x / boids.size(), sumOfPositions.x / boids.size() };
	return meanPointCohesion;*/
	Point p{ 0,0 };
	return p;
}



