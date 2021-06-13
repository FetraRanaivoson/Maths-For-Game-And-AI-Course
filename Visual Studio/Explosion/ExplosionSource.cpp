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
#include "VaisseauObject.h"
#include "Asteroid.h"
#include <time.h>
#include "sourceGame.h"


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


void CreateAndRandomizeAsteroids(Asteroid* asteroids[25])
{
	for (int i = 0; i < MAX_ASTEROIDS; i++) {
		asteroids[i] = new Asteroid(Point(rand() % WIDTH + 1, rand() % HEIGHT / 10 + 1),	//Position
			Vector(rand() % 50, rand() % 150),	//Speed
			20,													//radius
			200,												//mass
			WIDTH, HEIGHT);
	}
}

//	entry point of application
int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Asteroid game");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	srand((unsigned int)time(NULL));
	VaisseauObject vaisseau(100, 0.0, 25, Point(WIDTH / 2, HEIGHT - 100, true), WIDTH, HEIGHT);

	Asteroid* asteroids[MAX_ASTEROIDS];
	CreateAndRandomizeAsteroids(asteroids);

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
		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			asteroids[i]->draw(renderer, Color(244, 244, 244, SDL_ALPHA_OPAQUE), event);
		}
		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			if (asteroids[i]->isHitBy(vaisseau)) {
				asteroids[i]->decrementRadius(asteroids[i]->getRadius() -= 5);
				int signX = rand() % 2;
				int signY = rand() % 2;
				(signX == 0) ? signX = -1, signY = 1 : signX = 1, signY = -1;
				(signY == 0) ? signX = -1, signY = 1 : signX = 1, signY = -1;
				Vector newSpeed(-signX * rand()%500, - signY * rand()%500);
				asteroids[i]->setSpeed(newSpeed);
			}
		}
		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			if (asteroids[i]->Hit(vaisseau)) {
				quit_SDL();
			}
		}

		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, '\033');
		//SDL_KeyCode;
	}

	time = clock() - time;

	quit_SDL();

	return 0;
}