#pragma once

#include "../lib_Point/Point.h"


class Circle {
private:
	Point center;
	double radius;

public:
	Circle(Point center, double radius);
	void draw(SDL_Renderer* renderer, Color color, int nbPoints);
	void drawWithSqrt(SDL_Renderer* renderer, Color color);
	void upadte(SDL_Event& event);
	bool contains(Point p);
	void setRadius(double radius);

};
