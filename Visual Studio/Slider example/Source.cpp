#include <iostream>
using namespace std;

#include <SDL.h>

constexpr auto FENETRE_WIDTH = 400;
constexpr auto FENETRE_HEIGHT = 400;

#include "../lib_Slider/Slider.h"

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

	//	create the window and its renderer
	fenetre = SDL_CreateWindow("SDL template", 200, 100, FENETRE_WIDTH, FENETRE_HEIGHT, 0);
	renderer = SDL_CreateRenderer(fenetre, 0, 0);
#pragma endregion

	//	main loop here
	cout << "Press any key in window to exit" << endl;

	//	Slider(int x, int y, int width, double min, double max, double value)
	Slider slider1(20, 20, 150, 0, 100, 30);
	Slider slider2(180, 20, 150, 0, 100, 30);

	while (true) {
		//	prepare next frame
		//	******************

		//	paint window in black (clear)
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	event management
		//	****************

		//	remove next event from queue
		SDL_Event event;
		SDL_PollEvent(&event);

		//	give event to slider for update
		slider1.draw(renderer, event);
		slider2.draw(renderer, event);

		//	show new frame
		//	**************
		SDL_RenderPresent(renderer);

		//	print slider value in console
		cout << "slider 1 : " << slider1.getValue() << "   /   ";
		cout << "slider 2 : " << slider2.getValue() << endl;

		//	exit if key pressed
		if (event.type == SDL_KEYDOWN) {
			break;
		}
	}

#pragma region SDL quit
	//	destroy window and quit SDL
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
#pragma endregion

	return 0;
}