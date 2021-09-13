#include "Node.h"

#include "Labyrinth.h"

Node::Node(Point position, Point exit)
{
	this->position = position;
	this->H = this->calculateH(exit);

	if (this->predecessor != nullptr)
		this->G = this->predecessor->getG();
	else
		this->G = 0;

	this->F = this->G + this->H;
}

void Node::draw(SDL_Renderer* renderer, Color color, int size)
{
	position.draw(renderer, color, size);
	position.drawCircle(renderer, 6, color, true);
}

double Node::calculateH(Point exit)
{
	double H = sqrt((this->position.x - exit.x) * (this->position.x - exit.x)
		+ (this->position.y - exit.y) * (this->position.y - exit.y));
	return H;
}

Point Node::getPosition()
{
	return position;
}

Node* Node::getPredecessor() const
{
	return this->predecessor;
}



double Node::getG()
{
	return this->G;
}

double Node::getF()
{
	return this->F;
}

double Node::getH()
{
	return this->H;
}

void Node::setG(double newG)
{
	this->G = newG;
}

void Node::setF(double newF)
{
	this->F = newF;
}

void Node::setP(Node* p)
{
	this->predecessor = p;
}

bool Node::equal(Node* knot)
{
	return this->getPosition().x == knot->getPosition().x &&
		this->getPosition().y == knot->getPosition().y;
}

std::vector<Node*> Node::getNeighBoursKnots(std::vector<Wall*> walls)
{
	std::vector<Node*> neighboursKnots;

	//Normally, there should be 8 Neighbours per knot, here: begining from top left and going clockwise
	std::vector<Point*> neighbourPositions{
		new Point(this->position.x - NEIGHBOUR_TOLERANCE_RANGE, this->position.y - NEIGHBOUR_TOLERANCE_RANGE),
		new Point(this->position.x, this->position.y - NEIGHBOUR_TOLERANCE_RANGE),
		new Point(this->position.x + NEIGHBOUR_TOLERANCE_RANGE, this->position.y - NEIGHBOUR_TOLERANCE_RANGE),
		new Point(this->position.x + NEIGHBOUR_TOLERANCE_RANGE, this->position.y),
		new Point(this->position.x + NEIGHBOUR_TOLERANCE_RANGE, this->position.y + NEIGHBOUR_TOLERANCE_RANGE),
		new Point(this->position.x, this->position.y + NEIGHBOUR_TOLERANCE_RANGE),
		new Point(this->position.x - NEIGHBOUR_TOLERANCE_RANGE, this->position.y + NEIGHBOUR_TOLERANCE_RANGE),
		new Point(this->position.x - NEIGHBOUR_TOLERANCE_RANGE, this->position.y)
	};

	for (int neighbour = 0; neighbour < 8; neighbour++) {
		//Si le noeud à checker n'est pas un mur alors c'est un voisin du noeud
		if (!this->IsInsideWall(neighbourPositions, neighbour, walls)) {
			neighboursKnots.push_back(new Node(Point(neighbourPositions[neighbour]->x, neighbourPositions[neighbour]->y), Labyrinth::getExitKnot()->getPosition()));
		}
	}
	return neighboursKnots;
}

bool Node::IsInsideWall(std::vector<Point*>& neighbourPositions, int neighbour, std::vector<Wall*>& walls)
{
	for (int i = 0; i < walls.size(); i++) {
		if (neighbourPositions[neighbour]->x > walls[i]->getWall().x - WALL_TOLERANCE &&
			neighbourPositions[neighbour]->x < walls[i]->getWall().x + (double)walls[i]->getWall().w + WALL_TOLERANCE &&
			neighbourPositions[neighbour]->y > walls[i]->getWall().y - WALL_TOLERANCE &&
			neighbourPositions[neighbour]->y < walls[i]->getWall().y + (double)walls[i]->getWall().h + WALL_TOLERANCE) {
			return true;
		}
	}
	return false;
}


