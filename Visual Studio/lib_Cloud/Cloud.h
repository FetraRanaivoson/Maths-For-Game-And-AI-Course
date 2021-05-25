#pragma once

#include <SDL.h>
#include "../lib_Point/Point.h"

#define MAX_POINTS 10000

class Cloud {
public:
	Point points[MAX_POINTS];
	int nbPoints = 0;

	Cloud(int nbPoints, int width, int height);

	void addPoint(const Point& p);
	void addPoint(const Point& p, int indice);
	int closestPoint(const Point& p) const;
	void update(SDL_Event &evt);
	void draw(SDL_Renderer* renderer, Color c, bool traceSegments, SDL_Event *evt = NULL);

	Point computeIsobarycenter() const;
	bool contains(const Point& p) const;
	void removePoint(int indice);
};
