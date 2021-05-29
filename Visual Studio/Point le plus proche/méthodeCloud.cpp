#include <iostream>
using namespace std;

#include <SDL.h>
#include <time.h>

constexpr auto FENETRE_WIDTH = 600;
constexpr auto FENETRE_HEIGHT = 600;
#include "../lib_Point/Point.h"
#include "../lib_Cloud/Cloud.h"


#define WIDTH 800
#define HEIGHT 600

void exercicePointLePlusProche(SDL_Renderer* renderer) {
	Cloud* ptr_Cloud = new Cloud(10, WIDTH, HEIGHT);
	//point à manipuler
	Point p(WIDTH / 2, HEIGHT / 2, true);

	bool fin = false;
	while (!fin) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_Event evt;
		SDL_PollEvent(&evt);

		//	on affiche le nuage de points
		ptr_Cloud->draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), false, &evt);

		//	On affiche le point le plus proche
		//1-	On déréférence le pointeur pour chercher un point spécifique dans le Array of Clouds
		//2-	Pour le trouver, on utilise la méthode: closestPoint(const Point& p) qui donne un int (index)
		Point thePoint = ptr_Cloud->points[ptr_Cloud->closestPoint(p)];
		thePoint.draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), 5);
		thePoint.drawCircle(renderer, 25, Color(255, 0, 0, SDL_ALPHA_OPAQUE), true);
		//Plus la distance diminue, plus le rayon devient petit
		thePoint.drawCircle(renderer, sqrt(thePoint.sqrDist(p)), Color(75, 235, 120, SDL_ALPHA_OPAQUE), false);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, p.x, p.y, thePoint.x, thePoint.y);


		//	on affiche le point
		p.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 5);

		//	on gère le déplacement du point p
		p.update(evt);

		//	on montre le rendu
		SDL_RenderPresent(renderer);
	}
	delete ptr_Cloud;
}


int main(int argc, char** argv) {
#pragma region SDL initialization
	// SDL initialization////
	SDL_Window* fenetre = NULL;
	SDL_Renderer* renderer = NULL;
	
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

	srand((unsigned int)time(NULL));

	exercicePointLePlusProche(renderer);

	#pragma region sortie SDL
	//	suppression de la fenêtre et sortie SDL
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
	#pragma endregion

	return 0;
}
