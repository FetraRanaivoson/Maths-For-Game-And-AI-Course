#include <iostream>
using namespace std;

#include <SDL.h>

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 500, POS_Y = 500;		
constexpr auto WIDTH = 1600, HEIGHT = 600;



enum {SIN_X, X_CARREE, RACINE_10_MOINS_X_CARREE, LINEAIRE_A, LINEAIRE_B, Y0, k, x_MinS, x_MaxS, y_MinS, y_MaxS, POINT_NUMBER};

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
		maxRange = WIDTH - 90.0;
	if (sliderIndex == x_MaxS)
		maxRange = WIDTH - 90.0;
	if (sliderIndex == y_MinS)
		maxRange = HEIGHT - 130.0;
	if (sliderIndex == y_MaxS)
		maxRange = HEIGHT - 130.0;
	if (sliderIndex == POINT_NUMBER)
		maxRange = 0.5;
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
		minRange = WIDTH - 1510.0;
	if (sliderIndex == x_MaxS)
		minRange = x_MinS;
	if (sliderIndex == y_MinS)
		minRange = HEIGHT - 560.0;
	if (sliderIndex == y_MaxS)
		minRange = y_MinS;
	if (sliderIndex == POINT_NUMBER)
		minRange = 0.001;
	return minRange;
}

double getSliderInitialValue(int sliderIndex, double& xMinS, double& xMaxS, double& yMinS, double& yMaxS) {
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
		initialValue = WIDTH - 1500.0;
	if (sliderIndex == x_MaxS)
		initialValue = WIDTH - 100.0;
	if (sliderIndex == y_MinS)
		initialValue = HEIGHT - 550.0;
	if (sliderIndex == y_MaxS)
		initialValue = HEIGHT - 140.0;
	if (sliderIndex == POINT_NUMBER)
		initialValue = 0.1;
	return initialValue;
}


void placeSliders(Slider* sliders[], double& xMinS, double& xMaxS, double& yMinS, double& yMaxS)
{
	for (int i = 0; i < 12; i++) {
		int x = 30 + (i / 2) * (200 + 30);
		int y = HEIGHT - 100 + (i % 2) * 30;
		double maxRange = getSliderMaxRange(i);
		double minRange = getSliderMinRange(i);
		double initialValue = getSliderInitialValue(i, xMinS, xMaxS, yMinS, yMaxS);
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

void displayExponentFunction(double xMin, double xMax, Slider* sliders[], double yMin, double yMax, SDL_Renderer* renderer, 
	double& xMinS, double& xMaxS, double& yMinS, double& yMaxS)
{
	for (double x = xMin; x <= xMax; x += sliders[POINT_NUMBER]->getValue()) {
		Point pPlan(x, fonctionExponentielle(x, sliders));

		if (pPlan.y >= yMin && pPlan.y <= yMax) {
			Point pScreen = fromPlanToScreen(pPlan,
				xMin, xMax, yMin, yMax,
				xMinS, xMaxS, yMinS, yMaxS);
			pScreen.draw(renderer, Color(255, 215, 0, SDL_ALPHA_OPAQUE), 3);
		}
	}
}

void displaySinusFunction(double xMin, double xMax, Slider* sliders[], double yMin, double yMax, SDL_Renderer* renderer,
	double& xMinS, double& xMaxS, double& yMinS, double& yMaxS)
{
	for (double x = xMin; x <= xMax; x += sliders[POINT_NUMBER]->getValue()) {
		Point pPlan(x, fonctionSinus(x, sliders));

		if (pPlan.y >= yMin && pPlan.y <= yMax) {
			Point pScreen = fromPlanToScreen(pPlan,
				xMin, xMax, yMin, yMax,
				xMinS, xMaxS, yMinS, yMaxS);
			pScreen.draw(renderer, Color(120, 255, 0, SDL_ALPHA_OPAQUE), 3);
		}
	}
}


void displayParabolicFunction(double xMin, double xMax, Slider* sliders[], double yMin, double yMax, SDL_Renderer* renderer,
	double& xMinS, double& xMaxS, double& yMinS, double& yMaxS) {
	for (double x = xMin; x <= xMax; x += sliders[POINT_NUMBER]->getValue()) {
		Point pPlan1(x, fonctionParabole(x, sliders));

		if (pPlan1.y >= yMin && pPlan1.y <= yMax) {
			Point pScreen1 = fromPlanToScreen(pPlan1,
				xMin, xMax, yMin, yMax,
				xMinS, xMaxS, yMinS, yMaxS);
			pScreen1.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 3);
		}
	}
	
}

void displaySpecialFunction(double xMin, double xMax, Slider* sliders[], double yMin, double yMax, SDL_Renderer* renderer,
	double& xMinS, double& xMaxS, double& yMinS, double& yMaxS) {
	for (double x = xMin; x <= xMax; x += sliders[POINT_NUMBER]->getValue()) {
		Point pPlan1(x, fonctionSpéciale(x, sliders));

		if (pPlan1.y >= yMin && pPlan1.y <= yMax) {
			Point pScreen1 = fromPlanToScreen(pPlan1,
				xMin, xMax, yMin, yMax,
				xMinS, xMaxS, yMinS, yMaxS);
			pScreen1.draw(renderer, Color(125, 97, 35, SDL_ALPHA_OPAQUE), 3);
		}
	}

}

void displayLinearFunction(double xMin, double xMax, Slider* sliders[], double yMin, double yMax, SDL_Renderer* renderer, 
	double& xMinS, double& xMaxS, double& yMinS, double& yMaxS) {
	for (double x = xMin; x <= xMax; x += sliders[POINT_NUMBER]->getValue()) {
		Point pPlan1(x, fonctionLinéaire(x, sliders));

		if (pPlan1.y >= yMin && pPlan1.y <= yMax) {
			Point pScreen1 = fromPlanToScreen(pPlan1,
				xMin, xMax, yMin, yMax,
				xMinS, xMaxS, yMinS, yMaxS);
			pScreen1.draw(renderer, Color(0, 50, 255, SDL_ALPHA_OPAQUE), 3);
		}

	}
}


void DessinerLesRepères(SDL_Renderer* renderer, const Point& originPlan, double xMin, double xMax, double yMin, double yMax,
	double& xMinS, double& xMaxS, double& yMinS, double& yMaxS)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	Point originScreen = fromPlanToScreen(originPlan, xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS);
	originScreen.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), 5);
	SDL_RenderDrawLine(renderer, xMinS, originScreen.y, xMaxS, originScreen.y);
	SDL_RenderDrawLine(renderer, originScreen.x, yMinS, originScreen.x, yMaxS);
}

void ListenToScreenSquashAndStretch(double& xMinS, double& xMaxS, Slider* sliders[], double& yMinS, double& yMaxS)
{
	//Listen to screen Squash & stretch
	if (xMinS <= xMaxS) {
		xMinS = sliders[x_MinS]->getValue();		//Whatever we set for xMins,
		sliders[x_MinS]->setMax(xMaxS);				//xMinS will always be INFERIOR TO xMaxS
		sliders[x_MinS]->setMin(WIDTH - 1510.0);	//And all SUPERIOR TO Margin left
	}
	if (xMaxS >= xMinS) {
		xMaxS = sliders[x_MaxS]->getValue();		//Whatever we set for xMaxS,
		sliders[x_MaxS]->setMin(xMinS);				//xMaxS will always be SUPERIOR TO xMinS
		sliders[x_MaxS]->setMax(WIDTH - 90.0);		//And all INFERIOR TO Margin right
	}
	if (yMinS <= yMaxS) {
		yMinS = sliders[y_MinS]->getValue();		//Whatever we set for yMinS,
		sliders[y_MinS]->setMax(yMaxS);				// yMinS will always be INFERIOR TO yMaxS
		sliders[y_MinS]->setMin(HEIGHT - 560.0);	//And all SUPERIOR TO Margin top
	}

	if (yMaxS >= yMinS && yMaxS <= HEIGHT - 130.0) {
		yMaxS = sliders[y_MaxS]->getValue();		//Whatever we set for yMaxS,
		sliders[y_MaxS]->setMin(yMinS);				//yMaxS will always be SUPERIOR TO yMinS
		sliders[y_MaxS]->setMax(HEIGHT - 130.0);	//And all INFERIOR TO Margin bottom
	}
}

void UpdateSliders(Slider* sliders[], SDL_Renderer* renderer, SDL_Event& event)
{
	for (int i = 0; i < 12; i++)
		sliders[i]->draw(renderer, event);
}



int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Trace de fonctions");	//	this object will draw in our window

	/*	prepare useful objects here	*/
	Point originPlan(0, 0);
	double xMin = -10, xMax = 5;
	double yMin = -2, yMax = 2;

	//	specifications au the drawing window
	double xMinS = 50.0, xMaxS = 550.0;
	double yMinS = 50.0, yMaxS = 450.0;

	//Nos sliders
	Slider* sliders[12];
	placeSliders(sliders, xMinS, xMaxS, yMinS, yMaxS);
	
	
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
		DessinerLesRepères(renderer, originPlan, xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS);

		//Display curves
		displaySinusFunction(xMin, xMax, sliders, yMin, yMax, renderer, xMinS, xMaxS, yMinS, yMaxS);
		displayParabolicFunction(xMin, xMax, sliders, yMin, yMax, renderer, xMinS, xMaxS, yMinS, yMaxS);
		displaySpecialFunction(xMin, xMax, sliders, yMin, yMax, renderer, xMinS, xMaxS, yMinS, yMaxS);
		displayLinearFunction(xMin, xMax, sliders, yMin, yMax, renderer, xMinS, xMaxS, yMinS, yMaxS);
		displayExponentFunction(xMin, xMax, sliders, yMin, yMax, renderer, xMinS, xMaxS, yMinS, yMaxS);

		ListenToScreenSquashAndStretch(xMinS, xMaxS, sliders, yMinS, yMaxS);

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		UpdateSliders(sliders, renderer, event);
		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');
	}

	quit_SDL();
	return 0;
}

