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
#include "BehaviourTree.h"
#include "main.h"
#include "Droid3.h"
using namespace std;
constexpr auto POS_X = 200, POS_Y = 30;
constexpr auto WINDOW_WIDTH = 1200, WINDOW_HEIGHT = 800;
constexpr auto MAX_BOIDS = 50;
constexpr auto MAX_ENVIR_OBJECTS = 20;
constexpr auto MAX_RESOURCES = 150;

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

	//Initialize scene
	Labyrinth* labyrinth = new Labyrinth(WINDOW_WIDTH, WINDOW_HEIGHT);

	std::vector<Point>resources;
	for (int i = 0; i < MAX_RESOURCES; i++) {
		Point resourcePosition = Point((int)(rand() % WINDOW_WIDTH / 8) * 8.0, ((int)(rand() % WINDOW_HEIGHT) / 8) * 8.0, true);
		resources.push_back(resourcePosition);
	}

	//Droid* droid = new Droid(Point(8, 8), Color(255, 164, 0, SDL_ALPHA_OPAQUE));
	//labyrinth->addDroid(droid);
	Droid3* droid = new Droid3(Point(8, 8), Color(20, 10, 255, SDL_ALPHA_OPAQUE), WINDOW_WIDTH, WINDOW_HEIGHT);
	labyrinth->addResources(resources);

	//Create BT Nodes here
	BTNode* selectorBTNode1 = new BTNode("selector1", Type::selector);
	BTNode* sequencerBTNode = new BTNode("sequencer", Type::sequencer);

	BTNode* wanderBTNode = new BTNode("wander", Type::action);
	wanderBTNode->setActor(droid, Action::wander); //Adding listener (listener implements/handle the action)

	BTNode* checkResourcesBTNode = new BTNode("checkResources", Type::action);
	checkResourcesBTNode->setActor(droid, Action::checkResourcesInRange); //Adding listener (listener implements/handle the action)

	BTNode* goToResourcesBTNode = new BTNode("goToResources", Type::action);
	goToResourcesBTNode->setActor(droid, Action::goToResource); //Adding listener (listener implements/handle the action)

	//Add children to root node
	//left branch
	selectorBTNode1->addChild(sequencerBTNode);
	sequencerBTNode->addChild(checkResourcesBTNode);
	sequencerBTNode->addChild(goToResourcesBTNode);
	//right branch
	selectorBTNode1->addChild(wanderBTNode);


	int clickPosX, clickPosY;
	Uint32 buttons;

	Point testPoint(100, 70, true);

	bool endOfGame = false;
	while (!endOfGame) {
		clearWindow(renderer);
		SDL_Event event = getNextEvent();

		SDL_PumpEvents();
		buttons = SDL_GetMouseState(&clickPosX, &clickPosY);


		selectorBTNode1->evaluateNode(renderer); //Evaluate the root
		labyrinth->draw(renderer);
		droid->draw(renderer);
		testPoint.draw(renderer, Color(255, 125, 0, 1), 10);
		//droid->goTo(testPoint);

		testPoint.update(event);


		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');
	}

	delete labyrinth;
	delete droid;
	delete selectorBTNode1;
	delete sequencerBTNode;
	delete wanderBTNode;
	delete checkResourcesBTNode;
	delete goToResourcesBTNode;
	quit_SDL();
	return 0;
}

