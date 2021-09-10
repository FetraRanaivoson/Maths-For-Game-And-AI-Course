#pragma once
#include "../lib_Point/Point.h"
#include <vector>
#include "Wall.h"

constexpr double NEIGHBOUR_TOLERANCE_RANGE = 10.0;
constexpr double WALL_TOLERANCE = 5.0;

class Knot
{
private:
	Point position;
	double H = 0; // co�t de d�placement de ce noeud vers le noeud d'arriv�e
	double G = 0; //co�t de d�placement du n�ud de d�part vers ce n�ud
	double F = 0; //�valuation de ce noeud

	Knot* predecessor;

public:
	Knot(Point position, Point exit);
	void draw(SDL_Renderer* renderer, Color color, int size);

	double calculateH(Point exit);

	Point getPosition();
	Knot* getPredecessor() const;

	double getH();
	double getG();
	double getF();


	//D�finir le co�t de d�placement du noeud de d�part vers ce noeud
	void setG(double newG);

	//D�finir l'�valuation G + H o� H = co�t de d�placement de ce noeud vers le noeud d'arriv�e
	void setF(double newF);

	//D�finir P: le noeud pr�c�dent � ce noeud
	void setP(Knot* p);


	std::vector<Knot*> getNeighBoursKnots(std::vector<Wall*>walls);
	bool IsInsideWall(std::vector<Point*>& neighbourPositions, int neighbour, std::vector<Wall*>& walls);
};

