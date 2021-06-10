#include <iostream>
using namespace std;

#include <SDL.h>

constexpr auto MAX_OBJECTS = 10;

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 500, POS_Y = 200;
constexpr auto WIDTH = 600, HEIGHT = 700;



//	include desired header files for libraries
#include "VaisseauObject.h"
#include <time.h>
#include "Astéroids.h"


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

void manageKeysInput(SDL_Renderer* renderer, SDL_Event& event, VaisseauObject& vaisseau)
{
	//KEY MANAGEMENT
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
		Vector newSpeed(0, 500);
		vaisseau.setSpeed(newSpeed);
	}
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
		Vector newSpeed(0, -500);
		vaisseau.setSpeed(newSpeed);
	}
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) {
		Vector newSpeed(-500, 0);
		vaisseau.setSpeed(newSpeed);
	}
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {
		Vector newSpeed(500, 0);
		vaisseau.setSpeed(newSpeed);
	}
	

	if (event.type == SDL_KEYUP) {
		Vector newSpeed(0, 0);
		vaisseau.setSpeed(newSpeed);
	}
}

//	entry point of application
int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Asteroid game");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	VaisseauObject vaisseau(100, Point(WIDTH/2, HEIGHT/2, true), Vector(0, 0), Vector(0,1000), WIDTH, HEIGHT);


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

		vaisseau.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), event);
		manageKeysInput(renderer, event, vaisseau);

		//if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
			//vaisseau.fireLaser(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), event);
		//}


		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');
		SDL_KeyCode;
	}

	time = clock() - time;

	quit_SDL();

	return 0;
}