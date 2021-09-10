#pragma once
#include <vector>
#include "Knot.h"
#include "Wall.h"
#include <iostream>
#include <algorithm>
constexpr auto MAX_KNOTS = 20;
constexpr auto MAX_WALLS_COUNT = 150;
constexpr auto MIN_WALL_WIDTH = 5;
constexpr auto MIN_WALL_HEIGHT = 5;
constexpr auto MAX_WALL_WIDTH = 10;
constexpr auto MAX_WALL_HEIGHT = 10;

class Labyrinth
{
private:
	Knot* entryKnot;
	static Knot* exitKnot;
	std::vector <Wall*> walls;

	std::vector <Knot*> closedList; //noeuds déjà visités y compris le départ
	std::vector <Knot*> openList; //noeuds à traiter: voisins de la liste fermée

	double getDistance(Point start, Point end);

public:
	Labyrinth(int width, int height);
	void createWalls(int width, int height);
	void draw(SDL_Renderer* renderer);

	static Knot* getExitKnot();

	void findNearestPath(SDL_Renderer* renderer);

};

