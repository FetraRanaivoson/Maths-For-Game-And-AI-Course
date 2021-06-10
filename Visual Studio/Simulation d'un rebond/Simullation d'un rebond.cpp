#include <iostream>
using namespace std;

#include <SDL.h>
#include <time.h>
#include <math.h>

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 500, POS_Y = 200;		
constexpr auto WIDTH = 800, HEIGHT = 600;

//	include desired header files for libraries
#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"

enum {VITESSE_X, VITESSE_Y, ACCELERATION_X, ACCELERATION_Y};



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

//Get elapsed time in miliiseconds
clock_t readTimeMillis() {
	return clock();
}

clock_t getElapsedTimeMillis(clock_t start, clock_t end) {
	return end - start;
}

//get elapsed time in microseconds
__time64_t readTimeNano() {
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}
	return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

__time64_t getElapsedTimeNano(__time64_t start, __time64_t end)
{
	return end - start;
}


double getSliderMaxRange(int sliderIndex) {
	double maxRange = 0;
	if (sliderIndex == VITESSE_X || sliderIndex == VITESSE_Y)
		maxRange = 50000;
	if (sliderIndex == ACCELERATION_X || sliderIndex == ACCELERATION_Y)
		maxRange = 50000;
	return maxRange;
}

double getSliderMinRange(int sliderIndex) {
	double minRange = 0;
	if (sliderIndex == VITESSE_X || sliderIndex == VITESSE_Y)
		minRange = 0;
	if (sliderIndex == ACCELERATION_X)
		minRange = 0;
	if (sliderIndex == ACCELERATION_Y)
		minRange = 0;
	return minRange;
}

void placeSliders(Slider* sliders[])
{
	for (int i = 0; i < 4; i++) {
		int x = 30 + (i / 2) * (200 + 30);
		int y = HEIGHT - 100 + (i % 2) * 30;
		double maxRange = getSliderMaxRange(i);
		double minRange = getSliderMinRange(i);
		sliders[i] = new Slider(x, y, 200, minRange, maxRange, 0);
	}
}


void UpdateSliders(Slider* sliders[], SDL_Renderer* renderer, SDL_Event& event)
{
	for (int i = 0; i < 4; i++)
		sliders[i]->draw(renderer, event);
}

double vitesseY (Slider* sliders [], double resistance, int& fps) {
	return sliders[VITESSE_Y]->getValue() + (9.81 *2 - resistance) / 2 * ( (1.0/fps) + 1 );
}

double vitesseX (Slider* sliders[], int& fps) {
	return sliders[VITESSE_X]->getValue() + sliders[ACCELERATION_X]->getValue() * ((1.0 / fps) + 1);
}

double positionY(Slider* sliders[], int position, int& fps) {
	return position + sliders[VITESSE_Y]->getValue() * ((1.0 / fps) + 1);
}

double fonctionE(double x) {
	return 1 * exp (x);
}

double sgn(double x) {
	if (x < 0)
		return -1;
	else
		return 1;
}


int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Simulation d'un rebond");	//	this object will draw in our window
	/*	prepare useful objects here	*/
	Point object(400, 20, true);
	int objectSize = 50;
	double vtY = 100; //pixel par sec
	double vtX = 0;	//pixel par sec

	__time64_t startNano= readTimeNano(), endNano;

	Slider* sliders[2];
	placeSliders(sliders);

	//int fps = 5000;
	

	//int y = 0;

	//double resistance = 9.81 *2;

	bool endOfGame = false;


	while (!endOfGame) {

		
	
		clearWindow(renderer);

		/*	draw any desired graphical objects here	*/
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	
		//vtY = vitesseY(sliders, resistance, fps); //Initially, resistance = 9.72. If resistance < 9.72 then gravity wins!
		//vtX = vitesseX(sliders, fps);
		//resistance -= .006;  //Drop resistance so that gravity gradually wins!

		////NO COLLISION IN Y
		//if  (object.y > 0 && object.y <= HEIGHT) {
		//	object.y = object.y + vtY * ((1.0 / fps) + 1);			// Vt = 100px			  ----> 24 fr
		//															//? 100/24 = 4,16 px = 4px    <---- 1 fr

		//													// Vt = 100px					   ----> ~= 5000fr
		//													// 100/5000 =  0.02px (+1) = 1px   <----  1fr
		//													// CAD position +1 px par boucle


		//												//  Pour 6s		   ----> 600px
		//												//  24/600 = 0.04s <---- 4px
		//}
		////COLLISION TOP
		//if (object.y <= 0 + objectSize / 2.0) {
		//	object.y = 0 + objectSize / 2.0;
		//}
		////COLLISION BOTTOM
		//else if (object.y >= HEIGHT - objectSize / 2.0 && vtY >0) {
		//	object.y = HEIGHT - objectSize / 2.0;
		//	vtY *= -0.9;
		//	//object.y = object.y - 10 * vtY * ((1.0 / fps) + 1);
		//}

		//////NO COLLISION IN X
		//if (object.x > 0 && object.x <= WIDTH) {
		//	object.x = object.x + vtX * ((1.0 / fps) + 1);
		//}
		////COLLISION LEFT
		//if (object.x <= 0 + objectSize / 2.0) {
		//	object.x = 0 + objectSize / 2.0;
		//}
		////COLLISION RIGHT
		//else if (object.x >= WIDTH - objectSize / 2.0) {
		//	object.x = WIDTH - objectSize / 2.0;
		//}


		//vtY = vitesseY(sliders, resistance, fps); //Initially, resistance = 9.72. If resistance < 9.72 then gravity wins!
		//object.y = object.y + vtY * ((1.0 / fps) + 1);
		

		//Pour la vitesse
		endNano = readTimeNano();
		__time64_t ellapsedNano = getElapsedTimeNano(startNano, endNano);
		double ellapsedNanoInSecond = ellapsedNano * 1e-9;
		startNano = endNano;

		object.x = object.x + vtX * ellapsedNanoInSecond;
		object.y = object.y + vtY * ellapsedNanoInSecond;
	

		//Pour l'accélération
		//vtX		 = sgn(vtX) *  sliders[VITESSE_X]->getValue();

		vtX		 = vtX		+ (9.81 + sliders[ACCELERATION_X]->getValue()) * ellapsedNanoInSecond;
		vtY		 = vtY      + (9.81 + sliders[ACCELERATION_Y]->getValue()) * ellapsedNanoInSecond; 

		if ((object.x > WIDTH && vtX > 0) ||
			(object.x < 0 && vtX < 0)) {
			vtX *= -0.9;
		}
		if ((object.y > HEIGHT && vtY > 0) ||
			(object.y < 0 && vtY < 0)) {
			vtY *= -0.9;
		}
		
		//object.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), objectSize);
		object.drawCircle(renderer, objectSize / 2, Color(255, 255, 255, SDL_ALPHA_OPAQUE),true);



		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		object.update(event);
		UpdateSliders(sliders, renderer, event);


		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');

	

		//fps = (int)((double)1e9 / ellapsedNano); 
		//cout << fps <<" FPS"<< endl;
		//cout << ellapsedNano << " nanosec ellapsed" << endl;
		//cout << ellapsedNanoInSecond << " sec ellapsed" << endl;
		cout << "Vitesse (x,y): " << vtX << "," << vtY << endl;
	}

	quit_SDL();
	return 0;
}

