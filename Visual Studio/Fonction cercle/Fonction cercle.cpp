#include <iostream>
using namespace std;

#include <SDL.h>
#include "circle.h"

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 500, POS_Y = 300;
constexpr auto WIDTH = 800, HEIGHT = 600;

enum {CIRCLE_RADIUS, CIRCLE_RESOLUTION };


//	include desired header files for libraries
#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"
#include "Fonction cercle.h"

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


Point fromPlanToScreen(Point p,
	double xMin, double xMax, double yMin, double yMax,
	double xMinS, double xMaxS, double yMinS, double yMaxS) {

	double x = xMinS + (p.x - xMin) * (xMaxS - xMinS) / (xMax - xMin);
	double y = yMinS + (yMax - p.y) * (yMaxS - yMinS) / (yMax - yMin);

	Point result(x, y);

	return result;
}

double fonctionCercle(double x, Point& centreCercle, double& rayon) {
	return centreCercle.y + sqrt( rayon*rayon - ((x -centreCercle.x)*(x-centreCercle.x)) );
}

void drawCircle(SDL_Renderer* renderer, Point& centreCercle, double& rayon) {
	for (double alpha = 0; alpha < 360; alpha += .1) {
		Point p(centreCercle.x + rayon * cos (alpha * M_PI /180), 
				centreCercle.y - rayon * sin (alpha * M_PI /180));
		p.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 1);
	}
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



void UpdateCircleRadius(Circle& circle, Slider* sliders[])
{
	circle.setRadius(sliders[CIRCLE_RADIUS]->getValue());
}

void DrawCircle(Circle& circle, SDL_Renderer* renderer, Slider* sliders[])
{
	//Draw circle and update its resolution
	circle.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), sliders[CIRCLE_RESOLUTION]->getValue());
}

void drawPointToTest(Point& unPoint, SDL_Renderer* renderer)
{
	unPoint.draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), 10);
}

void IsPointInsideCircle(Circle& circle, Point& unPoint, SDL_Renderer* renderer)
{
	if (circle.contains(unPoint))
		unPoint.draw(renderer, Color(0, 0, 255, SDL_ALPHA_OPAQUE), 10);
}

void UpdateAllObjects(Circle& circle, SDL_Event& event, Point& unPoint, Slider* sliders[], SDL_Renderer* renderer)
{
	circle.upadte(event);
	unPoint.update(event);
	UpdateSliders(sliders, renderer, event);
}

int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Fonction cercle");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	Point centreCercle(400,200,true);
	double rayon = 200.0;
	Circle circle(centreCercle, rayon);
	Point unPoint(100, 100, true);
	Slider* sliders[2];
	placeSliders(sliders);
	
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
		DrawCircle(circle, renderer, sliders);
		UpdateCircleRadius(circle, sliders);
		drawPointToTest(unPoint, renderer);
		IsPointInsideCircle(circle, unPoint, renderer);

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		UpdateAllObjects(circle, event, unPoint, sliders, renderer);

		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');
	}

	quit_SDL();
	return 0;
}

