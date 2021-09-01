#include <iostream>
#include <SDL.h>
#include <time.h>
#include <random>
#include <cstdlib>
#include <time.h>
#include "../lib_Point/Point.h"
#include "SimplePoint.h"
using namespace std;
constexpr auto POS_X = 400, POS_Y = 75;
constexpr auto WIDTH = 700, HEIGHT = 700;


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

bool LeftClick(const Uint32& buttons)
{
	return (buttons & SDL_BUTTON_LMASK) != 0;
}
bool RightClick(const Uint32& buttons)
{
	return (buttons & SDL_BUTTON_RMASK) != 0;
}

Point lerp(Point initialPosition, Point destinationPosition, double alpha) {
	Point p = initialPosition;
	if (alpha >= 0 && alpha <= 1) {
		p.x = (1 - alpha) * p.x + alpha * destinationPosition.x;
		p.y = (1 - alpha) * p.y + alpha * destinationPosition.y;
	}
	return p;
}

int main(int argc, char** argv) {
	srand(time(NULL));
	SDL_Renderer* renderer = init_SDL("Interpolation");

	Color pointColor(255, 255, 0, SDL_ALPHA_OPAQUE);
	int pointSize = 25;
	double alpha = .005;
	SimplePoint* point = new SimplePoint(Vector(10, HEIGHT / 2), pointSize, alpha, pointColor);

	int clickPosX, clickPosY;
	Uint32 buttons;
	bool endOfGame = false;

	while (!endOfGame) {
		clearWindow(renderer);
		SDL_Event event = getNextEvent();

		SDL_PumpEvents();
		buttons = SDL_GetMouseState(&clickPosX, &clickPosY);
		
		
		//point->followSimple(renderer, clickPosX, clickPosY);
		point->followRealistic(renderer, clickPosX, clickPosY);
		point->draw(renderer);
		
		
		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');
	}
	quit_SDL();
	return 0;
}