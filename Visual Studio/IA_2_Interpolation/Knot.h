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
	double H = 0; // coût de déplacement de ce noeud vers le noeud d'arrivée
	double G = 0; //coût de déplacement du nœud de départ vers ce nœud
	double F = 0; //évaluation de ce noeud

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


	//Définir le coût de déplacement du noeud de départ vers ce noeud
	void setG(double newG);

	//Définir l'évaluation G + H oû H = coût de déplacement de ce noeud vers le noeud d'arrivée
	void setF(double newF);

	//Définir P: le noeud précédent à ce noeud
	void setP(Knot* p);


	std::vector<Knot*> getNeighBoursKnots(std::vector<Wall*>walls);
	bool IsInsideWall(std::vector<Point*>& neighbourPositions, int neighbour, std::vector<Wall*>& walls);
};

