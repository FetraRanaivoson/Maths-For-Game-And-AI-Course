#pragma once
#include "../lib_Point/Point.h"
#include <vector>
#include "Wall.h"

constexpr double NEIGHBOUR_TOLERANCE_RANGE = 8.0; //The greater the more precise but took longer to calculate
constexpr double WALL_TOLERANCE = 1.0;

class Node
{
private:
	Point position;
	double H = 0; // coût de déplacement de ce noeud vers le noeud d'arrivée
	double G = 0; //coût de déplacement du nœud de départ vers ce nœud
	double F = 0; //évaluation de ce noeud

	Node* predecessor;

public:
	Node(Point position, Point exit);
	void draw(SDL_Renderer* renderer, Color color, int size);

	double calculateH(Point exit);

	Point getPosition();
	Node* getPredecessor() const;

	double getH();
	double getG();
	double getF();


	//Définir le coût de déplacement du noeud de départ vers ce noeud
	void setG(double newG);

	//Définir l'évaluation G + H oû H = coût de déplacement de ce noeud vers le noeud d'arrivée
	void setF(double newF);

	//Définir P: le noeud précédent à ce noeud
	void setP(Node* p);

	bool equal(Node* knot);

	std::vector<Node*> getNeighBoursKnots(std::vector<Wall*>walls);
	bool IsInsideWall(std::vector<Point*>& neighbourPositions, int neighbour, std::vector<Wall*>& walls);
};

