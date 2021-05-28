#include <iostream>
using namespace std;

#include <SDL.h>

//	size of window on screen///////////////////////////////////////////////
constexpr auto FENETRE_WIDTH = 400;
constexpr auto FENETRE_HEIGHT = 400;
///////////////////////////////////////////////////////////////////////////



//	include desired libraries if needed
#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"

///////////////////////////////////////////////////////////////////////////



//	entry point of application/////////////////////////////////////////////
int main(int argc, char** argv) {

#pragma region SDL initialization
	// SDL initialization////
	SDL_Window* fenetre = NULL;
	SDL_Renderer* renderer = NULL;
	/////////////////////////

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

	//	prepare usefull objects here
	//	****************************
	int rayon = 100; //initialize the radius

	Point point(100,100,true);
	Slider slider1(200,300,150,0,100,rayon);
	


	//	main loop here
	//	**************
	while (true) {
		//	draw image
		//	**********

		//	- clear window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	- draw any desired graphical object
		Color color(255, 255, 255, SDL_ALPHA_OPAQUE);
		point.draw(renderer, color, 5);
		point.drawCircle(renderer, rayon, color, true);

		
		//	event management
		//	****************

		//	remove next event from queue
		SDL_Event event;
		SDL_PollEvent(&event);

		/*assign the slider value to the circle radius*/
		rayon = slider1.getValue(); 
		point.drawCircle(renderer, rayon, color, true); 


		//	give event to objects for update //hey
		point.update(event);
		slider1.draw(renderer, event);

		//	show rendering buffer
		//	*********************
		SDL_RenderPresent(renderer);


		//cout << "Circle radius: " << slider1.getValue() << endl;


		//	check keypress for exit
		//	***************************
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