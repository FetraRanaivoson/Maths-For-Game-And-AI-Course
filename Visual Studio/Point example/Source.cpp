#include <iostream>
using namespace std;

#include <SDL.h>

constexpr auto FENETRE_WIDTH = 400;
constexpr auto FENETRE_HEIGHT = 400;

#include "../lib_Point/Point.h"

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

	//	Point(double x, double y, bool isMoveable = false)
	Point point(100, 100, true);

	while (true) {
		//	prepare next frame
		//	******************

		//	paint window in black (clear)
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		Color color(255, 255, 255, SDL_ALPHA_OPAQUE);
		point.draw(renderer, color, 5);
		point.drawCircle(renderer, 100, color, true);

		//	event management
		//	****************

		//	remove next event from queue
		SDL_Event event;
		SDL_PollEvent(&event);

		//	give event to point for update
		point.update(event);

		//	show new frame
		//	**************
		SDL_RenderPresent(renderer);

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