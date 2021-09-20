#include "Labyrinth2.h"
Node* Labyrinth::entryNode;
Node* Labyrinth::exitNode;
Node* Labyrinth::randomExitPointNode;
std::vector <Wall*>Labyrinth::walls;
std::vector<Point>Labyrinth::resources;
std::vector <Node*>Labyrinth::closedList;
std::vector <Node*>Labyrinth::openList;
bool Labyrinth::pathFound = false;
std::vector<Node*>Labyrinth::pathNodes;
Droid* Labyrinth::droid;

int Labyrinth::width;
int Labyrinth::height;

double Labyrinth::getDistance(Point start, Point end)
{
	double d = sqrt((start.x - end.x) * (start.x - end.x)
		+ (start.y - end.y) * (start.y - end.y));
	return d;
}

Labyrinth::Labyrinth(int width, int height)
{
	this->width = width;
	this->height = height;

	this->createWalls();
	this->createExitNode();
	this->createEntryKnot();
	this->createRandomExitPointNode();
	//this->openList.push_back(this->entryKnot);
}
Labyrinth::~Labyrinth()
{
	delete this->entryNode;
	delete this->exitNode;
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
		this->entryNode = new Node(Point((rand() % 150) * 8.0, (rand() % 10) * 8.0), this->exitNode->getPosition(), nullptr);
		if (this->nodeInsideWall(this->entryNode, walls)) {
			ok = false;
		}
		else
			ok = true;
	} while (!ok);
}
void Labyrinth::createExitNode()
{
	bool ok = false;
	do {
		Point exitPosition((double)Labyrinth::width - (rand() % 50) * 8.0, (double)Labyrinth::height - (rand() % 50) * 8.0);
		Labyrinth::exitNode = new Node(exitPosition, exitPosition, nullptr);
		if (Labyrinth::nodeInsideWall(Labyrinth::exitNode, walls)) {
			ok = false;
		}
		else
			ok = true;
	} while (!ok);
}
void Labyrinth::createRandomExitPointNode() {
	bool ok = false;
	do {
		Point randomExitPosition((double)(rand() % Labyrinth::width / 8) * 8.0, (double)(rand() % Labyrinth::height/8) * 8.0);
		Labyrinth::randomExitPointNode = new Node(randomExitPosition, randomExitPosition, nullptr);
		if (Labyrinth::nodeInsideWall(Labyrinth::randomExitPointNode, walls)) {
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
	this->entryNode->draw(renderer, Color(255, 255, 0, SDL_ALPHA_OPAQUE), 5);
	this->exitNode->draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), 15);
	this->randomExitPointNode->draw(renderer, Color(125, 125, 0, SDL_ALPHA_OPAQUE), 25); 
	this->droid->draw(renderer);

	for (int i = 0; i < resources.size(); i++) {
		resources[i].draw(renderer, Color(125, 255, 0, SDL_ALPHA_OPAQUE), 10);
	}
}

void Labyrinth::addDroid(Droid* droid)
{
	this->droid = droid;
}

Droid* Labyrinth::getDroid()
{
	return Labyrinth::droid;
}

void Labyrinth::addResources(std::vector<Point> resources)
{
	for (int i = 0; i < resources.size(); i++) {
		this->resources.push_back(resources[i]);
	}
}

std::vector<Node*> Labyrinth::getPathNodes()
{
	return Labyrinth::pathNodes;
}

Node*& Labyrinth::getRandomExitPointNode()
{
	return Labyrinth::randomExitPointNode;
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
			this->pathNodes = this->getPath(N);
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
			this->findNeighboursOfN(neighboursOfN, N);

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


void Labyrinth::executeAstar(Point start, Point end)
{
	start.x = ((int)(start.x/8)) * 8.0 ;
	start.y = ((int)(start.y/8)) * 8.0 ;

	end.x = ((int)(end.x / 8)) * 8.0;
	end.y = ((int)(end.y / 8)) * 8.0;

	Node* startNode = new Node(start, end, nullptr);
	Node* endNode = new Node(end, end, nullptr);

	Labyrinth::openList.clear();
	Labyrinth::closedList.clear();
	Labyrinth::openList.push_back(startNode);
	

	bool pathFound = false;

	while (!openList.empty() && !pathFound) {
		//Retirer de l'open-list un nœud N minimisant F=G+H
		Node* N = nullptr;
		double minF = 10000000;
		int minIndex = -1;
		Labyrinth::FindN(minF, N, minIndex);

		if (Labyrinth::isAtExitPoint(N, endNode) && !Labyrinth::pathFound) {
			Labyrinth::pathNodes = Labyrinth::getPath(N);
			Labyrinth::droid->setPath(pathNodes);
			pathFound = true;
		}
		else {
			//placer N dans la closed-list. N: current knot
			Labyrinth::openList.erase(Labyrinth::openList.cbegin() + minIndex);
			Labyrinth::closedList.push_back(N);
			/*for (Node* knot : this->closedList) {
				knot->draw(renderer, Color(255, 125, 0, SDL_ALPHA_OPAQUE), 1);
			}*/

			//Trouver les voisins de N
			std::vector<Node*>neighboursOfN;
			Labyrinth::findNeighboursOfN(neighboursOfN, N);

			//Pour chaque voisin V de N...
			for (Node* V : neighboursOfN) {
				//...qui n'est pas dans la closed-list
				bool found = false;
				for (Node* knot : Labyrinth::closedList) {
					if (knot->equal(V)) {
						found = true; //so do nothing, it is in the closed list, which we won't touch anymore
					}
				}
				if (!found) {
					/* closed-list ne contient pas V */
					Labyrinth::updateNeighboursOfNHGFP(V, N);

					//Si V est dans l'open-list: recalculate their G cost from the parent of the current parent "N",
					//To see if it's interesting to start FROM that parent TO the current V 
					//And check if the path from that parent to the V is shorter  
					found = false;
					for (Node* openListknot : Labyrinth::openList) {
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
						Labyrinth::openList.push_back(V);
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



Node*& Labyrinth::getExitNode()
{
	return Labyrinth::exitNode;
}

Node*& Labyrinth::getEntryNode()
{
	return Labyrinth::entryNode;
}

void Labyrinth::updateNeighboursOfNHGFP(Node* V, Node* N)
{
	//a- Calculer H(V): distance of the knot to end : done inside getNeighBoursKnots

	//b- Calculer G(V) = G(N) + coût(N->V). N: current knot
	V->setG(N->getG() + Labyrinth::getDistance(N->getPosition(), V->getPosition()));

	//c- F(V) = G(V) + H(V)
	V->setF(V->getG() + V->getH());

	//d- et P(V) = N (noeud qui précède V = N)
	V->setP(N);
}

void Labyrinth::findNeighboursOfN(std::vector<Node*>& neighboursOfN, Node* N)
{
	neighboursOfN = N->getNeighBoursKnots(Labyrinth::walls);
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

std::vector <Node*> Labyrinth::getPath(Node*& N)
{
	std::vector <Node*> pathNodes;
	//Afficher le chemin en partant de N et en utilisant la propriété P : nœud précédent
	pathNodes.push_back(N);
	while (N->getPredecessor() != nullptr) {
		Node* predecessor = N->getPredecessor();
		pathNodes.push_back(predecessor);

		/*SDL_RenderDrawLine(renderer, N->getPosition().x, N->getPosition().y,
			predecessor->getPosition().x,
			predecessor->getPosition().y);*/
		N = predecessor;
	}
	return pathNodes;
}

bool Labyrinth::isAtExitPoint(Node* N)
{	
	return abs(N->getPosition().x - exitNode->getPosition().x) < 1.0 && abs(N->getPosition().y - exitNode->getPosition().y);
	//return N->getPosition().x == Labyrinth::exitNode->getPosition().x && N->getPosition().y == Labyrinth::exitNode->getPosition().y;
		//|| N->getPosition().x == Labyrinth::randomExitPointNode->getPosition().x && N->getPosition().y == Labyrinth::randomExitPointNode->getPosition().y;
}

bool Labyrinth::isAtExitPoint(Node* N, Node* end)
{
	return N->getPosition().x == end->getPosition().x && N->getPosition().y == end->getPosition().y;
}

bool Labyrinth::nodeInsideWall(Node* knot, std::vector<Wall*>& walls)
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

bool Labyrinth::exitNodeInsideWall()
{
	for (int i = 0; i < Labyrinth::walls.size(); i++) {
		if (Labyrinth::exitNode->getPosition().x > Labyrinth::walls[i]->getWall().x - 6.0
			&& Labyrinth::exitNode->getPosition().x < (double)Labyrinth::walls[i]->getWall().x + Labyrinth::walls[i]->getWall().w + 6.0
			&& Labyrinth::exitNode->getPosition().y > Labyrinth::walls[i]->getWall().y - 6.0
			&& Labyrinth::exitNode->getPosition().y < (double)Labyrinth::walls[i]->getWall().y + Labyrinth::walls[i]->getWall().h + 6.0) {
			return true;
		}
	}
	return false;
}

bool Labyrinth::randomExitNodeInsideWall()
{
	for (int i = 0; i < Labyrinth::walls.size(); i++) {
		if (Labyrinth::randomExitPointNode->getPosition().x > Labyrinth::walls[i]->getWall().x - 6.0
			&& Labyrinth::randomExitPointNode->getPosition().x < (double)Labyrinth::walls[i]->getWall().x + Labyrinth::walls[i]->getWall().w + 6.0
			&& Labyrinth::randomExitPointNode->getPosition().y > Labyrinth::walls[i]->getWall().y - 6.0
			&& Labyrinth::randomExitPointNode->getPosition().y < (double)Labyrinth::walls[i]->getWall().y + Labyrinth::walls[i]->getWall().h + 6.0) {
			return true;
		}
	}
	return false;
}

