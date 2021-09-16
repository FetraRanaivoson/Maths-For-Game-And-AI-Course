#include "Labyrinth2.h"

Node* Labyrinth::exitKnot;

double Labyrinth::getDistance(Point start, Point end)
{
	double d = sqrt((start.x - end.x) * (start.x - end.x)
		+ (start.y - end.y) * (start.y - end.y));
	return d;
}

Labyrinth::Labyrinth(int width, int height)
	: width(width), height(height)
{
	this->createWalls();
	this->createExitKnot();
	this->createEntryKnot();
	//this->openList.push_back(this->entryKnot);
}

Labyrinth::~Labyrinth()
{
	delete this->entryKnot;
	delete this->exitKnot;
	for (Wall* wall : this->walls) {
		delete wall;
	}
	for (Node* nodeCl : this->closedList) {
		delete nodeCl;
	}
	for (Node* nodeOl : this->openList) {
		delete nodeOl;
	}
}

void Labyrinth::createEntryKnot()
{
	bool ok = false;
	do {
		this->entryKnot = new Node(Point((rand() % 150) * 8.0, (rand() % 10) * 8.0), this->exitKnot->getPosition(), nullptr);
		if (this->knotInsideWall(this->entryKnot, walls)) {
			ok = false;
		}
		else
			ok = true;
	} while (!ok);
}

void Labyrinth::createExitKnot()
{
	bool ok = false;
	do {
		Point exitPosition((double)this->width - (rand() % 50) * 8.0, (double)this->height - (rand() % 50) * 8.0);
		this->exitKnot = new Node(exitPosition, exitPosition, nullptr);
		if (this->knotInsideWall(this->exitKnot, walls)) {
			ok = false;
		}
		else
			ok = true;
	} while (!ok);
}

void Labyrinth::createWalls()
{
	//Create walls
	for (int i = 0; i < MAX_WALLS_COUNT; i++) {
		Point begin(rand() % this->width, rand() % this->height);
		SDL_Rect wall = { rand() % this->width + MIN_WALL_WIDTH,
						  rand() % this->height + MIN_WALL_HEIGHT,
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
	this->entryKnot->draw(renderer, Color(255, 255, 0, SDL_ALPHA_OPAQUE), 5);
	this->exitKnot->draw(renderer, Color(255, 255, 0, SDL_ALPHA_OPAQUE), 5);

	this->droid->draw(renderer);
}



void Labyrinth::addDroid(Droid* droid)
{
	this->droid = droid;
}

Droid* Labyrinth::getDroid()
{
	return this->droid;
}

std::vector<Node*> Labyrinth::getPathNodes()
{
	return this->pathNodes;
}


void Labyrinth::findShortestPath(SDL_Renderer* renderer)
{
	if (!openList.empty()) {
		//Retirer de l'open-list un nœud N minimisant F=G+H
		Node* N = nullptr;
		double minF = 10000000;
		int minIndex = -1;
		this->FindN(minF, N, minIndex);

		if (this->isAtExitPoint(N)) {
			this->pathNodes = this->getPath(N, renderer);
			this->pathFound = true;
		}

		else {
			//placer N dans la closed-list. N: current knot
			this->openList.erase(this->openList.cbegin() + minIndex);
			this->closedList.push_back(N);
			for (Node* knot : this->closedList) {
				knot->draw(renderer, Color(255, 125, 0, SDL_ALPHA_OPAQUE), 1);
			}

			//Trouver les voisins de N
			std::vector<Node*>neighboursOfN;
			this->findNeighboursOfN(neighboursOfN, N, renderer);

			//Pour chaque voisin V de N...
			for (Node* V : neighboursOfN) {
				//...qui n'est pas dans la closed-list
				bool found = false;
				for (Node* knot : this->closedList) {
					if (knot->equal(V)) {
						found = true; //so do nothing, it is in the closed list, which we won't touch anymore
					}
				}
				if (!found) {
					/* closed-list ne contient pas V */
					this->updateNeighboursOfNHGFP(V, N);

					//Si V est dans l'open-list: recalculate their G cost from the parent of the current parent "N",
					//To see if it's interesting to start FROM that parent TO the current V 
					//And check if the path from that parent to the V is shorter  
					found = false;
					for (Node* openListknot : this->openList) {
						if (openListknot->equal(V)) {
							found = true;
							//IF the G of neighbour but calculated from previous parent >  The G of the same neighbour calculated from the actual parent N (line 127)
							if (openListknot->getG() > V->getG()) {
								//We take the lowest G cost from actual parent to the Knot and so we update the open-list knot to the lowest G cost
								openListknot->setG(V->getG()); //G(N) + c'
													//Set the parent of the knot in the open-list to the the actual knot 
								openListknot->setP(N); //P = N 
							}
						}
					}
					//Sinon ajouter V dans l'open-list (i.e noeud à traiter)
					if (!found) {
						this->openList.push_back(V);
					}
				}
			}
		}
	}
}

void Labyrinth::findShortestPath(Node* start, Node* end, SDL_Renderer* renderer)
{
	this->openList.clear();
	this->closedList.clear();
	this->openList.push_back(start);
	bool pathFound = false;
	while (!openList.empty() && !pathFound) {
		//Retirer de l'open-list un nœud N minimisant F=G+H
		Node* N = nullptr;
		double minF = 10000000;
		int minIndex = -1;
		this->FindN(minF, N, minIndex);

		if (this->isAtExitPoint(N, end) && !this->pathFound) {
			this->pathNodes = this->getPath(N, renderer);
			this->droid->setPath(pathNodes);
			pathFound = true;
		}
		else {
			//placer N dans la closed-list. N: current knot
			this->openList.erase(this->openList.cbegin() + minIndex);
			this->closedList.push_back(N);
			/*for (Node* knot : this->closedList) {
				knot->draw(renderer, Color(255, 125, 0, SDL_ALPHA_OPAQUE), 1);
			}*/

			//Trouver les voisins de N
			std::vector<Node*>neighboursOfN;
			this->findNeighboursOfN(neighboursOfN, N, renderer);

			//Pour chaque voisin V de N...
			for (Node* V : neighboursOfN) {
				//...qui n'est pas dans la closed-list
				bool found = false;
				for (Node* knot : this->closedList) {
					if (knot->equal(V)) {
						found = true; //so do nothing, it is in the closed list, which we won't touch anymore
					}
				}
				if (!found) {
					/* closed-list ne contient pas V */
					this->updateNeighboursOfNHGFP(V, N);

					//Si V est dans l'open-list: recalculate their G cost from the parent of the current parent "N",
					//To see if it's interesting to start FROM that parent TO the current V 
					//And check if the path from that parent to the V is shorter  
					found = false;
					for (Node* openListknot : this->openList) {
						if (openListknot->equal(V)) {
							found = true;
							//IF the G of neighbour but calculated from previous parent >  The G of the same neighbour calculated from the actual parent N (line 127)
							if (openListknot->getG() > V->getG()) {
								//We take the lowest G cost from actual parent to the Knot and so we update the open-list knot to the lowest G cost
								openListknot->setG(V->getG()); //G(N) + c'
													//Set the parent of the knot in the open-list to the the actual knot 
								openListknot->setP(N); //P = N 
							}
						}
					}
					//Sinon ajouter V dans l'open-list (i.e noeud à traiter)
					if (!found) {
						this->openList.push_back(V);
					}
				}
			}
		}
	}
	//this->getDroid()->wander(renderer, this->width, this->height);
}

bool Labyrinth::isPathFound()
{
	return this->pathFound;
}



Node* Labyrinth::getExitNode()
{
	return exitKnot;
}

Node* Labyrinth::getEntryNode()
{
	return this->entryKnot;
}

void Labyrinth::updateNeighboursOfNHGFP(Node* V, Node* N)
{
	//a- Calculer H(V): distance of the knot to end : done inside getNeighBoursKnots

	//b- Calculer G(V) = G(N) + coût(N->V). N: current knot
	V->setG(N->getG() + this->getDistance(N->getPosition(), V->getPosition()));

	//c- F(V) = G(V) + H(V)
	V->setF(V->getG() + V->getH());

	//d- et P(V) = N (noeud qui précède V = N)
	V->setP(N);
}

void Labyrinth::findNeighboursOfN(std::vector<Node*>& neighboursOfN, Node* N, SDL_Renderer* renderer)
{
	neighboursOfN = N->getNeighBoursKnots(this->walls);
	/*for (Node* neighbour : neighboursOfN) {
		neighbour->draw(renderer, Color(125, 0, 125, SDL_ALPHA_OPAQUE), 1);
	}*/
}

void Labyrinth::FindN(double& minF, Node*& N, int& minIndex)
{
	for (int i = 0; i < openList.size(); i++) {
		double currentKnotF = openList[i]->getF();
		if (currentKnotF < minF) {
			minF = currentKnotF;
			N = openList[i];
			minIndex = i;
		}
	}
}

std::vector <Node*> Labyrinth::getPath(Node*& N, SDL_Renderer* renderer)
{
	std::vector <Node*> pathNodes;
	//Afficher le chemin en partant de N et en utilisant la propriété P : nœud précédent
	pathNodes.push_back(N);
	while (N->getPredecessor() != nullptr) {
		Node* predecessor = N->getPredecessor();
		pathNodes.push_back(predecessor);

		SDL_RenderDrawLine(renderer, N->getPosition().x, N->getPosition().y,
			predecessor->getPosition().x,
			predecessor->getPosition().y);
		N = predecessor;
	}
	return pathNodes;
}

bool Labyrinth::isAtExitPoint(Node* N)
{
	return N->getPosition().x == this->exitKnot->getPosition().x && N->getPosition().y == this->exitKnot->getPosition().y;
}

bool Labyrinth::isAtExitPoint(Node* N, Node* end)
{
	return N->getPosition().x == end->getPosition().x && N->getPosition().y == end->getPosition().y;
}

bool Labyrinth::knotInsideWall(Node* knot, std::vector<Wall*>& walls)
{
	for (int i = 0; i < walls.size(); i++) {
		if (knot->getPosition().x > walls[i]->getWall().x - 6.0
			&& knot->getPosition().x < (double)walls[i]->getWall().x + walls[i]->getWall().w + 6.0
			&& knot->getPosition().y > walls[i]->getWall().y - 6.0
			&& knot->getPosition().y < (double)walls[i]->getWall().y + walls[i]->getWall().h + 6.0) {
			return true;
		}
	}
	return false;
}

