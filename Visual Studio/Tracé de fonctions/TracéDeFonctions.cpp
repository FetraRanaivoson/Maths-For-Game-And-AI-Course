//#include <iostream>
//using namespace std;
//
//#include <SDL.h>
//
////	size of window on screen
//constexpr auto WIDTH = 1600;
//constexpr auto HEIGHT = 600;
//constexpr auto ORIGIN_X = WIDTH / 2;
//constexpr auto ORIGIN_Y = HEIGHT / 2;
//
////	include desired header files for libraries
//#include "../lib_Slider/Slider.h"
//#include "../lib_Point/Point.h"
//#include "TracéDeFonctions.h"
//
//enum { X_GAUCHE, X_DROIT, Y_BAS, Y_HAUT, COEFF_A, COEFF_B, SQUARE_COEFF};
//#define MAX_POINTS 1000
//
//void placePointsForSquareFunction(Point points [], Slider* sliders []) {
//	// y = x²
//	for (int x = 0; x < MAX_POINTS; x++) {
//		points[x].x = ORIGIN_X ;
//		points[x].y = ORIGIN_Y;
//	}
//
//}
//
//void displayParabolicFunction(Point points[], Slider* sliders[], SDL_Renderer* renderer) {
//	for (int x = 0; x < MAX_POINTS; x++) {
//		points[x].x = x;
//		points[x].y = sliders[SQUARE_COEFF]->getValue() * points[x].x * points[x].x + ORIGIN_Y;
//		points[x].x += ORIGIN_X; //OFFSET TO ORIGIN
//		points[x].draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 2);
//	}
//
//}
//
//void displayWavyFunction(Point points[], Slider* sliders[], SDL_Renderer* renderer) {
//	//y= ax² + by + c
//	double c = 15;
//	double b = 15;
//	for(int x = 0; x < MAX_POINTS; x++) {
//		points[x].x = x;
//		points[x].y = (sliders[SQUARE_COEFF]->getValue() * points[x].x * points[x].x + c) / (1 - b) + ORIGIN_Y;
//		//points[x].y = sliders[SQUARE_COEFF]->getValue() *  points[x].x * points[x].x + points[x].y + c + ORIGIN_Y;
//		points[x].x += ORIGIN_X; //OFFSET TO ORIGIN
//		points[x].draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 2);
//	}
//}
//
//
//
//void drawBottomLeftRectangle(SDL_Rect& rect, Slider* sliders []) {
//	rect.x = ORIGIN_X;
//	rect.y = ORIGIN_Y;
//	rect.w = sliders[X_GAUCHE]->getValue();
//	rect.h = - sliders[Y_BAS]->getValue();
//}
//
//void drawBottomRightRectangle(SDL_Rect& rect, Slider* sliders[]) {
//	rect.x += 1;
//	rect.w = sliders[X_DROIT]->getValue();
//	rect.h = - sliders[Y_BAS]->getValue();
//}
//
//void drawTopLeftRectangle(SDL_Rect& rect, Slider* sliders[]) {
//	rect.y -= 1; //monter de -1 en y
//	rect.x -= 1; //remettre x en place (origine) pour monter
//	rect.w = sliders[X_GAUCHE]->getValue();
//	rect.h = - sliders[Y_HAUT]->getValue();
//}
//
//void drawTopRightRectangle(SDL_Rect& rect, Slider* sliders[]) {
//	rect.x += 1; //Ajouter +1 à x à nouveau pour le rectangle à droite
//	rect.w = sliders[X_DROIT]->getValue();
//	rect.h = - sliders[Y_HAUT]->getValue();
//}
//
//void displayLinearFunction(Point pointA, Point pointB, Slider* sliders[], SDL_Renderer* renderer) {
//	//y = ax - b
//	double y = 0;
//	double x = 0;
//	double a = 0;
//	double b = 0;
//	a = sliders[COEFF_A]->getValue();
//	b = sliders[COEFF_B]->getValue();
//	//Finding points coordinates that intersect the line
//	if (x == 0) {
//		pointB.x = 0 + ORIGIN_X;
//		pointB.y = -b + ORIGIN_Y;
//		//Make an horizontal line between B and A
//		//pointA.x = ORIGIN_X + 100;
//		//pointA.y = -b + ORIGIN_Y;
//	}
//	if (y == 0) {
//		if (a == 0) {
//			//Horizontal line
//			pointA.x = 0 + ORIGIN_X;
//			pointA.y = - b + ORIGIN_Y;
//			
//		}
//		if (a != 0) {
//			pointA.x = (b / a) + ORIGIN_X;
//			pointA.y = 0 + ORIGIN_Y;
//		}
//		
//	}
//
//	pointA.draw(renderer, Color(240, 0, 0, SDL_ALPHA_OPAQUE), 10);
//	pointB.draw(renderer, Color(240, 0, 0, SDL_ALPHA_OPAQUE), 10);
//	SDL_RenderDrawLine(renderer, pointA.x, pointA.y, pointB.x, pointB.y);
//	cout << "(" << a << ")" << "(" << b << ")"<< endl;
//}
//
//
//double getSliderMaxRange(int sliderIndex) {
//	double maxRange = 0;
//	if (sliderIndex == X_GAUCHE || sliderIndex == Y_BAS)
//		maxRange = -500;
//	if (sliderIndex == X_DROIT || sliderIndex == Y_HAUT)
//		maxRange = 500;
//	if (sliderIndex == COEFF_A)
//		maxRange = 100.0;
//	if (sliderIndex == COEFF_B)
//		maxRange = 1000.0;
//	if (sliderIndex == SQUARE_COEFF)
//		maxRange = .5;
//	return maxRange;
//}
//
//double getSliderMinRange(int sliderIndex) {
//	double minRange = 0;
//	if (sliderIndex == X_GAUCHE || sliderIndex == Y_BAS)
//		minRange = 0;
//	if (sliderIndex == X_DROIT || sliderIndex == Y_HAUT)
//		minRange = 0;
//	if (sliderIndex == COEFF_A)
//		minRange = - 100.0;
//	if (sliderIndex == COEFF_B)
//		minRange = -1000.0;
//	if (sliderIndex == SQUARE_COEFF)
//		minRange = -.5;
//	return minRange;
//}
//
//double getSliderInitialValue(int sliderIndex) {
//	double initialValue = 0;
//	if (sliderIndex == X_GAUCHE || sliderIndex == Y_BAS)
//		initialValue = 250.0;
//	if (sliderIndex == X_DROIT || sliderIndex == Y_HAUT)
//		initialValue = -250.0;
//	if (sliderIndex == COEFF_A || sliderIndex == COEFF_B)
//		initialValue = 0;
//	if (sliderIndex == SQUARE_COEFF)
//		initialValue = .25;
//	return initialValue;
//}
//
//void placeSliders(Slider* sliders[])
//{
//	for (int i = 0; i < 7; i++) {
//		int x = 30 + (i / 2) * (200 + 30);
//		int y = HEIGHT - 100 + (i % 2) * 30;
//		double maxRange = getSliderMaxRange(i);
//		double minRange = getSliderMinRange(i);
//		double initialValue = getSliderInitialValue(i);
//		sliders[i] = new Slider(x, y, 200, minRange, maxRange, initialValue);
//	}
//}
//
//void setPainterToBlack(SDL_Renderer* renderer) {
//SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
//}
//
//void setPainterToWhite(SDL_Renderer* renderer) {
//	SDL_SetRenderDrawColor(renderer, 241, 241, 241, SDL_ALPHA_OPAQUE);
//}
//
////	entry point of application
//
//
//
//
//int main(int argc, char** argv) {
//
//#pragma region SDL initialization
//	// SDL initialization
//	SDL_Window* fenetre = NULL;
//	SDL_Renderer* renderer = NULL;
//
//	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
//		cout << "ERREUR : can't initialize SDL!" << endl;
//		exit(0);
//	}
//
//	//SDL_ShowCursor(SDL_DISABLE);	//	hide mouse cursor
//	SDL_ShowCursor(SDL_ENABLE);	//	show mouse cursor
//
//	//	create the window and its associated renderer
//	fenetre = SDL_CreateWindow("SDL template", 200, 100, WIDTH, HEIGHT, 0);
//	renderer = SDL_CreateRenderer(fenetre, 0, 0);
//#pragma endregion
//
//	SDL_Rect rect;
//	Slider* sliders[7];
//	placeSliders(sliders);
//
//	//Place points for linear function y = ax + b
//	Point pointA(ORIGIN_X, ORIGIN_Y, true);
//	Point pointB(ORIGIN_X, ORIGIN_Y, true);
//	
//	//Place points for curved function
//	Point points[MAX_POINTS];
//	placePointsForSquareFunction(points, sliders);
//
//
//	bool end = false;
//	while (!end) {
//		//	paint window in black (clear)
//		setPainterToBlack(renderer);
//		SDL_RenderClear(renderer);
//		setPainterToWhite(renderer);
//
//		//	- draw any desired graphical objects here
//		displayAxis(rect, sliders, renderer);
//		displayLinearFunction(pointA, pointB, sliders, renderer);
//		//displayParabolicFunction(points, sliders, renderer);
//		displayWavyFunction(points, sliders, renderer);
//
//		//Verify boundaries
//		for (int i = 0; i < MAX_POINTS; i++) {
//			if (points[i].x > ORIGIN_X + sliders[X_DROIT]->getValue()) {
//				points[i].draw(renderer, Color(0, 0, 0, SDL_ALPHA_OPAQUE), 2);
//			}
//			if (points[i].y > ORIGIN_Y - sliders[Y_BAS]->getValue()) {
//				points[i].draw(renderer, Color(0, 0, 0, SDL_ALPHA_OPAQUE), 2);
//			}
//			if (points[i].y < ORIGIN_Y - sliders[Y_HAUT]->getValue()) {
//				points[i].draw(renderer, Color(0, 0, 0, SDL_ALPHA_OPAQUE), 2);
//			}
//		}
//
//		//	****************  //
//		//	event management  //
//		//	****************  //
//	
//		//	- remove next event from queue
//		SDL_Event event;
//		SDL_PollEvent(&event);
//
//		//	- give event to objects for update if needed
//		//	affichage des sliders
//		drawAndUpdateSliders(sliders, renderer, event);
//		updatePointsForLinearFunction(pointA, event, pointB);
//		updatePointsForSquareFunction(points, event);
//
//		//	*********************  //
//		//	show rendering buffer  //
//		//	*********************  //
//		SDL_RenderPresent(renderer);
//	}
//
//#pragma region SDL quit
//	//	destroy window and quit SDL
//	SDL_DestroyWindow(fenetre);
//	SDL_Quit();
//#pragma endregion
//
//	return 0;
//}
//
//void displayAxis(SDL_Rect& rect, Slider* sliders[], SDL_Renderer* renderer)
//{
//	drawBottomLeftRectangle(rect, sliders);
//	SDL_RenderFillRect(renderer, &rect);
//	drawBottomRightRectangle(rect, sliders);
//	SDL_RenderFillRect(renderer, &rect);
//	drawTopLeftRectangle(rect, sliders);
//	SDL_RenderFillRect(renderer, &rect);
//	drawTopRightRectangle(rect, sliders);
//	SDL_RenderFillRect(renderer, &rect);
//}
//
//void updatePointsForSquareFunction(Point  points[], SDL_Event& event)
//{
//	for (int x = 0; x < MAX_POINTS; x++) {
//		points[x].update(event);
//	}
//}
//
//void updatePointsForLinearFunction(Point& pointA, SDL_Event& event, Point& pointB)
//{
//	pointA.update(event);
//	pointB.update(event);
//}
//
//void drawAndUpdateSliders(Slider* sliders[], SDL_Renderer* renderer, SDL_Event& event)
//{
//	for (int i = 0; i < 7; i++) {
//		sliders[i]->draw(renderer, event);
//	}
//}
//
//
