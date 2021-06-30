#pragma once
#include "../lib_Point/Point.h"
#include "Explosion.h"


class PointObject
{
private:
	Point position;

	double mass;

	Vector speed;
	Vector acceleration;
	Vector explosionImpact;
	Vector friction;

	__time64_t lastUpdate;	//	date in nanoseconds since last update
	__time64_t getTimeInNanoSeconds();

	bool isAffectedByExplosion(Explosion explosion);

public:
	PointObject(Point position, double mass);

	void listenForExplosion(Explosion explosion);

	void setSpeed(Vector newSpeed);
	void setExplosionImpact(Vector explosionMagnitude);
	Vector getExplosionImpact();

	void draw(SDL_Renderer* renderer, Color color, SDL_Event& event);
	void update(SDL_Event& event);

	Vector computeFriction();

};

