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
constexpr auto MAX_ASTEROIDS = 25;


//	include desired header files for libraries
#include <time.h>
#include "Explosion.h"

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


//	entry point of application
int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Explosion simulation");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	Point épicentre(WIDTH / 2, HEIGHT / 2);
	double forceEpicentre = 5000; //En Newtion (f0)
	double déperditionPuissance = .01; //Plus c'est petit, plus la force diminue lentement
	Explosion explosion(Point(WIDTH / 2, HEIGHT / 2), forceEpicentre, déperditionPuissance);
	Color expolsionColor(240,86,39,SDL_ALPHA_OPAQUE);


	bool endOfGame = false;
	while (!endOfGame) {
		clearWindow(renderer);

		SDL_Event event = getNextEvent();
		explosion.draw(renderer, expolsionColor, event);

		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, '\033');
		//SDL_KeyCode;
	}

	quit_SDL();

	return 0;
}