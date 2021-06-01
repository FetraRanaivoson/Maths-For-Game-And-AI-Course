#include <iostream>
using namespace std;

#include <SDL.h>

//	size of window on screen
constexpr auto WIDTH = 600;
constexpr auto HEIGHT = 600;

//	hauteur de la zone des sliders en bas de la fenetre
constexpr auto HAUTEUR_SLIDERS = 100;

//	include desired header files for libraries
#include "../lib_Slider/Slider.h"

//	on donne des noms aux indices du tableau de pointeurs de sliders
enum { MARGE_GAUCHE, MARGE_HAUT, LONGUEUR, HAUTEUR, ESPACEMENT_X, ESPACEMENT_Y };

void premierRectangle(SDL_Rect& rect, Slider* sliders[]) {
	rect.x = sliders[MARGE_GAUCHE]->getValue();
	rect.y = sliders[MARGE_HAUT]->getValue();
	rect.w = sliders[LONGUEUR]->getValue();
	rect.h = sliders[HAUTEUR]->getValue();
}

void rectangleDroit(SDL_Rect& rect, Slider* sliders[]) {
	rect.x += sliders[LONGUEUR]->getValue() + sliders[ESPACEMENT_X]->getValue();
}

void rectangleRetourLigne(SDL_Rect& rect, Slider* sliders[]) {
	rect.x = sliders[MARGE_GAUCHE]->getValue();		//	retour au debut de la ligne
	rect.y += sliders[HAUTEUR]->getValue() + sliders[ESPACEMENT_Y]->getValue();	//	ligne suivante
}

bool rectangleVisible(SDL_Rect& rect) {
	return rect.x + rect.w <= WIDTH && rect.y + rect.h <= 500;
}

int calculerLignes(Slider sliders[], int with, int height) {
	//	a faire
	return -1;
}

int calculerColonnes(Slider sliders[], int with, int height) {
	//	a faire
	return -1;
}

//	entry point of application
int main(int argc, char** argv) {

#pragma region SDL initialization
	// SDL initialization
	SDL_Window* fenetre = NULL;
	SDL_Renderer* renderer = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "ERREUR : can't initialize SDL!" << endl;
		exit(0);
	}

	//SDL_ShowCursor(SDL_DISABLE);	//	hide mouse cursor
	SDL_ShowCursor(SDL_ENABLE);	//	show mouse cursor

	//	create the window and its associated renderer
	fenetre = SDL_CreateWindow("SDL template", 1920 + 200, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(fenetre, 0, 0);
#pragma endregion

	//	****************************  //
	//	prepare usefull objects here  //
	//	****************************  //

	//	creation des 6 sliders
	Slider* sliders[6];
	for (int i = 0; i < 6; i++) {
		//	i    0  1  2  3  4  5
		//	i/2	 0  0  1  1  2  2
		//	i%2  0  1  0  1  0  1
		int x = 20 + (i / 2) * (100 + 30);
		int y = HEIGHT - HAUTEUR_SLIDERS + 50 + (i % 2) * 30;
		sliders[i] = new Slider(x, y, 100, 0, 100, 30);
	}

	//	rectangle utilise pour l'affichage
	SDL_Rect rect;

	//	**************  //
	//	main loop here  //
	//	**************  //
	while (true) {
		//	******************************  //
		//	draw image in rendering buffer  //
		//	******************************  //

		//	- clear window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	- draw any desired graphical objects here

		//	affichage de la ligne qui separe les 2 zones (zone des rectangles et zone des sliders)
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, 0, 500, WIDTH, 500);

		//	affichage du rectangle en haut a gauche
		SDL_SetRenderDrawColor(renderer, 200, 255, 50, SDL_ALPHA_OPAQUE);
		int nbLignes = 0, nbRectangles = 0;
		bool finAffichage = false;
		premierRectangle(rect, sliders);
		while (!finAffichage) {
			if (rectangleVisible(rect)) {
				SDL_RenderFillRect(renderer, &rect);
				nbRectangles++;
				rectangleDroit(rect, sliders);
			}
			else {
				nbLignes++;
				rectangleRetourLigne(rect, sliders);
				if (!rectangleVisible(rect)) {
					finAffichage = true;
				}
			}
		}

		cout << nbRectangles << ", " << nbLignes << ", " << nbRectangles / nbLignes << endl;

		//	****************  //
		//	event management  //
		//	****************  //

		//	- remove next event from queue
		SDL_Event event;
		SDL_PollEvent(&event);

		//	- give event to objects for update if needed
		//	affichage des sliders
		for (int i = 0; i < 6; i++) {
			sliders[i]->draw(renderer, event);
		}

		//	*********************  //
		//	show rendering buffer  //
		//	*********************  //
		SDL_RenderPresent(renderer);

		//	***********************  //
		//	check keypress for exit  //
		//	***********************  //
		/*if (event.type == SDL_KEYDOWN) {
			break;
		}*/
	}

#pragma region SDL quit
	//	destroy window and quit SDL
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
#pragma endregion

	return 0;
}