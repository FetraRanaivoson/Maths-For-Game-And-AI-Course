#pragma once

void AfficherPersonnesEtBateau(Point& personneA, SDL_Renderer* renderer, Point& personneB, Point& bateau);

void AfficherLeTriangle(SDL_Renderer* renderer, Point& personneA, Point& personneB, Point& bateau);

void CalculerAlphaEtBeta(Point& personneB, Point& personneA, Point& bateau, double& alpha, double& beta);

void CalculerDistanceEntreDeuxPersonnes(double& distanceEntreDeuxPersonnes, Point& personneB, Point& personneA);

void AfficherAbEtAc(Point& pointA, SDL_Renderer* renderer, Point& pointB, Point& pointC);

void AfficherLesVecteursAbEtAc(Vector& u, Point& pointA, Point& pointB, Vector& v, Point& pointC, SDL_Renderer* renderer);
