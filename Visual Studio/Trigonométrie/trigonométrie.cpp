#include <iostream>
using namespace std;

#include <SDL.h>
#include <math.h>
#include "../lib_Point/Point.h"


constexpr auto WIDTH = 1600;
constexpr auto HEIGHT = 600;

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
	window = SDL_CreateWindow(title, 200, 500, WIDTH, HEIGHT, 0);
	return SDL_CreateRenderer(window, 0, 0);
#pragma endregion
}


double calculerAngle(const Vector& u, const Vector& v) {

	//u->operator*(*v);
						//Point A c'est l'origine (0,0)
	
	//VECTEUR U (Du point A au point B)
	double Bx = u.x; //Coordon�� du point B en x
	double By = u.y;  //Coordon�� du point B en y


	//VECTEUR V (Du point A au point C)
	double Cx = v.x;  //Coordon�� du point C en x
	double Cy = v.y;  //Coordon�� du point C en y


				// a*c  +   b*d	 / ||u|| *  ||v||
				// Bx * Cx + By * Cy
	double cosUV = (u.operator*(v)) / (sqrt(Bx * Bx + By * By)   *   sqrt(Cx * Cx + Cy * Cy));
										 //  (x2 - x1)� + (y2-y1)�
										 // x1 = 0, y1 = 0
	return acos(cosUV) * 180 / M_PI; // acos(cosUV)
	// 180deg ----> M_PI rad
	//	  ?	  <---- acos(cosUV) rad
}

double calculerHauteur(double& alpha, double& hauteurPersonne, double& distanceAuTour) {
	double alphaRadian = alpha * M_PI / 180;
	double hauteurDeLaTour = distanceAuTour * tan(alphaRadian) + hauteurPersonne;
	alphaRadian = 5;
	return hauteurDeLaTour;
}




double calculerD(double alpha, double beta, double distanceEntreDeuxPersonnes)
{
	double alphaRadian = alpha * M_PI / 180;
	double betaRadian = beta * M_PI / 180;

	//En utilisant la loi des sinus
	double hypot�nuseBeta = sin(alphaRadian) * distanceEntreDeuxPersonnes
		/ sin(M_PI - (alphaRadian + betaRadian));
	
	return hypot�nuseBeta * sin(betaRadian);
}

int main(int argc, char** argv) {

//SDL_Renderer* renderer = init_SDL("Trace de fonctions");


#pragma region Angle form� par deux vecteurs
	//Le vecteur u est form� par les deux points A et B. A est l'origine
	Point pointA(0,0,true);
	Point pointB(15, 5, true);
	//Cr�ation du vecteur u
	Vector u = Vector(pointA, pointB);
	
	//Le vecteur v est form� par les deux points A et C.
	Point pointC(7, 200, true);
	//Cr�ation du vecteur v
	Vector v = Vector(pointA, pointC);

	//Calcul de l'angle
	//cout << calculerAngle(u, v) << endl;
#pragma endregion
	
#pragma region Calcul hauteur de la tour
	//Calcul de la hauteur du tour
	double alphaDegr� = 60;
	double hauteurPersonne = 1.78;
	double distanceAuTour = 15;
	//cout << calculerHauteur(alphaDegr�, hauteurPersonne, distanceAuTour) << endl;
#pragma endregion


#pragma region Calcul de la distance d: bateau - port
	double distanceEntreDeuxPersonnes = 100;
	double alpha = 40;
	double beta = 40;
	cout << calculerD(alpha, beta, distanceEntreDeuxPersonnes) << endl;
#pragma endregion

	
	return 0;
}

