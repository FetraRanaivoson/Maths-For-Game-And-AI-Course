#include <iostream>
using namespace std;

#include <SDL.h>


//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 500, POS_Y = 300;
constexpr auto WIDTH = 800, HEIGHT = 600;

//	include desired header files for libraries
#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"


SDL_Renderer* init_SDL(const char* title) {
#pragma region SDL initialization
	// SDL initialization
	SDL_Window* window = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "ERREUR : can't initialize SDL!" << endl;
		exit(0);
	}

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


Point fromPlanToScreen(Point p,
	double xMin, double xMax, double yMin, double yMax,
	double xMinS, double xMaxS, double yMinS, double yMaxS) {

	double x = xMinS + (p.x - xMin) * (xMaxS - xMinS) / (xMax - xMin);
	double y = yMinS + (yMax - p.y) * (yMaxS - yMinS) / (yMax - yMin);

	Point result(x, y);

	return result;
}

void placeSliders(Slider* sliders[])
{
	for (int i = 0; i < 12; i++) {
		int x = 30 + (i / 2) * (200 + 30);
		int y = HEIGHT - 100 + (i % 2) * 30;
		sliders[i] = new Slider(x, y, 200, 0, 1000, 50);
	}
}


void UpdateSliders(Slider* sliders[], SDL_Renderer* renderer, SDL_Event& event)
{
	for (int i = 0; i < 2; i++)
		sliders[i]->draw(renderer, event);
}


int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Quiz1");	//	this object will draw in our window

	/*	prepare useful objects here	*/

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
		//	show drawing window
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/

		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');
	}

	quit_SDL();
	return 0;
}

