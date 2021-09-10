#include "Labyrinth.h"

Knot* Labyrinth::exitKnot;

double Labyrinth::getDistance(Point start, Point end)
{
	double d = sqrt((start.x - end.x) * (start.x - end.x)
		+ (start.y - end.y) * (start.y - end.y));
	return d;
}

Labyrinth::Labyrinth(int width, int height)
{
	this->createWalls(width, height);

	//Le noeud d'arrivé
	Point exitPosition((double)width - rand() % 300, (double)height - rand() % 300);
	//Point exitPosition(500, 500);
	this->exitKnot = new Knot(exitPosition, exitPosition);

	//Le noeud de départ
	this->entryKnot = new Knot(Point(rand()%150, 20), this->exitKnot->getPosition());
	openList.push_back(this->entryKnot);
}

void Labyrinth::createWalls(int width, int height)
{
	//Create walls
	for (int i = 0; i < MAX_WALLS_COUNT; i++) {
		Point begin(rand() % width, rand() % height);
		SDL_Rect wall = { rand() % width + 10,rand() % height + 10,
						  rand() % MAX_WALL_WIDTH + MIN_WALL_WIDTH,
						  rand() % MAX_WALL_HEIGHT + MIN_WALL_HEIGHT };
		this->walls.push_back(new Wall(wall));
	}
}

void Labyrinth::draw(SDL_Renderer* renderer)
{
	for (Wall* wall : walls) {
		wall->draw(renderer);
	}

	this->entryKnot->draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 1);
	this->exitKnot->draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), 10);

}

Knot* Labyrinth::getExitKnot()
{
	return exitKnot;
}

void Labyrinth::findNearestPath(SDL_Renderer* renderer)
{
	if (!openList.empty()) {
		//Retirer de l'open-list un nœud N minimisant F=G+H
		Knot* N = nullptr;
		double minF = 10000;
		for (int i = 0; i < openList.size(); i++) {
			double currentKnotF = openList[i]->getF();
			if (currentKnotF < minF) {
				minF = currentKnotF;
				N = openList[i];
				openList.erase(openList.cbegin() + i);
			}
		}

		if (N->getPosition().x >= exitKnot->getPosition().x
			&& N->getPosition().y >= exitKnot->getPosition().y) {
			std::cout << "Arrivée" << std::endl;
			//Afficher le chemin en partant de N et en utilisant la propriété P : nœud précédent
			for (Knot* knot : closedList) {
				if (knot->getPredecessor()) {
					SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
					SDL_RenderDrawLine(renderer, knot->getPosition().x, knot->getPosition().y,
						knot->getPredecessor()->getPosition().x,
						knot->getPredecessor()->getPosition().y);
				}
			}
		}

		//placer N dans la closed-list
		this->closedList.push_back(N);
		for (Knot* knot : closedList) {
			knot->draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 1);
		}


		//Trouver les voisins de N
		std::vector<Knot*>neighboursOfN;
		neighboursOfN = N->getNeighBoursKnots(walls);
		for (Knot* knot : neighboursOfN) {
			knot->draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), 1);
		}

		//Pour chaque voisin V de N...
		for (Knot* V : neighboursOfN) {
			//...qui n'est pas dans la closed-list
			if (std::find(closedList.begin(), closedList.end(), V) != closedList.end()) {
				/* closed-list contient V */
			}
			else {
				/* closed-list ne contient pas V */
				//a- Calculer H(V): distance of the knot to end : done inside getNeighBoursKnots

				//b- Calculer G(V) = G(N) + coût(N->V)
				V->setG(N->getG() + this->getDistance(N->getPosition(), V->getPosition()));

				//c- F(V) = G(V) + H(V)
				V->setF(V->getG() + V->getH());

				//d- et P(V) = N (noeud qui précède V = N)
				V->setP(N);

				//Si V est dans l'open-list
				if (std::find(openList.begin(), openList.end(), V) != openList.end()) {
					for (Knot* knot : openList) {
						//Mettre à jour G et P du nœud de l'open-list 
						//si son coût G est supérieur à celui de V
						if (knot->getG() > V->getG()) {
							knot->setG(V->getG());
							//knot->setP(N);
						}
					}
				}
				//Sinon ajouter V dans l'open-list (i.e noeud à traiter)
				else {
					openList.push_back(V);
				}
			}
		}
	}
}

