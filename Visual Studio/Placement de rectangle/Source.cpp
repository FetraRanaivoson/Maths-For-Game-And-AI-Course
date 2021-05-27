#include <iostream>
using namespace std;

#include <SDL.h>
constexpr auto WIDTH = 800;
constexpr auto HEIGHT = 600;
#include "../lib_Point/Point.h"
#include "../lib_Cloud/Cloud.h"
#include "../lib_Slider/Slider.h"

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
	fenetre = SDL_CreateWindow("SDL template", 200, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(fenetre, 0, 0);
#pragma endregion


	double marginLeft = 100;
	double marginTop = 100;
	double horizontalSpacing = 150;
	double verticalSpacing = 100;

	//Create sliders/////////
	Slider marginLeftSlider(marginLeft, 400, 100, 0, 100, marginLeft);
	Slider marginTopSlider(marginLeft, 500, 100, 0, 100, marginTop);

	Slider recWidthSlider(marginLeft + 200, 400, 100, 0, 100, 70);
	Slider recHeightSlider(marginLeft + 200, 500, 100, 0, 100, 50);

	Slider horizontalSpacingSlider(marginLeft + 400, 400, 100, 0, 100, horizontalSpacing);
	Slider verticalSpacingSlider(marginLeft + 400, 500, 100, 0, 100, verticalSpacing);
	////////////////////////

	

	SDL_Rect rectangle;
	
	while (true) {
		//	draw image
	//	**********

	//	- clear window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	- draw any desired graphical object
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		double width = 0;
		
		rectangle.x = marginLeftSlider.getValue();
		rectangle.y = marginTopSlider.getValue();
		rectangle.w = recWidthSlider.getValue();
		rectangle.h = recHeightSlider.getValue();
		SDL_RenderDrawRect(renderer, &rectangle);
		SDL_RenderFillRect(renderer, &rectangle);

		do {
			rectangle.x += recWidthSlider.getValue() + horizontalSpacingSlider.getValue();
			SDL_RenderDrawRect(renderer, &rectangle);
			SDL_RenderFillRect(renderer, &rectangle);

			width += rectangle.x + (rectangle.w) / 2.0;
			
		} while (width < WIDTH); 
		cout << width <<" "<< WIDTH <<endl;
		/*
		width = 0;
		rectangle.x = marginLeftSlider.getValue();
		rectangle.y += marginTopSlider.getValue() + horizontalSpacingSlider.getValue();

		do {
			rectangle.x += recWidthSlider.getValue();
			SDL_RenderDrawRect(renderer, &rectangle);
			SDL_RenderFillRect(renderer, &rectangle);

			width += rectangle.x;
		} while (width < WIDTH);
		*/	

	
		

		//	event management
		//	****************

		//	remove next event from queue
		SDL_Event event;
		SDL_PollEvent(&event);



		//	give event to objects for update
		//point1.update(event);
		recWidthSlider.draw(renderer, event);
		recHeightSlider.draw(renderer, event);
		marginLeftSlider.draw(renderer, event);
		marginTopSlider.draw(renderer, event);
		horizontalSpacingSlider.draw(renderer, event);
		verticalSpacingSlider.draw(renderer, event);

		//	show rendering buffer
		//	*********************
		SDL_RenderPresent(renderer);

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