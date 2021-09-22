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
	static Droid* droid;
	static Node* entryNode;
	static Node* exitNode;
	static Node* randomExitPointNode;
	static std::vector <Wall*> walls;

	static std::vector <Node*> closedList; //noeuds déjà visités y compris le départ
	static std::vector <Node*> openList; //noeuds à traiter: voisins de la liste fermée

	static double getDistance(Point start, Point end);
	static void FindN(double& minF, Node*& N, int& minIndex);
	static std::vector <Node*> getPath(Node*& N);
	static void findNeighboursOfN(std::vector<Node*>& neighboursOfN, Node* N);
	static void updateNeighboursOfNHGFP(Node* V, Node* N);

	static int width, height;

	void createEntryKnot();
	void createWalls();

	static bool pathFound;
	static std::vector<Node*>pathNodes;

	static bool isAtExitPoint(Node* N);
	static bool isAtExitPoint(Node* N, Node* end);

	bool runningAstar = false;

	Color goldColor;
	Uint32 currentColorChangeTime = 0;
	Uint32 lastTimeColorChanged = 0;
	int timeToUpdateColor = 50; //ms


public:
	Labyrinth(int width, int height);
	~Labyrinth();

	static std::vector<Point> resources;
	
	static void createExitNode();
	static void createRandomExitPointNode();
	

	void draw(SDL_Renderer* renderer);
	
	//The A* algorithm
	void findShortestPath(SDL_Renderer* renderer); 
	static std::vector<Node*> getPathNodes(Point start, Point end);
	bool isPathFound();
	
	void reset();

	static Node*& getExitNode();
	static Node*& getEntryNode();
	static Node*& getRandomExitPointNode();


	void addDroid(Droid* droid);
	static Droid* getDroid();

	void addResources(std::vector<Point> resources);

	static std::vector<Node*>getPathNodes();

	static bool nodeInsideWall(Node* knot, std::vector<Wall*>& walls);
	static bool exitNodeInsideWall();
	static bool randomExitNodeInsideWall();

};

