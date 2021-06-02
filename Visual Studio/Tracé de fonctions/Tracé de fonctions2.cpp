#include <iostream>
using namespace std;

#include <SDL.h>

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 500, POS_Y = 500;		
constexpr auto WIDTH = 1600, HEIGHT = 600;

//	specifications au the drawing window
auto xMinS = 50, xMaxS = 550;
auto yMinS = 50, yMaxS = 450;

enum {SIN_X, X_CARREE, RACINE_10_MOINS_X_CARREE, LINEAIRE_A, LINEAIRE_B, Y0, k, x_MinS, x_MaxS, y_MinS, y_MaxS};

//	include desired header files for libraries
#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"
#include "Tracé de fonctions2.h"


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

double getSliderMaxRange(int sliderIndex) {
	double maxRange = 0;
	if (sliderIndex == SIN_X)
		maxRange = 5;
	if (sliderIndex == X_CARREE)
		maxRange = 1;
	if (sliderIndex == RACINE_10_MOINS_X_CARREE)
		maxRange = 10;
	if (sliderIndex == LINEAIRE_A || sliderIndex == LINEAIRE_B)
		maxRange = 10;
	if (sliderIndex == Y0 || sliderIndex == k)
		maxRange = 10;
	if (sliderIndex == x_MinS)
		maxRange = 550;
	if (sliderIndex == x_MaxS)
		maxRange = 1000;
	if (sliderIndex == y_MinS)
		maxRange = 450;
	if (sliderIndex == y_MaxS)
		maxRange = 1000;
	return maxRange;
}

double getSliderMinRange(int sliderIndex) {
	double minRange = 0;
	if (sliderIndex == SIN_X)
		minRange = -5;
	if (sliderIndex == X_CARREE)
		minRange = -1;
	if (sliderIndex == RACINE_10_MOINS_X_CARREE)
		minRange = -1;
	if (sliderIndex == LINEAIRE_A || sliderIndex == LINEAIRE_B)
		minRange = -10;
	if (sliderIndex == Y0 || sliderIndex == k)
		minRange = -10;
	if (sliderIndex == x_MinS)
		minRange = 0;
	if (sliderIndex == x_MaxS)
		minRange= 0;
	if (sliderIndex == y_MinS)
		minRange = 0;
	if (sliderIndex == y_MaxS)
		minRange = 0;
	return minRange;
}

double getSliderInitialValue(int sliderIndex) {
	double initialValue = 0;
	if (sliderIndex == SIN_X)
		initialValue = .5;
	if (sliderIndex == X_CARREE)
		initialValue = .1;
	if (sliderIndex == RACINE_10_MOINS_X_CARREE)
		initialValue = .1;
	if (sliderIndex == LINEAIRE_A || sliderIndex == LINEAIRE_B)
		initialValue = 1;
	if (sliderIndex == Y0 || sliderIndex == k)
		initialValue = 1;
	if (sliderIndex == x_MinS)
		initialValue = xMinS;
	if (sliderIndex == x_MaxS)
		initialValue = xMaxS;
	if (sliderIndex == y_MinS)
		initialValue = yMinS;
	if (sliderIndex == y_MaxS)
		initialValue = yMaxS;
	return initialValue;
}


void placeSliders(Slider* sliders[])
{
	for (int i = 0; i < 11; i++) {
		int x = 30 + (i / 2) * (200 + 30);
		int y = HEIGHT - 100 + (i % 2) * 30;
		double maxRange = getSliderMaxRange(i);
		double minRange = getSliderMinRange(i);
		double initialValue = getSliderInitialValue(i);
		sliders[i] = new Slider(x, y, 200, minRange, maxRange, initialValue);
	}
}


Point fromPlanToScreen(Point p,
	double xMin, double xMax, double yMin, double yMax,
	double xMinS, double xMaxS, double yMinS, double yMaxS) {

	double x = xMinS + (p.x - xMin) * (xMaxS - xMinS) / (xMax - xMin);
	double y = yMinS + (yMax - p.y) * (yMaxS - yMinS) / (yMax - yMin);

	Point result(x, y);

	return result;
}


double fonctionSinus(double x, Slider* sliders [] ) {
	return sliders[SIN_X]->getValue() * sin(x);
}

double fonctionParabole(double x, Slider* sliders[]) {
	return sliders[X_CARREE]->getValue() * x * x;
}

double fonctionSpéciale(double x, Slider* sliders[]) {
	return sliders[RACINE_10_MOINS_X_CARREE]->getValue() * sqrt(10 - x * x);
}

double fonctionLinéaire(double x, Slider* sliders[]) {
	return sliders[LINEAIRE_A]->getValue() * x + sliders[LINEAIRE_B]->getValue();
}

double fonctionExponentielle(double x, Slider* sliders[]) {
	return sliders[Y0]->getValue() * exp(- sliders[k]->getValue() * x * x);
}

void displayExponentFunction(double xMin, double xMax, Slider* sliders[], double yMin, double yMax, SDL_Renderer* renderer)
{
	for (double x = xMin; x <= xMax; x += 0.1) {
		Point pPlan(x, fonctionExponentielle(x, sliders));

		if (pPlan.y >= yMin && pPlan.y <= yMax) {
			Point pScreen = fromPlanToScreen(pPlan,
				xMin, xMax, yMin, yMax,
				xMinS, xMaxS, yMinS, yMaxS);
			pScreen.draw(renderer, Color(255, 215, 0, SDL_ALPHA_OPAQUE), 3);
		}
	}
}

void displaySinusFunction(double xMin, double xMax, Slider* sliders[], double yMin, double yMax, SDL_Renderer* renderer)
{
	for (double x = xMin; x <= xMax; x += 0.1) {
		Point pPlan(x, fonctionSinus(x, sliders));

		if (pPlan.y >= yMin && pPlan.y <= yMax) {
			Point pScreen = fromPlanToScreen(pPlan,
				xMin, xMax, yMin, yMax,
				xMinS, xMaxS, yMinS, yMaxS);
			pScreen.draw(renderer, Color(120, 255, 0, SDL_ALPHA_OPAQUE), 3);
		}
	}
}


void displayParabolicFunction(double xMin, double xMax, Slider* sliders[], double yMin, double yMax, SDL_Renderer* renderer) {
	for (double x = xMin; x <= xMax; x += 0.1) {
		Point pPlan1(x, fonctionParabole(x, sliders));

		if (pPlan1.y >= yMin && pPlan1.y <= yMax) {
			Point pScreen1 = fromPlanToScreen(pPlan1,
				xMin, xMax, yMin, yMax,
				xMinS, xMaxS, yMinS, yMaxS);
			pScreen1.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 3);
		}
	}
	
}

void displaySpecialFunction(double xMin, double xMax, Slider* sliders[], double yMin, double yMax, SDL_Renderer* renderer) {
	for (double x = xMin; x <= xMax; x += 0.1) {
		Point pPlan1(x, fonctionSpéciale(x, sliders));

		if (pPlan1.y >= yMin && pPlan1.y <= yMax) {
			Point pScreen1 = fromPlanToScreen(pPlan1,
				xMin, xMax, yMin, yMax,
				xMinS, xMaxS, yMinS, yMaxS);
			pScreen1.draw(renderer, Color(125, 97, 35, SDL_ALPHA_OPAQUE), 3);
		}
	}

}

void displayLinearFunction(double xMin, double xMax, Slider* sliders[], double yMin, double yMax, SDL_Renderer* renderer) {
	for (double x = xMin; x <= xMax; x += 0.1) {
		Point pPlan1(x, fonctionLinéaire(x, sliders));

		if (pPlan1.y >= yMin && pPlan1.y <= yMax) {
			Point pScreen1 = fromPlanToScreen(pPlan1,
				xMin, xMax, yMin, yMax,
				xMinS, xMaxS, yMinS, yMaxS);
			pScreen1.draw(renderer, Color(0, 50, 255, SDL_ALPHA_OPAQUE), 3);
		}

	}
}


void DessinerLesRepères(SDL_Renderer* renderer, const Point& originPlan, double xMin, double xMax, double yMin, double yMax)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	Point originScreen = fromPlanToScreen(originPlan, xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS);
	originScreen.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), 5);
	SDL_RenderDrawLine(renderer, xMinS, originScreen.y, xMaxS, originScreen.y);
	SDL_RenderDrawLine(renderer, originScreen.x, yMinS, originScreen.x, yMaxS);
}

//	entry point of application
int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Trace de fonctions");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	Point originPlan(0, 0);
	double xMin = -10, xMax = 5;
	double yMin = -2, yMax = 2;


	//Nos sliders
	Slider* sliders[11];
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
		SDL_Rect rect = { xMinS, yMinS, xMaxS - xMinS, yMaxS - yMinS };
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(renderer, &rect);

		//Dessiner les repères (0,x,y) du screen
		DessinerLesRepères(renderer, originPlan, xMin, xMax, yMin, yMax);

		//Display curves
		displaySinusFunction(xMin, xMax, sliders, yMin, yMax, renderer);
		displayParabolicFunction(xMin, xMax, sliders, yMin, yMax, renderer);
		displaySpecialFunction(xMin, xMax, sliders, yMin, yMax, renderer);
		displayLinearFunction(xMin, xMax, sliders, yMin, yMax, renderer);
		displayExponentFunction(xMin, xMax, sliders, yMin, yMax, renderer);

		//Stretch /shrink
		xMinS = sliders[x_MinS]->getValue();
		xMaxS = sliders[x_MaxS]->getValue();
		yMinS = sliders[y_MinS]->getValue();
		yMaxS = sliders[y_MaxS]->getValue();

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		UpdateSliders(sliders, renderer, event);
		//originPlan.update(event);

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');
	}

	quit_SDL();

	return 0;
}

void UpdateSliders(Slider* sliders[], SDL_Renderer* renderer, SDL_Event& event)
{
	for (int i = 0; i < 11; i++)
		sliders[i]->draw(renderer, event);
}
