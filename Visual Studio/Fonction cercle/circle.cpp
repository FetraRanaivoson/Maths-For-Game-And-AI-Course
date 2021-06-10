#include "circle.h"


Circle::Circle(Point center, double radius) {
    this->center = center;
    this->radius = radius;
}

void Circle::draw(SDL_Renderer* renderer, Color color, int nbPoints)
{
	this->center.draw(renderer, color, 5);
	double deltaAlpha = 360.0 / nbPoints;
	for (double alpha = 0; alpha < 360; alpha += deltaAlpha) {
		Point p(this->center.x + this ->radius * cos(alpha * M_PI / 180),
			this->center.y - this -> radius* sin(alpha * M_PI / 180));
		p.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 1);
	}
}

void Circle::drawWithSqrt(SDL_Renderer* renderer, Color color) {
	this->center.draw(renderer, color, 5);

	for (int x = this->center.x - this->radius; x <= this->center.x + this->radius; x++) {
		double y = this->center.y + sqrt(this->radius * this->radius - (x - this->center.x) * (x - this->center.x));
		Point pBottom(x, y);
		pBottom.draw(renderer, color, 1);
		y = this->center.y - sqrt(this->radius * this->radius - (x - this->center.x) * (x - this->center.x));
		Point pTop(x, y);
		pTop.draw(renderer, color, 1);
	}
	
}

void Circle::upadte(SDL_Event& event)
{
	this->center.update(event);
}


bool Circle::contains(Point p)
{
	double d = sqrt((p.x - this->center.x)*(p.x - this->center.x) +
		(p.y - this->center.y)*(p.y - this->center.y));
	
	return d <= this->radius;
}

void Circle::setRadius(double radius)
{
	this->radius = radius;
}
