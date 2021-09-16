#pragma once
#include <vector>
#include "Node2.h"
#include "Wall2.h"
#include <iostream>
#include <algorithm>
#include "Droid2.h"
constexpr auto MAX_WALLS_COUNT = 200; //1000
constexpr auto MIN_WALL_WIDTH = 8;
constexpr auto MIN_WALL_HEIGHT = 8;
constexpr auto MAX_WALL_WIDTH = 16;
constexpr auto MAX_WALL_HEIGHT = 16;

class Labyrinth
{
private:
	Droid* droid;
	Node* entryKnot;
	static Node* exitKnot;
	std::vector <Wall*> walls;

	std::vector <Node*> closedList; //noeuds déjà visités y compris le départ
	std::vector <Node*> openList; //noeuds à traiter: voisins de la liste fermée

	double getDistance(Point start, Point end);
	void FindN(double& minF, Node*& N, int& minIndex);
	std::vector <Node*> getPath(Node*& N, SDL_Renderer* renderer);
	void findNeighboursOfN(std::vector<Node*>& neighboursOfN, Node* N, SDL_Renderer* renderer);
	void updateNeighboursOfNHGFP(Node* V, Node* N);

	int width, height;

	void createEntryKnot();
	void createWalls();

	bool pathFound = false;
	std::vector<Node*>pathNodes;

	bool isAtExitPoint(Node* N);
	bool isAtExitPoint(Node* N, Node* end);

	bool runningAstar = false;

public:
	void createExitKnot();
	Labyrinth(int width, int height);
	~Labyrinth();
	void draw(SDL_Renderer* renderer);
	//The A* algorithm
	void findShortestPath(SDL_Renderer* renderer); 
	void findShortestPath(Node* start, Node* end, SDL_Renderer* renderer);
	bool isPathFound();
	void reset();
	static Node*& getExitNode();
	Node* getEntryNode();


	void addDroid(Droid* droid);
	Droid* getDroid();

	std::vector<Node*>getPathNodes();

	bool nodeInsideWall(Node* knot, std::vector<Wall*>& walls);
	bool exitNodeInsideWall();

};

