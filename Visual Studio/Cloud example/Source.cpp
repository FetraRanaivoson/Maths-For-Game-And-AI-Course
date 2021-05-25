#include <iostream>
using namespace std;

#include "SDL.h"
#include <conio.h>
#include <time.h>

#include "../lib_Point/Point.h"
#include "../lib_Cloud/Cloud.h"
#include "../lib_Slider/Slider.h"

#include <vector>

#define WIDTH 800
#define HEIGHT 600

#define SET_RGB(r, g, b, valueR, valueG, valueB) r=(valueR); g = (valueG); b=(valueb);

//	fonctions pour exercices
void exercicePointLePlusProche(SDL_Renderer* renderer) {
	Cloud *ptr_cloud = new Cloud(5, WIDTH, HEIGHT);

	//	point à manipuler
	Point p(WIDTH / 2, HEIGHT / 2, true);

	bool fin = false;
	while (!fin) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_Event evt;
		SDL_PollEvent(&evt);

		//	on affiche le nuage de points
		ptr_cloud->draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), true, &evt);

		// on affiche le point le plus proche
		ptr_cloud->points[ptr_cloud->closestPoint(p)].draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), 10);

		//	on affiche le point
		p.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 5);

		//	on gère le déplacement du point p
		p.update(evt);

		//	on montre le rendu
		SDL_RenderPresent(renderer);
	}

	delete ptr_cloud;
}

void exerciceIsobarycentre(SDL_Renderer* renderer) {
	Cloud *ptr_cloud = new Cloud(5, WIDTH, HEIGHT);

	bool fin = false;
	while (!fin) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_Event evt;
		SDL_PollEvent(&evt);

		//	on affiche le nuage de points
		ptr_cloud->draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), false, &evt);

		//	on affiche l'isobarycentre du nuage
		ptr_cloud->computeIsobarycenter().draw(renderer, Color(255, 0, 255, SDL_ALPHA_OPAQUE), 5);

		//	on montre le rendu
		SDL_RenderPresent(renderer);
	}

	delete ptr_cloud;
}

void exerciceProduitScalaire(SDL_Renderer* renderer) {
	Cloud* ptr_cloud = new Cloud(2, WIDTH, HEIGHT);

	//	point à manipuler
	Point C(WIDTH / 2, HEIGHT / 2, true);

	bool fin = false;
	while (!fin) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_Event evt;
		SDL_PollEvent(&evt);

		//	on affiche le nuage de points
		ptr_cloud->draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), true, &evt);

		//	ex prod scalaire : calcul du produit scalaire AB.AC
		Point A = ptr_cloud->points[0];
		Point B = ptr_cloud->points[1];

		//	vecteur AB
		double ABx = B.x - A.x;
		double ABy = B.y - A.y;

		//	vecteur AC
		double ACx = C.x - A.x;
		double ACy = C.y - A.y;

		//	produit scalaire AB.AC
		double prodScalaire = ABx * ACx + ABy * ACy;

		Color c = (prodScalaire >= 0 ?
			Color(0, 255, 0, SDL_ALPHA_OPAQUE) :
			Color(255, 0, 0, SDL_ALPHA_OPAQUE));

		//	on gère le déplacement du point p
		C.update(evt);

		//	on affiche le point
		C.draw(renderer, c, 30);

		//	on montre le rendu
		SDL_RenderPresent(renderer);
	}

	delete ptr_cloud;
}

void exerciceProduitVectoriel(SDL_Renderer* renderer) {
	Cloud* ptr_cloud = new Cloud(2, WIDTH, HEIGHT);

	//	point à manipuler
	Point C(WIDTH / 2, HEIGHT / 2, true);

	bool fin = false;
	while (!fin) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_Event evt;
		SDL_PollEvent(&evt);

		//	on affiche le nuage de points
		ptr_cloud->draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), true, &evt);

		//	ex prod scalaire : calcul du produit scalaire AB.AC
		Point A = ptr_cloud->points[0];
		Point B = ptr_cloud->points[1];

		//	vecteur AB
		double ABx = B.x - A.x;
		double ABy = B.y - A.y;

		//	vecteur AC
		double ACx = C.x - A.x;
		double ACy = C.y - A.y;

		//	produit vectoriel AB.AC
		double prodVectoriel = ABx * ACy - ABy * ACx;

		Color c = (prodVectoriel >= 0 ?
			Color(0, 255, 0, SDL_ALPHA_OPAQUE) :
			Color(255, 0, 0, SDL_ALPHA_OPAQUE));

		//	on affiche le point
		C.draw(renderer, c, 30);

		//	on montre le rendu
		SDL_RenderPresent(renderer);

		//	on gère le déplacement du point p
		C.update(evt);
	}

	delete ptr_cloud;
}

void affichageSurHorloge(SDL_Renderer* renderer, int nbTraits, int rayonMin, int rayonMax) {
	double alpha, pasAlpha;

	alpha = 0;
	pasAlpha = 2 * M_PI / nbTraits;

	while (alpha < 2 * M_PI) {
		double x1 = WIDTH / 2 + rayonMin * cos(alpha);
		double y1 = HEIGHT / 2 - rayonMin * sin(alpha);
		double x2 = WIDTH / 2 + rayonMax * cos(alpha);
		double y2 = HEIGHT / 2 - rayonMax * sin(alpha);

		SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);

		alpha += pasAlpha;
	}
}

void afficherAiguille(SDL_Renderer* renderer, int taille, double alpha) {
	double x1 = WIDTH / 2;
	double y1 = HEIGHT / 2;
	double x2 = WIDTH / 2 + taille * cos(alpha);
	double y2 = HEIGHT / 2 - taille * sin(alpha);

	SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);
}

void horloge(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	//	heure de départ
	int h = 4;
	int m = 18;
	int s = 45;

	while (true) {
		//	effacement du buffer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	affichage de l'horloge

		//	- affichage du cadre
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		affichageSurHorloge(renderer, 1200, 248, 252);

		//	- affiche des petites marques
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
		affichageSurHorloge(renderer, 60, 240, 250);

		//	- affiche des grandes marques
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		affichageSurHorloge(renderer, 12, 230, 250);

		//	affichage de l'heure (= des aiguilles)
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		double alphaSecondes = M_PI / 2 - s * 2.0 * M_PI / 60;
		afficherAiguille(renderer, 225, alphaSecondes);

		double alphaMinutes = M_PI / 2 - (m + s / 60.0) * 2 * M_PI / 60;
		afficherAiguille(renderer, 200, alphaMinutes);

		double alphaHeures = M_PI / 2 - (h + m / 60.0 + s / 3600.0) * 2 * M_PI / 12;
		afficherAiguille(renderer, 170, alphaHeures);

		//	on montre le rendu
		SDL_RenderPresent(renderer);

		//	seconde suivante
		s++;
		if (s == 60) {
			s = 0;
			m++;

			if (m == 60) {
				m = 0;
				h++;

				if (h == 12)
					h = 0;
			}
		}

		SDL_Event event;
		SDL_PollEvent(&event);

		SDL_Delay(10);
	}
}

void exerciceInterieurPolygone(SDL_Renderer* renderer) {
	Cloud* ptr_cloud = new Cloud(8, WIDTH, HEIGHT);

	//	point à manipuler
	Point p(WIDTH / 2, HEIGHT / 2, true);

	Point* ptrPointCourant = NULL;		//	adresse du point que la souris déplace

	bool fin = false;
	bool middleButtonPressed = false;
	while (!fin) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_Event evt;
		SDL_PollEvent(&evt);

		//	on affiche le nuage de points
		ptr_cloud->draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), true, &evt);

		//	on gère le déplacement des points avec la souris
		p.update(evt);

		//	on affiche le point selon 
		Color c(255, 0, 0, SDL_ALPHA_OPAQUE);
		if (ptr_cloud->contains(p))
			c = Color(0, 255, 0, SDL_ALPHA_OPAQUE);
		p.draw(renderer, c, 5);

		//	on montre le rendu
		SDL_RenderPresent(renderer);


		//	suppression d'un sommet
		/*
		if (bouton == SDL_BUTTON(SDL_BUTTON_MIDDLE)) {	//	suppression d'un sommet
			if (!middleButtonPressed) {
				Point souris(x, y);
				cloud.supprimerPoint(cloud.pointPlusProche(souris));
				middleButtonPressed = true;
			}
		}
		else
			middleButtonPressed = false;
			*/
	}

	delete ptr_cloud;
}

//	les 3 points A, B et C ne doivent pas être alignés
Point geolocaliser(SDL_Renderer* renderer, Point A, Point B, Point C, double dA, double dB, double dC) {
	Point O = A;

	A.drawCircle(renderer, (int)(dA + 0.5), Color(255, 0, 0, SDL_ALPHA_OPAQUE), true);
	B.drawCircle(renderer, (int)(dB + 0.5), Color(0, 255, 0, SDL_ALPHA_OPAQUE), true);
	C.drawCircle(renderer, (int)(dC + 0.5), Color(0, 0, 255, SDL_ALPHA_OPAQUE), true);

	//	on positionne l'origine du repère en A (translation de vecteur -OA)
	B.x -= O.x; B.y -= O.y;
	C.x -= O.x; C.y -= O.y;
	A.x -= O.x; A.y -= O.y;	//	(0, 0)

	double alpha = (dA*dA - dB * dB + B.x*B.x + B.y*B.y) / 2;
	double beta = (dA*dA - dC * dC + C.x*C.x + C.y*C.y) / 2;

	double x, y;

	if (B.x != 0) {
		//	B.x n'est pas nul
		y = (beta *B.x - alpha * C.x) / (C.y*B.x - B.y*C.x);	//	le dénominateur vaut OB ^ OC = AB^AC. Il n'est pas nul sinon A, B et C seraient alignés
		x = (alpha - y * B.y) / B.x;
	}
	else {
		//	B.x est nul donc AB = (Oy)
		y = alpha / B.y;	//	B.y n'est pas nul sinon on a A=B et donc les points seraient alignés
		x = (beta - y * C.y) / C.x;	//	C.y n'est pas nul sinon A, B et C sont sur (Oy), et donc alignés
	}

	//	on se replace dans le repère original (translation de vecteur +OA)
	x += O.x;	y += O.y;

	Point(x, y).draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), 10);

	return Point(0, 0);
}

void exerciceGeolocalisation(SDL_Renderer* renderer) {
	Cloud* ptr_cloud = new Cloud(3, WIDTH, HEIGHT);

	//	point à manipuler
	Point p(WIDTH / 2, HEIGHT / 2, true);

	Point* ptrPointCourant = NULL;		//	adresse du point que la souris déplace

	bool fin = false;
	bool middleButtonPressed = false, rightButtonPressed = false;
	while (!fin) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_Event evt;
		SDL_PollEvent(&evt);

		//	on affiche le nuage de points
		ptr_cloud->draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), true, &evt);

		//	calcul de la position du point
		geolocaliser(renderer,
			ptr_cloud->points[0], ptr_cloud->points[1], ptr_cloud->points[2],
			sqrt(p.sqrDist(ptr_cloud->points[0])),
			sqrt(p.sqrDist(ptr_cloud->points[1])),
			sqrt(p.sqrDist(ptr_cloud->points[2])));

		//	on affiche le point
		p.update(evt);
		p.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 3);

		//	on montre le rendu
		SDL_RenderPresent(renderer);

		//	on gère le déplacement des points avec la souris


		int x, y, bouton;
		bouton = SDL_GetMouseState(&x, &y);

		//	drag and drop de points
		if (bouton == SDL_BUTTON(SDL_BUTTON_LEFT)) {
			if (ptrPointCourant == NULL) {
				Point souris(x, y);
				ptrPointCourant = &ptr_cloud->points[ptr_cloud->closestPoint(souris)];

				if (souris.sqrDist(p) < souris.sqrDist(*ptrPointCourant))
					ptrPointCourant = &p;
			}

			ptrPointCourant->x = x;
			ptrPointCourant->y = y;
		}
		else {
			ptrPointCourant = NULL;
		}

		//	suppression d'un sommet
		if (bouton == SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
			if (!middleButtonPressed) {
				Point souris(x, y);
				ptr_cloud->removePoint(ptr_cloud->closestPoint(souris));
				middleButtonPressed = true;
			}
		}
		else {
			middleButtonPressed = false;
		}

		//	ajout d'un sommet
		if (bouton == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			if (!rightButtonPressed) {
				//	recherche du point le plus pres de la souris et ajout d'un point apres lui, dans le nuage
				Point souris(x, y);
				ptr_cloud->addPoint(souris, ptr_cloud->closestPoint(souris));
				rightButtonPressed = true;
			}
		}
		else {
			rightButtonPressed = false;
		}
	}

	delete ptr_cloud;

	exit(0);
}

void testSlider(SDL_Renderer* renderer) {
	bool fin = false;
	Slider sliderRadius(50, 20, 200, 100, 300, 200);	//	slider for radius
	Slider sliderCenterX(50, 60, 200, 200, 400, 200);	//	slider for X center

	Point other(100, 100, true);

	while (!fin) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	get the event to give to objects
		SDL_Event evt;
		SDL_PollEvent(&evt);

		//	draw and get slider values
		double radius = sliderRadius.draw(renderer, evt);
		double posX = sliderCenterX.draw(renderer, evt);
		other.update(evt);

		//	draw circle with slider values
		Point(posX, 300).drawCircle(renderer, (int)radius, Color(255, 255, 255, SDL_ALPHA_OPAQUE), true);

		other.draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), 10);

		SDL_RenderPresent(renderer);

		//fin = testerTouche(SDL_SCANCODE_SPACE);
	}
}

void testElastique(SDL_Renderer *renderer, int nbPoints) {

	bool fin = false;

	Point* tabPoints = new Point[nbPoints]; //	les points a suivre

	Point p2(200, 130, true);	//	le point qui suit
	double vx = 0, vy = 0;
	double ax = 0, ay = 0;
	double deltaT = 0.01;
	double masse = 5;

	double kf = 0.4;	//	frottement

	//	definition de l'elastique
	int l = 40;		//	taille de l'elastique
	double k = 10;	//	coefficient d'elasticite

	while (!fin) {
		int t = clock();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//	get the event to give to objects
		SDL_Event evt;
		SDL_PollEvent(&evt);

		for (int i = 0; i < nbPoints; i++) {
			tabPoints[i].update(evt);
			tabPoints[i].draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), 5);
			SDL_RenderDrawLine(renderer, (int)tabPoints[i].x, (int)tabPoints[i].y, (int)p2.x, (int)p2.y);
		}

		p2.update(evt);
		p2.draw(renderer, { 255, 255,255, SDL_ALPHA_OPAQUE }, 5);

		SDL_RenderPresent(renderer);

		if (!p2.isDragged()) {

			//	mise a jour des informations du point qui suit
			//	- position
			p2.x = p2.x + vx * deltaT;
			p2.y = p2.y + vy * deltaT;

			//	- vitesse
			vx = vx + ax * deltaT;
			vy = vy + ay * deltaT;

			//	- acceleration

			//	calcul de la somme des forces appliquees au point
			double Fx = 0, Fy = 0;

			for (int i = 0; i < nbPoints; i++) {

				//	- elastiques
				double f_elastX = tabPoints[i].x - p2.x;
				double f_elastY = tabPoints[i].y - p2.y;

				//	l'elastique est-il tendu ?
				double d = sqrt(f_elastX * f_elastX + f_elastY * f_elastY);
				if (d > l) {
					//	normer le vecteur
					f_elastX /= d;
					f_elastY /= d;

					//	calcul de la norme de la force
					f_elastX *= (d - l) * k;
					f_elastY *= (d - l) * k;
				}
				else {
					f_elastX = 0;
					f_elastY = 0;
				}

				Fx += f_elastX;
				Fy += f_elastY;
			}

			//	- force de frottement
			Fx += -kf * vx;
			Fy += -kf * vy;

			//	- gravite
			Fx += 0;
			Fy += masse * 9.81;

			//	- du vent
			//Fx += -10;
			//Fy += 0;

			//	calcul de l'acceleration
			ax = Fx / masse;
			ay = Fy / masse;
		}
		else {
			vx = vy = ax = ay = 0;
		}

		while (((double)clock() - t) / (double)CLOCKS_PER_SEC < deltaT / 1.0);
	}
}

//	*******************
//	fonction principale
//	*******************
int main(int argc, char** argv) {

#pragma region initialisation SDL
	//	initialisation SDL
	SDL_Window* fenetre = NULL;
	SDL_Renderer* renderer = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "ERREUR : SDL non initialisé!" << endl;
		exit(0);
	}

	//SDL_ShowCursor(SDL_DISABLE);	//	cacher le curseur de la souris

	//	création de la fenêtre et de son renderer
	fenetre = SDL_CreateWindow("titre", 200, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(fenetre, 0, 0);

#pragma endregion

	srand((unsigned int)time(NULL));

	//exercicePointLePlusProche(renderer);
	//exerciceIsobarycentre(renderer);
	//exerciceProduitScalaire(renderer);
	//exerciceProduitVectoriel(renderer);
	//horloge(renderer);
	//exerciceInterieurPolygone(renderer);
	exerciceGeolocalisation(renderer);
	//testSlider(renderer);
	//testElastique(renderer, 2);

#pragma region sortie SDL
	//	suppression de la fenêtre et sortie SDL
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
#pragma endregion

	return 0;
}