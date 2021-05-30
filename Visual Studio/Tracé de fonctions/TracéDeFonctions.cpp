#include <iostream>
using namespace std;

#include <SDL.h>

//	size of window on screen
constexpr auto WIDTH = 1600;
constexpr auto HEIGHT = 600;
constexpr auto ORIGIN_X = WIDTH / 2;
constexpr auto ORIGIN_Y = HEIGHT / 2;

//	include desired header files for libraries
#include "../lib_Slider/Slider.h"
#include "../lib_Point/Point.h"

enum { X_GAUCHE, X_DROIT, Y_BAS, Y_HAUT, COEFF_A, COEFF_B};

void drawLeftBottomContainer(SDL_Rect& rect, Slider* sliders []) {
	rect.x = ORIGIN_X;
	rect.y = ORIGIN_Y;
	rect.w = sliders[X_GAUCHE]->getValue();
	rect.h = - sliders[Y_BAS]->getValue();
}

void drawRightBottomContainer(SDL_Rect& rect, Slider* sliders[]) {
	rect.x += 1;
	rect.w = sliders[X_DROIT]->getValue();
	rect.h = - sliders[Y_BAS]->getValue();
}

void drawTopLeftContainer(SDL_Rect& rect, Slider* sliders[]) {
	rect.y -= 1;
	rect.x -= 1;
	rect.w = sliders[X_GAUCHE]->getValue();
	rect.h = - sliders[Y_HAUT]->getValue();
}

void drawTopRightContainer(SDL_Rect& rect, Slider* sliders[]) {
	rect.x += 1;
	rect.w = sliders[X_DROIT]->getValue();
	rect.h = - sliders[Y_HAUT]->getValue();
}

void drawLinearFunction(Point pointA, Point pointB, Slider* sliders[], SDL_Renderer* renderer) {
	//y = ax - b
	double y = 0;
	double x = 0;
	double a = 0;
	double b = 0;
	a = sliders[COEFF_A]->getValue();
	b = sliders[COEFF_B]->getValue();
	//Finding points coordinates that intersect the line
	if (y == 0) {
		if (a == 0) {
			//Horizontal line
			pointA.x = 0 + ORIGIN_X;
			pointA.y = b + ORIGIN_Y;
			
		}
		if (a != 0) {
			pointA.x = (b / a) + ORIGIN_X;
			pointA.y = 0 + ORIGIN_Y;
		}
		
	}
	if (x == 0) {
		pointB.x = 0 + ORIGIN_X;
		pointB.y = - b + ORIGIN_Y;
	}
	pointA.draw(renderer, Color(240, 0, 0, SDL_ALPHA_OPAQUE), 10);
	pointB.draw(renderer, Color(240, 0, 0, SDL_ALPHA_OPAQUE), 10);
	SDL_RenderDrawLine(renderer, pointA.x, pointA.y, pointB.x, pointB.y);
	cout << "(" << pointA.x << "," << pointA.y << ")" <<
	"(" << pointB.x << "," << pointB.y << ")"<< endl;
}


double getSliderMaxRange(int sliderIndex) {
	double maxRange = 0;
	if (sliderIndex == X_GAUCHE || sliderIndex == Y_BAS)
		maxRange = -500;
	if (sliderIndex == X_DROIT || sliderIndex == Y_HAUT)
		maxRange = 500;
	if (sliderIndex == COEFF_A)
		maxRange = 100.0;
	if (sliderIndex == COEFF_B)
		maxRange = 1000.0;
	return maxRange;
}

double getSliderMinRange(int sliderIndex) {
	double minRange = 0;
	if (sliderIndex == X_GAUCHE || sliderIndex == Y_BAS)
		minRange = 0;
	if (sliderIndex == X_DROIT || sliderIndex == Y_HAUT)
		minRange = 0;
	if (sliderIndex == COEFF_A)
		minRange = - 100.0;
	if (sliderIndex == COEFF_B)
		minRange = - 1000.0;
	return minRange;
}




double getSliderInitialValue(int sliderIndex) {
	double initialValue = 0;
	if (sliderIndex == X_GAUCHE || sliderIndex == Y_BAS)
		initialValue = ORIGIN_X - 250.0;
	if (sliderIndex == X_DROIT || sliderIndex == Y_HAUT)
		initialValue = ORIGIN_Y + 250.0;
	if (sliderIndex == COEFF_A || sliderIndex == COEFF_B)
		initialValue = 0;
	return initialValue;
}

void setPainterToBlack(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void setPainterToWhite(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 241, 241, 241, SDL_ALPHA_OPAQUE);
}

void start(SDL_Renderer* renderer, SDL_Rect& rect, Slider* sliders[]) {
	bool end = false;
	while (!end) {
		//	paint window in black (clear)
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	- draw any desired graphical objects here
		drawLeftBottomContainer(rect, sliders);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

		//	****************  //
		//	event management  //
		//	****************  //


		//	- remove next event from queue
		SDL_Event event;
		SDL_PollEvent(&event);

		//	- give event to objects for update if needed

		//	*********************  //
		//	show rendering buffer  //
		//	*********************  //
		SDL_RenderPresent(renderer);
	}
}

//	entry point of application
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

	//	create the window and its associated renderer
	fenetre = SDL_CreateWindow("SDL template", 200, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(fenetre, 0, 0);
#pragma endregion

	SDL_Rect rect;
	Slider* sliders[6];
	for (int i = 0; i < 6; i++ ) {
		int x = 30 + (i / 2) * (500 + 30);
		int y = HEIGHT - 100 + (i % 2) * 30;
		double maxRange = getSliderMaxRange(i);
		double minRange = getSliderMinRange(i);
		double initialValue = getSliderInitialValue(i);
		sliders[i] = new Slider(x, y, 500, minRange, maxRange, initialValue);
	}
	Point pointA(ORIGIN_X, ORIGIN_Y, true);
	Point pointB(ORIGIN_X, ORIGIN_Y, true);

	bool end = false;
	while (!end) {
		//	paint window in black (clear)
		setPainterToBlack(renderer);
		SDL_RenderClear(renderer);

		//	- draw any desired graphical objects here
		setPainterToBlack(renderer);
		drawLeftBottomContainer(rect, sliders);
		SDL_RenderDrawRect(renderer, &rect);
		setPainterToWhite(renderer);
		SDL_RenderFillRect(renderer, &rect);

		setPainterToBlack(renderer);
		drawRightBottomContainer(rect, sliders);
		SDL_RenderDrawRect(renderer, &rect);
		setPainterToWhite(renderer);
		SDL_RenderFillRect(renderer, &rect);

		setPainterToBlack(renderer);
		drawTopLeftContainer(rect, sliders);
		SDL_RenderDrawRect(renderer, &rect);
		setPainterToWhite(renderer);
		SDL_RenderFillRect(renderer, &rect);

		setPainterToBlack(renderer);
		drawTopRightContainer(rect, sliders);
		SDL_RenderDrawRect(renderer, &rect);
		setPainterToWhite(renderer);
		SDL_RenderFillRect(renderer, &rect);

		//Display a linear function graph
		drawLinearFunction(pointA, pointB, sliders, renderer);
		

		//	****************  //
		//	event management  //
		//	****************  //
	

		//	- remove next event from queue
		SDL_Event event;
		SDL_PollEvent(&event);

		//	- give event to objects for update if needed
		//	affichage des sliders
		for (int i = 0; i < 6; i++) {
			sliders[i]->draw(renderer, event);
		}
		pointA.update(event);
		pointB.update(event);

		//	*********************  //
		//	show rendering buffer  //
		//	*********************  //
		SDL_RenderPresent(renderer);
	}

#pragma region SDL quit
	//	destroy window and quit SDL
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
#pragma endregion

	return 0;
}

