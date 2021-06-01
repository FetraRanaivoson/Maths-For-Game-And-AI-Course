#include <iostream>
using namespace std;

#include <SDL.h>
#include <math.h>
#include "../lib_Point/Point.h"


double calculerAngle(Vector* u, Vector* v) {

	//u->operator*(*v);
						//Point A c'est l'origine (0,0)

	//VECTEUR U (Du point A au point B)
	double Bx = u->x; //Coordonéé du point B en x
	double By = u->y;  //Coordonéé du point B en y


	//VECTEUR V (Du point A au point C)
	double Cx = v->x;  //Coordonéé du point C en x
	double Cy = v->y;  //Coordonéé du point C en y


					// a*c  +   b*d		/	||u|| *  ||v||
	double cosUV = (Bx * Cx + By * Cy) / (sqrt(Bx * Bx + By * By)   *   sqrt(Cx * Cx + Cy * Cy));
										 //  (x2 - x1)² + (y2-y1)²
										 // x1 = 0, y1 = 0
	return acos(cosUV) * 180 / M_PI; // acos(cosUV)
	// 180deg ----> M_PI rad
	//	  ?	  <---- acos(cosUV) rad
}

double calculerHauteur(double& alphaRadian, double& hauteurPersonne, double& distanceAuTour) {
	double hauteurDeLaTour = distanceAuTour * tan(alphaRadian) + hauteurPersonne;
	alphaRadian = 5;
	return hauteurDeLaTour;
}



int main(int argc, char** argv) {

#pragma region Angle formé par deux vecteurs
	//Le vecteur u est formé par les deux points A et B. A est l'origine
	Point pointA(0,0,true);
	Point pointB(15, 5, true);
	//Création du vecteur u
	Vector* u = new Vector(pointA, pointB);
	
	//Le vecteur v est formé par les deux points A et C.
	Point pointC(7, 200, true);
	//Création du vecteur v
	Vector* v = new Vector(pointA, pointC);

	//Calcul de l'angle
	//cout << calculerAngle(u, v) << endl;
#pragma endregion
	
#pragma region Calcul hauteur de la tour
	//Calcul de la hauteur du tour
	double alphaDegré = 60;
	double alphaRadian = alphaDegré * M_PI / 180;
	double hauteurPersonne = 1.78;
	double distanceAuTour = 15;
	//cout << calculerHauteur(alphaRadian, hauteurPersonne, distanceAuTour) << endl;
#pragma endregion


	
	return 0;
}

