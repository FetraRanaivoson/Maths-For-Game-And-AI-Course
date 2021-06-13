#include <iostream>
using namespace std;

#include <SDL.h>
#include <math.h>
#include "../lib_Point/Point.h"
#include "trigonométrie.h"


constexpr auto WIDTH = 600;
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
	window = SDL_CreateWindow(title, 200, 200, WIDTH, HEIGHT, 0);
	return SDL_CreateRenderer(window, 0, 0);
#pragma endregion
}
void clearWindow(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}
SDL_Event getNextEvent() {
	SDL_Event event;
	SDL_PollEvent(&event);

	return event;
}
bool keypressed(SDL_Event& event, char key) {
	return event.type == SDL_KEYDOWN && event.key.keysym.sym == key;
}
void showRenderingBuffer(SDL_Renderer* renderer) {
	SDL_RenderPresent(renderer);
}
void quit_SDL() {
#pragma region SDL quit
	//	quit SDL
	SDL_Quit();
#pragma endregion
}

double calculerAngle(const Vector& u, const Vector& v) {

	//u->operator*(*v);
						//Point A c'est l'origine (0,0)
	
	//VECTEUR U (Du point A au point B)
	double Bx = u.x; //Coordonéé du point B en x
	double By = u.y;  //Coordonéé du point B en y


	//VECTEUR V (Du point A au point C)
	double Cx = v.x;  //Coordonéé du point C en x
	double Cy = v.y;  //Coordonéé du point C en y


				// a*c  +   b*d	 / ||u|| *  ||v||
				// Bx * Cx + By * Cy
	double cosUV = u.operator*(v) / (sqrt(Bx * Bx + By * By)   *   sqrt(Cx * Cx + Cy * Cy));
										 //  (x2 - x1)² + (y2-y1)²
										 // x1 = 0, y1 = 0
	double sinUV = u.operator^(v) / (sqrt(Bx * Bx + By * By) * sqrt(Cx * Cx + Cy * Cy));

	double angle = acos(cosUV) * 180 / M_PI; // acos(cosUV)
										// 180deg ----> M_PI rad
										//	  ?	  <---- acos(cosUV) rad
	/*
	if (sinUV < 0) {
		angle = 360 - angle;    //cos (a) = cos (360 -a) //sin(a) = sin (180-a)
	}
	*/

	return angle;
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

	//En utilisant la loi des sinus: sin (Teta) / L = sin (alpha) / BC
	double hypoténuseBeta = sin(alphaRadian) * distanceEntreDeuxPersonnes
		/ sin(M_PI - (alphaRadian + betaRadian));
	
	return hypoténuseBeta * sin(betaRadian);
}

void AfficherPersonnesEtBateau(Point& personneA, SDL_Renderer* renderer, Point& personneB, Point& bateau)
{
	personneA.draw(renderer, Color(255, 120, 35, SDL_ALPHA_OPAQUE), 5);
	personneB.draw(renderer, Color(255, 120, 35, SDL_ALPHA_OPAQUE), 5);
	bateau.draw(renderer, Color(255, 120, 35, SDL_ALPHA_OPAQUE), 10);
}

void AfficherLeTriangle(SDL_Renderer* renderer, Point& personneA, Point& personneB, Point& bateau)
{
	SDL_RenderDrawLine(renderer, personneA.x, personneA.y, personneB.x, personneB.y);
	SDL_RenderDrawLine(renderer, personneA.x, personneA.y, bateau.x, bateau.y);
	SDL_RenderDrawLine(renderer, personneB.x, personneB.y, bateau.x, bateau.y);
}

void CalculerAlphaEtBeta(Point& personneB, Point& personneA, Point& bateau, double& alpha, double& beta)
{
	Vector AB = Vector(personneB.x - personneA.x, personneB.y - personneA.y);
	Vector AC = Vector(bateau.x - personneA.x, bateau.y - personneA.y);
	Vector BA = Vector(personneA.x - personneB.x, personneA.y - personneB.y);
	Vector BC = Vector(bateau.x - personneB.x, bateau.y - personneB.y);
	alpha = calculerAngle(AB, AC);
	beta = calculerAngle(BA, BC);
}

void CalculerDistanceEntreDeuxPersonnes(double& distanceEntreDeuxPersonnes, Point& personneB, Point& personneA)
{
	distanceEntreDeuxPersonnes = sqrt((personneB.x - personneA.x) * (personneB.x - personneA.x)
		+ (personneB.y - personneB.y) * (personneB.y - personneB.y));

}

void AfficherAbEtAc(Point& pointA, SDL_Renderer* renderer, Point& pointB, Point& pointC)
{
	pointA.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), 10);
	pointB.draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 5);
	pointC.draw(renderer, Color(0, 0, 255, SDL_ALPHA_OPAQUE), 5);
}

void AfficherLesVecteursAbEtAc(Vector& u, Point& pointA, Point& pointB, Vector& v, Point& pointC, SDL_Renderer* renderer)
{
	//Création du vecteur u
	u = Vector(pointA, pointB);
	//Création du vecteur v
	v = Vector(pointA, pointC);
	//Display vector 
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, pointA.x, pointA.y, pointB.x, pointB.y);
	SDL_RenderDrawLine(renderer, pointA.x, pointA.y, pointC.x, pointC.y);
}

int main(int argc, char** argv) {

SDL_Renderer* renderer = init_SDL("Trigonométrie");

	/*	prepare useful objects here	*/
#pragma region Angle formé par deux vecteurs
	//Le vecteur u est formé par les deux points A et B. A est l'origine
Point pointA(0, 0, true);
Point pointB(150, 50, true);
Point pointC(70, 200, true);
Vector u = Vector(pointA, pointB);
Vector v = Vector(pointA, pointC);
//Calcul de l'angle
//cout << calculerAngle(u, v) << endl;
#pragma endregion

#pragma region Calcul hauteur de la tour
//Calcul de la hauteur du tour
double alphaDegré = 60;
double hauteurPersonne = 1.78;
double distanceAuTour = 15;
//cout << calculerHauteur(alphaDegré, hauteurPersonne, distanceAuTour) << endl;
#pragma endregion

#pragma region Calcul de la distance d: bateau - port
Point personneA(150, 150, true);
Point personneB(200, 150, true);
Point bateau(175, 100, true);
double alpha = 0;
double beta = 0;
double distanceEntreDeuxPersonnes = 0;
//cout << calculerD(alpha, beta, distanceEntreDeuxPersonnes) << endl;
#pragma endregion

	bool endOfGame = false;

	while (!endOfGame) {
		//	******************************  //
		//	draw image in rendering buffer  //
		//	******************************  //
		clearWindow(renderer);

		/*	draw any desired graphical objects here	*/
		//	show drawing window

		//Bateau
		AfficherPersonnesEtBateau(personneA, renderer, personneB, bateau);
		AfficherLeTriangle(renderer, personneA, personneB, bateau);
		CalculerAlphaEtBeta(personneB, personneA, bateau, alpha, beta);
		CalculerDistanceEntreDeuxPersonnes(distanceEntreDeuxPersonnes, personneB, personneA);
		cout << calculerD(alpha, beta, distanceEntreDeuxPersonnes)<< endl;

		//Angle formé par U et V
		AfficherAbEtAc(pointA, renderer, pointB, pointC);
		AfficherLesVecteursAbEtAc(u, pointA, pointB, v, pointC, renderer);
		//cout << calculerAngle(u, v) << endl;

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		personneA.update(event);
		personneB.update(event);
		bateau.update(event);

		pointA.update(event);
		pointB.update(event);
		pointC.update(event);
		
		showRenderingBuffer(renderer);
		endOfGame = keypressed(event, 'q');
	}

	quit_SDL();
	
	return 0;
}

