#pragma once
#include <vector>
#include "Node.h"
#include "Wall.h"
#include <iostream>
#include <algorithm>
constexpr auto MAX_WALLS_COUNT = 1000;
constexpr auto MIN_WALL_WIDTH = 8;
constexpr auto MIN_WALL_HEIGHT = 8;
constexpr auto MAX_WALL_WIDTH = 16;
constexpr auto MAX_WALL_HEIGHT = 16;

class Labyrinth
{
private:
	Node* entryKnot;
	static Node* exitKnot;
	std::vector <Wall*> walls;

	std::vector <Node*> closedList; //noeuds déjà visités y compris le départ
	std::vector <Node*> openList; //noeuds à traiter: voisins de la liste fermée

	double getDistance(Point start, Point end);
	void FindN(double& minF, Node*& N, int& minIndex);
	void displayPath(Node*& N, SDL_Renderer* renderer);
	void findNeighboursOfN(std::vector<Node*>& neighboursOfN, Node* N, SDL_Renderer* renderer);
	void updateNeighboursOfNHGFP(Node* V, Node* N);

	int width, height;

	void createEntryKnot();
	void createExitKnot();
	void createWalls();

	bool pathFound = false;

	bool knotInsideWall(Node* knot, std::vector<Wall*>& walls);
	bool isAtExitPoint(Node* N);

public:
	Labyrinth(int width, int height);
	void draw(SDL_Renderer* renderer);
	//The A* algorithm
	void findShortestPath(SDL_Renderer* renderer); 
	bool isPathFound();
	void reset();

	static Node* getExitKnot();

};

