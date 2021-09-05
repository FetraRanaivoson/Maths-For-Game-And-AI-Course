#include "..\IA_1_Perceptron\main.h"
#include "..\IA_1_Perceptron\main.h"
#include <iostream>
#include <SDL.h>
#include <time.h>
#include <random>
#include <cstdlib>
#include <time.h>
#include "../lib_Point/Point.h"
#include "Boid.h"
#include "BehaviorManager.h"
#include "Object.h"
#include "../lib_Slider/Slider.h"
using namespace std;
constexpr auto POS_X = 200, POS_Y = 75;
constexpr auto WINDOW_WIDTH = 1200, WINDOW_HEIGHT = 700;
constexpr auto MAX_BOIDS = 30;
constexpr auto MAX_ENVIR_OBJECTS = 80;


void createBoids(std::vector<Boid*>& boids, int pointSize, double alpha);
void createEnvironment(std::vector<Object*>& staticArea);

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
	SDL_SetRenderDrawColor(renderer, 0, 20, 64, 1);
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
	SDL_Renderer* renderer = init_SDL("Interpolation");

	//Initialize all objects
	int pointSize = 7;
	double alpha = .005;
	std::vector<Boid*> boids;
	createBoids(boids, pointSize, alpha);

	std::vector <Object*> props;
	createEnvironment(props);

	Object* movingTarget = new Object(Point(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), 15, Color(255, 0, 0, SDL_ALPHA_OPAQUE), WINDOW_WIDTH, WINDOW_HEIGHT);

	BehaviorManager* bm = new BehaviorManager();
	bm->addBoids(boids);

	Slider* separationSlider = new Slider(10, 10, 400, 0, 1, .75);
	Slider* alignmentSlider = new Slider(430, 10, 400, 0, 1, 0);
	Slider* cohesionSlider = new Slider(840, 10, 400, 0, 1, 0);
	Slider* targetSlider = new Slider(10, WINDOW_HEIGHT-10, 400, 0, 1, 0);

	int clickPosX, clickPosY;
	Uint32 buttons;
	bool endOfGame = false;

	while (!endOfGame) {
		clearWindow(renderer);
		SDL_Event event = getNextEvent();

		SDL_PumpEvents();
		buttons = SDL_GetMouseState(&clickPosX, &clickPosY);

		movingTarget->drawRandom(renderer);
		separationSlider->draw(renderer, event);
		cohesionSlider->draw(renderer, event);
		alignmentSlider->draw(renderer, event);
		targetSlider->draw(renderer, event);


		for (Boid* boid : boids) {
			boid->setSeparationWeight(separationSlider->getValue());
			boid->setAlignmentWeight(alignmentSlider->getValue());
			boid->setCohesionWeight(cohesionSlider->getValue());
			boid->setTargetWeight(targetSlider->getValue());
			boid->update();
			boid->follow(renderer, movingTarget->getPosition(), movingTarget, props);
			boid->draw(renderer);
		}

		for (Object* prop : props) {
			prop->draw(renderer);
		}

		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');
	}
	quit_SDL();
	return 0;
}


void createBoids(std::vector<Boid*>& boids, int pointSize, double alpha)
{
	for (int i = 0; i < MAX_BOIDS; i++) {
		double maxSpeed = 100.0;
		double maxAcceleration = 700;
		Color pointColor(200, 200, 50, SDL_ALPHA_OPAQUE);
		boids.push_back(new Boid(Vector(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT), 
			pointSize, alpha, maxSpeed, maxAcceleration, pointColor));
	}
}

void createEnvironment(std::vector<Object*>& staticArea)
{
	for (int i = 0; i < MAX_ENVIR_OBJECTS; i++) {
		staticArea.push_back(new Object(Point(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT), rand() % 10 + 5.0, Color(20, 10, 10, SDL_ALPHA_OPAQUE), WINDOW_WIDTH, WINDOW_HEIGHT));
	}
}
