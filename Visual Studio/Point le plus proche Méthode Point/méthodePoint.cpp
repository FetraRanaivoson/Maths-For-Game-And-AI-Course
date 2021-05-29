#include <iostream>
using namespace std;

#include <SDL.h>

//	size of window on screen
constexpr auto WIDTH = 600;
constexpr auto HEIGHT = 600;

//	include desired header files for libraries
#include "../lib_Slider/Slider.h"
#include "../lib_Point/Point.h"

//Le nuage de points
#define MAX_POINTS 50
Point points[MAX_POINTS];

//Mon point à tester
Point myPoint(0, 0, true);

void placePointCloud() { //Placer les points hors de la boucle avant de faire points[i].draw(...) dans la boucle
	srand((unsigned int)time(NULL));
	for (int i = 0; i < MAX_POINTS; i++) {
		points[i].x = rand() % WIDTH;
		points[i].y = rand() % HEIGHT;
	}
	myPoint.x = rand() % WIDTH;
	myPoint.y = rand() % HEIGHT;
}

void drawPointCloud(SDL_Renderer* renderer) {
	for (int i = 0; i < MAX_POINTS; i++) {
		points[i].draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), 5);
		//points[0].x = 15;
		//points[0].y = 25; // rand() % 1000;
	}
}

void drawMyPoint(SDL_Renderer* renderer) {
	myPoint.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 15);
}

void updatePointCloud(SDL_Event event) {
	for (int i = 0; i < 100; i++) {
		points[i].update(event);
	}
}

double sqrDistance(Point a, Point b) {
	double sqrDistance = 0;
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	sqrDistance = dx * dx + dy * dy;
	return sqrDistance;
}

int nearestPointIndex() {
	double minDistance = sqrDistance(points[0], myPoint);
	int minIndex = -1;
	int currentIndex = 0;
	for (Point pointToTest : points)
	{
		double pointToTestDistance = sqrDistance(pointToTest, myPoint);
		if ( pointToTestDistance < minDistance) {
			minDistance = pointToTestDistance;
			minIndex = currentIndex;
		}
		currentIndex += 1;
	}
	return minIndex;
}

void updateNearestPoint(int index, SDL_Renderer* renderer) {
	Point nearestPoint = points[index];
	nearestPoint.drawCircle(renderer, 25, Color(255, 0, 0, SDL_ALPHA_OPAQUE), true);
	SDL_RenderDrawLine(renderer, myPoint.x, myPoint.y, nearestPoint.x, nearestPoint.y);
}

int farthestPointIndex() {
	double maxDistance = sqrDistance(points[0], myPoint);
	int maxIndex = -1;
	int currentIndex = 0;
	for (Point pointToTest : points) {
		double pointToTestDistance = sqrDistance(pointToTest, myPoint);
		if (pointToTestDistance > maxDistance) {
			maxDistance = pointToTestDistance;
			maxIndex = currentIndex;
		}
		currentIndex += 1;
	}
	return maxIndex;
}

void updateFarthestPoint(int index, SDL_Renderer* renderer) {
	Point farthestPoint = points[index];
	farthestPoint.drawCircle(renderer, 25, Color(0, 0, 255, SDL_ALPHA_OPAQUE), true);
	SDL_RenderDrawLine(renderer, myPoint.x, myPoint.y, farthestPoint.x, farthestPoint.y);
}


void start(SDL_Renderer* renderer) {
	bool end = false;
	while (!end) {
		//	paint window in black (clear)
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	- draw any desired graphical objects here
		drawMyPoint(renderer);
		drawPointCloud(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

		//	****************  //
		//	event management  //
		//	****************  //

		updateNearestPoint(nearestPointIndex(), renderer);
		updateFarthestPoint(farthestPointIndex(), renderer);

		//	- remove next event from queue
		SDL_Event event;
		SDL_PollEvent(&event);

		//	- give event to objects for update if needed
		updatePointCloud(event);
		myPoint.update(event);

		//	*********************  //
		//	show rendering buffer  //
		//	*********************  //
		SDL_RenderPresent(renderer);
	}
}

//	entry point of application
int main(int argc, char** argv) {
	
#pragma region SDL initialization
	// SDL initialization
	SDL_Window* fenetre = NULL;
	SDL_Renderer* renderer = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "ERREUR : can't initialize SDL!" << endl;
		exit(0);
	}

	//SDL_ShowCursor(SDL_DISABLE);	//	hide mouse cursor
	SDL_ShowCursor(SDL_ENABLE);	//	show mouse cursor

	//	create the window and its associated renderer
	fenetre = SDL_CreateWindow("SDL template", 200, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(fenetre, 0, 0);
#pragma endregion
	placePointCloud();
	start(renderer);

#pragma region SDL quit
	//	destroy window and quit SDL
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
#pragma endregion

	return 0;
}

