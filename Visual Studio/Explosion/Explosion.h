#pragma once

#include "../lib_Point/Point.h"

class Explosion
{
private:
	Point position;
	double radius; //d: Rayon de propagation de la force de l'�picentre
	double �picentre;  //Fo: force au niveau de l'�picentre en Newton
	double lossFactor; //k: force de d�perdition de la puissance de l'explosion

	double mass; //en kg

	double speed;
	double acceleration;


	__time64_t lastUpdate;	//	date in nanoseconds since last update
	__time64_t getTimeInNanoSeconds();

public:
	Explosion(Point position, double �picentre, double lossFactor);

	void draw(SDL_Renderer* renderer, Color color, SDL_Event& event);

	void update(SDL_Event& event);

	double getMagnitude();
	Point getPosition();
	double& getRadius();

	double computeFriction();
};

