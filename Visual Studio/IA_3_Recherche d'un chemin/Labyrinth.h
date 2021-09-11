#pragma once
#include <vector>
#include "Knot.h"
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
	Knot* entryKnot;
	static Knot* exitKnot;
	std::vector <Wall*> walls;

	std::vector <Knot*> closedList; //noeuds déjà visités y compris le départ
	std::vector <Knot*> openList; //noeuds à traiter: voisins de la liste fermée

	double getDistance(Point start, Point end);
	void FindN(double& minF, Knot*& N, int& minIndex);
	void displayPath(Knot*& N, SDL_Renderer* renderer);
	void findNeighboursOfN(std::vector<Knot*>& neighboursOfN, Knot* N, SDL_Renderer* renderer);
	void updateNeighboursOfNHGFP(Knot* V, Knot* N);

	int width, height;

	void createEntryKnot();
	void createExitKnot();
	void createWalls();

	bool pathFound = false;

	bool knotInsideWall(Knot* knot, std::vector<Wall*>& walls);
	bool isAtExitPoint(Knot* N);

public:
	Labyrinth(int width, int height);
	void draw(SDL_Renderer* renderer);
	//The A* algorithm
	void findShortestPath(SDL_Renderer* renderer); 
	bool isPathFound();
	void reset();

	static Knot* getExitKnot();

};

