#include "Cloud.h"

#include <stdlib.h>


//	*************
//	set of points
//	*************

//	constructeur
Cloud::Cloud(int nbPoints, int width, int height) {
	for (int i = 0; i < nbPoints; i++) {
		addPoint(Point(rand() % width, rand() % height, true));
	}
}

//	specific methods

//	add a point to the cloud (at the end)
void Cloud::addPoint(const Point& p) {
	if (nbPoints < MAX_POINTS) {
		points[nbPoints++] = p;
	}
}

//	add a point to the cloud (to a given index)
void Cloud::addPoint(const Point& p, int indice) {
	if (nbPoints < MAX_POINTS) {
		for (int i = nbPoints; i > indice; i--) {
			points[i] = points[i - 1];
		}

		points[indice] = p;
		nbPoints++;
	}
}

//	compute closest point of the cloud from a given point
int Cloud::closestPoint(const Point& p) const {
	int minIndex = -1;

	if (nbPoints > 0) {
		double dMin = p.sqrDist(points[0]); //initialize the min distance
		minIndex = 0;

		for (int i = 1; i < nbPoints; i++) {
			double d = p.sqrDist(points[i]); //Calculer la distance par rapport au point p qu'on manipule

			if (d < dMin) {
				dMin = d;   //le nouveau distance le plus proche de p
				minIndex = i; //son index
			}
		}
	}

	return minIndex; //on retourne son index
}








void Cloud::update(SDL_Event &evt) {
	for (int i = 0; i < nbPoints; i++) {
		points[i].update(evt);
	}
}

//	print the cloud of points
//	if the boolean is true, draw lines between vertex 
void Cloud::draw(SDL_Renderer* renderer, Color c, bool traceSegments, SDL_Event *evt) {
	if (evt != NULL)
		update(*evt);

	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

	if (traceSegments && nbPoints >= 2) {
		SDL_Point* tabPoints = new SDL_Point[(long long)nbPoints + 1];

		for (int i = 0; i < nbPoints; i++) {
			tabPoints[i].x = (int)points[i].x;
			tabPoints[i].y = (int)points[i].y;
		}
		tabPoints[nbPoints].x = (int)points[0].x;
		tabPoints[nbPoints].y = (int)points[0].y;

		SDL_RenderDrawLines(renderer, tabPoints, nbPoints + 1);

		delete[] tabPoints;	//	free memory
	}

	for (int i = 0; i < nbPoints; i++) {
		points[i].draw(renderer, c, 5);
	}
}

//	compute isobarycenter of all the points in the cloud
Point Cloud::computeIsobarycenter() const {
	Point isobarycentre(0, 0);

	for (int i = 0; i < nbPoints; i++) {
		isobarycentre.x += points[i].x;
		isobarycentre.y += points[i].y;
	}

	isobarycentre.x /= nbPoints;
	isobarycentre.y /= nbPoints;

	return isobarycentre;
}

//	compute weither a given point is inside the cloud
bool Cloud::contains(const Point& p) const {
	bool firstSegment = true;
	double signe = 0;
	bool fin = false;

	//	go through every edge
	int i = 0;
	while (!fin && i < nbPoints) {
		//	compute cross product
		Vector segment(points[i], points[(i + 1) % nbPoints]);
		Vector v(points[i], p);
		double prodVect = segment ^ v;

		//	if cross product changes sign then we're outside
		if (firstSegment) {
			signe = prodVect;
			firstSegment = false;
		}
		else if (prodVect*signe < 0)	//	change of sign
			fin = true;	//	exiting loop

		i++;	//	next edge
	}

	return fin == false;
}

//	remove a point from the cloud
void Cloud::removePoint(int indice) {
	for (int i = indice; i < nbPoints - 1; i++) {
		points[i] = points[i + 1];
	}
	nbPoints--;
}