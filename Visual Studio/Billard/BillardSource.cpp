#include <iostream>
using namespace std;

#include <SDL.h>
#include "Queue.h"

constexpr auto MAX_OBJECTS = 10;

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 500, POS_Y = 200;
constexpr auto WIDTH = 600, HEIGHT = 700;
constexpr auto MAX_BALL = 14;


//	include desired header files for libraries
#include <time.h>
#include "Ball.h"
#include "BillardSource.h"

SDL_Renderer* init_SDL(const char* title) {
#pragma region SDL initialization
	// SDL initialization
	SDL_Window* window = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "ERREUR : can't initialize SDL!" << endl;
		exit(0);
	}

	//SDL_ShowCursor(SDL_DISABLE);	//	hide mouse cursor
	SDL_ShowCursor(SDL_ENABLE);	//	show mouse cursor

	//	create the window and its associated renderer
	window = SDL_CreateWindow(title, POS_X, POS_Y, WIDTH, HEIGHT, 0);
	return SDL_CreateRenderer(window, 0, 0);
#pragma endregion
}

void clearWindow(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 30, 80, 35, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void showRenderingBuffer(SDL_Renderer* renderer) {
	SDL_RenderPresent(renderer);
}

SDL_Event getNextEvent() {
	SDL_Event event;
	SDL_PollEvent(&event);

	return event;
}

bool keypressed(SDL_Event& event, char key) {
	return event.type == SDL_KEYDOWN && event.key.keysym.sym == key;
}

void quit_SDL() {
#pragma region SDL quit
	//	quit SDL
	SDL_Quit();
#pragma endregion
}

double sqrDistance(Point a, Point b) {
	double sqrDistance = 0;
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	sqrDistance = dx * dx + dy * dy;
	return sqrDistance;
}


//int findNearestBallIndex(Ball* ball[], Queue& queue)
//{
//	double minDistance = sqrDistance(ball[0]->getCenter(), queue.getQueueTip());
//	int minIndex = 1;
//	for (int i = 0; i < 2; i++) {
//		double ballToTestDistance = sqrDistance(ball[i]->getCenter(), queue.getQueueTip());
//
//		if (ballToTestDistance < minDistance) {
//			minDistance = ballToTestDistance;
//			minIndex = i;
//		}
//	}
//	return minIndex;
//}

//	entry point of application
int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Billard game");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	//La queue
	Color queueColor(150, 75, 0, SDL_ALPHA_OPAQUE);
	Queue queue(0.5, -90, Point(WIDTH / 2, HEIGHT - 20, true), Point(WIDTH / 2, HEIGHT - 20.0), WIDTH, HEIGHT);

	//LesBoules
	Color ballColor1(255, 255, 255, SDL_ALPHA_OPAQUE);
	Color ballColor2(0, 0, 0, SDL_ALPHA_OPAQUE);
	Ball* ball[2];
	for (int i = 0; i < 2; i++) {
		ball[i] = new Ball(0.5, Point(WIDTH / 2, HEIGHT / 7 + 500 * i), 15, WIDTH, HEIGHT);
	}

	long time = clock();

	//	*********  //
	//	main loop  //
	//	*********  //
	bool endOfGame = false;
	while (!endOfGame) {
		//	******************************  //
		//	draw image in rendering buffer  //
		//	******************************  //
		clearWindow(renderer);

		/*	draw any desired graphical objects here	*/

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		queue.draw(renderer, queueColor, event);
		//for (int i = 0; i < 2; i++) {
			ball[0]->draw(renderer, ballColor2, event, queue, queue.getPropulsion());
			ball[1]->draw(renderer, ballColor1, event, queue, queue.getPropulsion());
		//}

		//For balls self collide
		ball[0]->listenForHit(ball[1], ball[1]->getSpeed());
		ball[1]->listenForHit(ball[0], ball[0]->getSpeed());


		double minDistance = sqrDistance(ball[0]->getCenter(), queue.getQueueTip());
		int minIndex = 0;
		for (int i = 0; i < 2; i++) {
			double ballToTestDistance = sqrDistance(ball[i]->getCenter(), queue.getQueueTip());
			if (ballToTestDistance < minDistance) {
				minDistance = ballToTestDistance;
				minIndex = i;
			}
		}

		ball[minIndex]->visualizePath(queue, renderer);
		


		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, '\033');
		//SDL_KeyCode;
	}

	time = clock() - time;

	quit_SDL();

	return 0;
}