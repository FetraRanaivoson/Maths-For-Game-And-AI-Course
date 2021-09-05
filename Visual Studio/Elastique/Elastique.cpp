#include <iostream>
using namespace std;

#include <SDL.h>

constexpr auto MAX_OBJECTS = 10;

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 500, POS_Y = 200;
constexpr auto WINDOW_WIDTH = 600, WINDOW_HEIGHT = 900;



//	include desired header files for libraries
#include "GameObject.h"
#include <time.h>


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
	window = SDL_CreateWindow(title, POS_X, POS_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	return SDL_CreateRenderer(window, 0, 0);
#pragma endregion
}

void clearWindow(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
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

//	entry point of application
int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("SLD template");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	Perceptron objects(3, Point(WINDOW_WIDTH/4, WINDOW_HEIGHT/4, true), Point(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, true), Vector(0, 0), Vector(0,0), WINDOW_WIDTH, WINDOW_HEIGHT);
	double initialDistanceInX = objects.getABDistanceInX();
	double initialDistanceInY = objects.getABDistanceInY();


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

		//if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN )
		objects.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), event, initialDistanceInX, initialDistanceInY);


		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');

		SDL_KeyCode;
	}

	time = clock() - time;

	quit_SDL();

	return 0;
}