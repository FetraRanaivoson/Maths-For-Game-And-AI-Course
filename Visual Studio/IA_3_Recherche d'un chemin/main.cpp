#include <iostream>
#include <SDL.h>
#include <time.h>
#include <random>
#include <cstdlib>
#include <time.h>
#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"
#include "Labyrinth.h"
using namespace std;
constexpr auto POS_X = 200, POS_Y = 30;
constexpr auto WINDOW_WIDTH = 1200, WINDOW_HEIGHT = 800;
constexpr auto MAX_BOIDS = 50;
constexpr auto MAX_ENVIR_OBJECTS = 20;

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
	SDL_SetRenderDrawColor(renderer, 10, 10, 10, 1);
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

int main(int argc, char** argv) {
	srand(time(NULL));
	SDL_Renderer* renderer = init_SDL("A*");

	//Initialize all objects
	Labyrinth* labyrinth = new Labyrinth(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	double timeToReset = 0.0; 

	bool endOfGame = false;
	while (!endOfGame) {
		
		SDL_Event event = getNextEvent();

		if (!labyrinth->isPathFound()) {
			labyrinth->draw(renderer);
			labyrinth->findShortestPath(renderer);
		}
		if (labyrinth->isPathFound())
		{
			timeToReset += .001;
			if (timeToReset > 5) {
				clearWindow(renderer);
				labyrinth->reset();
				timeToReset = 0;
			}
		}

		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');
	}
	quit_SDL();
	return 0;
}

