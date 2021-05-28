#include <iostream>
using namespace std;

#include <SDL.h>
constexpr auto FENETRE_WIDTH = 600;
constexpr auto FENETRE_HEIGHT = 600;
#include "../lib_Point/Point.h"
#include "../lib_Cloud/Cloud.h"
#include "../lib_Slider/Slider.h"

//	on donne des noms aux indices du tableau de pointeurs de sliders
enum { MARGE_GAUCHE, MARGE_HAUT, LONGUEUR, HAUTEUR, ESPACEMENT_X, ESPACEMENT_Y };

int main(int argc, char** argv) {

#pragma region SDL initialization
	// SDL initialization////
	SDL_Window* fenetre = NULL;
	SDL_Renderer* renderer = NULL;
	/////////////////////////

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "ERREUR : can't initialize SDL!" << endl;
		exit(0);
	}

	//SDL_ShowCursor(SDL_DISABLE);	//	hide mouse cursor
	SDL_ShowCursor(SDL_ENABLE);	//	show mouse cursor

	//	create the window and its renderer
	fenetre = SDL_CreateWindow("SDL template", 200, 100, FENETRE_WIDTH, FENETRE_HEIGHT, 0);
	renderer = SDL_CreateRenderer(fenetre, 0, 0);
#pragma endregion


	//	creation des 6 sliders
	Slider* sliders[6];
	for (int i = 0; i < 6; i++) {
		//	i    0  1  2  3  4  5
		//	i/2	 0  0  1  1  2  2
		//	i%2  0  1  0  1  0  1
		int x = 100 + (i / 2) * (100 + 30);
		int y = 550 + (i % 2) * 30;
		sliders[i] = new Slider(x, y, 120, 1, 300, 30);
	}

	
	//	rectangle utilise pour l'affichage
	SDL_Rect rect;
	
	while (true) {
		//	draw image
	//	**********

	//	- clear window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	- draw any desired graphical object
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		int largeurTotalePremièreLigne = 0; 
		int largeurTotaleDeuxièmeLigne = 0; 
		rect.x = sliders[MARGE_GAUCHE]->getValue();
		rect.y = sliders[MARGE_HAUT]->getValue();
		rect.w = sliders[LONGUEUR]->getValue();
		rect.h = sliders[HAUTEUR]->getValue();
		SDL_RenderFillRect(renderer, &rect);

		//	Affichage des rectangles de la première ligne
		do {
			rect.x += sliders[LONGUEUR]->getValue() + sliders[ESPACEMENT_X]->getValue();
			SDL_RenderFillRect(renderer, &rect);
			//L = abscisses en X du dernier rectangle affiché + sa longueur
			largeurTotalePremièreLigne = rect.x + rect.w; 

			//	Je me mets à la ligne et je sors du boucle si width > WIDTH
			if (largeurTotalePremièreLigne > FENETRE_WIDTH) {
				//Supprimer dabord le rectangle qui dépasse
				rect.w = 0;
				rect.h = 0;
				SDL_RenderFillRect(renderer, &rect);
				//Se mettre à la ligne
				rect.x = sliders[MARGE_GAUCHE]->getValue();
				rect.y += sliders[MARGE_HAUT]->getValue() + sliders[ESPACEMENT_Y]->getValue();
				//Réafficher les réctangles sur la deuxième ligne
				rect.w = sliders[LONGUEUR]->getValue();
				rect.h = sliders[HAUTEUR]->getValue();
				SDL_RenderFillRect(renderer, &rect);
			}
			// Tant que largeurTotalePremièreLigne < FENETRE_WIDTH,  j'affiche des rectangles en haut
		} while (largeurTotalePremièreLigne < FENETRE_WIDTH); 

		cout << largeurTotalePremièreLigne << endl;

		//	Affichage des rectangles de la deuxième ligne
		do {
			rect.x += sliders[LONGUEUR]->getValue() + sliders[ESPACEMENT_X]->getValue();
			SDL_RenderFillRect(renderer, &rect);
			//	l = abscisses en X du dernier rectangle affiché de la deuxième + sa longueur
			largeurTotaleDeuxièmeLigne = rect.x + rect.w; 
			
		} while (largeurTotalePremièreLigne > FENETRE_WIDTH && largeurTotalePremièreLigne > largeurTotaleDeuxièmeLigne );





		//	event management
		//	****************

		//	remove next event from queue
		SDL_Event event;
		SDL_PollEvent(&event);



		//	give event to objects for update
			//	affichage des sliders
		for (int i = 0; i < 6; i++) {
			sliders[i]->draw(renderer, event);
		}


		//	show rendering buffer
		//	*********************
		SDL_RenderPresent(renderer);

		//	check keypress for exit
	//	***************************
		if (event.type == SDL_KEYDOWN) {
			break;
		}

	}

#pragma region SDL quit
	//	destroy window and quit SDL
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
#pragma endregion

	return 0;
}