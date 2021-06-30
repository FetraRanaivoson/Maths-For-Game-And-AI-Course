#include <iostream>
using namespace std;
#include <SDL.h>

constexpr auto MAX_POINTS = 100;

constexpr auto POS_X = 400, POS_Y = 50;
constexpr auto WIDTH = 1600, HEIGHT = 900;
constexpr auto MAX_ASTEROIDS = 25;

//	include desired header files for libraries
#include <time.h>
#include "Explosion.h"
#include "PointObject.h"
#include "ExplosionSource.h"

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


void createPointCloud(PointObject* points[])
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < MAX_POINTS; i++) {
		points[i] = new PointObject(Point(rand() % WIDTH + 5, rand() % HEIGHT + 5, false), rand() % 10 + 5);
	}
}

void drawPointCloud(PointObject* points[], SDL_Renderer* renderer, SDL_Event& event)
{

	for (int i = 0; i < MAX_POINTS; i++) {
		points[i]->draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), event);
	}
}

void activateExplosionInCase(Explosion& explosion, SDL_Renderer* renderer, const Color& explosionColor, SDL_Event& event)
{
	explosion.draw(renderer, explosionColor, event);
}

void listenForExplosion(PointObject* points[], const Explosion& explosion)
{
	for (int i = 0; i < MAX_POINTS; i++) {
		points[i]->listenForExplosion(explosion);
	}
}

//	entry point of application
int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Explosion simulation");	//	this object will draw in our window

	//Explosion objects
	Point épicentre(WIDTH / 2, HEIGHT / 2, true);
	double forceEpicentre = 500000; //En Newton (f0)
	double déperditionPuissance = .000001; //Plus c'est petit, plus l'impact sur la distance est fort
	Explosion explosion(épicentre, forceEpicentre, déperditionPuissance);
	Color explosionColor(240, 86, 39, SDL_ALPHA_OPAQUE);

	//Point cloud
	PointObject* points[MAX_POINTS];
	createPointCloud(points);
	
	//Info
	cout << "Drag the red dot to activate the explosion..." << endl;

	bool endOfGame = false;
	while (!endOfGame) {
		clearWindow(renderer);

		SDL_Event event = getNextEvent();
		drawPointCloud(points, renderer, event);
		activateExplosionInCase(explosion, renderer, explosionColor, event);
		listenForExplosion(points, explosion);

		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, '\033');
	}

	quit_SDL();

	return 0;
}