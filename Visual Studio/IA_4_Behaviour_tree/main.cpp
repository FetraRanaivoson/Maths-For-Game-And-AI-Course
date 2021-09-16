#include <iostream>
#include <SDL.h>
#include <time.h>
#include <random>
#include <cstdlib>
#include <time.h>
#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"
#include "../IA_4_Behaviour_tree/Labyrinth2.h"
#include "../IA_4_Behaviour_tree/Droid2.h"
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
	SDL_Renderer* renderer = init_SDL("Behaviour");

	//Initialize all objects
	Labyrinth* labyrinth = new Labyrinth(WINDOW_WIDTH, WINDOW_HEIGHT);
	Droid* droid = new Droid(Point(8, 8), Color(255, 164, 0, SDL_ALPHA_OPAQUE));
	labyrinth->addDroid(droid);

	double timeToReset = 0.0;

	int clickPosX, clickPosY;
	Uint32 buttons;

	bool endOfGame = false;
	while (!endOfGame) {
		clearWindow(renderer);
		SDL_Event event = getNextEvent();

		SDL_PumpEvents();
		buttons = SDL_GetMouseState(&clickPosX, &clickPosY);
		if (LeftClick(buttons)) {
			labyrinth->getExitNode()->setPosition(Point(clickPosX, clickPosY));
			//labyrinth->createExitKnot();
			labyrinth->findShortestPath(
				new Node(
					Point(labyrinth->getDroid()->getPosition().x, labyrinth->getDroid()->getPosition().y),
					Point(labyrinth->getExitNode()->getPosition().x, labyrinth->getExitNode()->getPosition().y),nullptr),
				new Node(
					Point(labyrinth->getExitNode()->getPosition().x, labyrinth->getExitNode()->getPosition().y),
					Point(labyrinth->getExitNode()->getPosition().x, labyrinth->getExitNode()->getPosition().y), nullptr
				), renderer
			);
		}

		labyrinth->draw(renderer);
		//labyrinth->findShortestPath(renderer);
		

		/*if (labyrinth->isPathFound())
		{
			timeToReset += .001;
			if (timeToReset > 5) {
				clearWindow(renderer);
				labyrinth->reset();
				timeToReset = 0;
			}
		}*/
		//labyrinth->getDroid()->draw(renderer);
		//labyrinth->getDroid()->goTo(labyrinth->getExitNode()->getPosition());
		//labyrinth->getDroid()->wander(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
		//if (labyrinth->isPathFound()) {
		//	//Go to first point, next time go to the next point, etc until pathNodes.size()
		//	labyrinth->getDroid()->goTo(labyrinth->getPathNodes()[0]->getPosition());
		//	labyrinth->getDroid()->draw(renderer);
		//}


		labyrinth->getDroid()->wander(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');
	}
	delete labyrinth;
	delete droid;
	quit_SDL();
	return 0;
}
