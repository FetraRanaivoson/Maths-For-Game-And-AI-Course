#include <iostream>
#include <SDL.h>
#include "Perceptron.h"
#include <time.h>
#include <random>
#include <cstdlib>
#include "Perceptron.h"
#include <time.h>
#include "main.h"
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

int main(int argc, char** argv) {
	srand(time(NULL));
	SDL_Renderer* renderer = init_SDL("Perceptron");

	Perceptron* perceptron = new Perceptron(2, WIDTH, HEIGHT);//2 K*: 2 entrées 

	int clickPosX, clickPosY;
	Uint32 buttons;
	bool endOfGame = false;

	cout << "LEFT CLICK: ADD RED (CAT), RIGHT CLICK: ADD GREEN (DOG)" << endl;

	while (!endOfGame) {
		clearWindow(renderer);
		SDL_Event event = getNextEvent();

		SDL_PumpEvents();
		buttons = SDL_GetMouseState(&clickPosX, &clickPosY);

		if (LeftClick(buttons)) 
			perceptron->addCat(clickPosX, clickPosY);
		if (RightClick(buttons))
			perceptron->addDog(clickPosX, clickPosY);
	
		perceptron->drawRepères(renderer);
		perceptron->drawCandidates(renderer);
		perceptron->update(renderer);

		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');
	}
	quit_SDL();
	return 0;
}