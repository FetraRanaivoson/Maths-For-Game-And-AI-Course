#include "Perceptron.h"

double Perceptron::getRandomNumber(double xMin, double xMax)
{
	double number = 0;
	double min = xMin;
	double max = xMax;
	double r = (double)rand() / (double)RAND_MAX;
	number = min + r * (max - min);
	return number;

}
void Perceptron::initializeWeights(int nbInputs)
{
	for (int i = 0; i < nbInputs; i++) {
		this->weights.push_back(getRandomNumber(-1, 1));
	}
}
void Perceptron::initializeTeta()
{
	this->teta = getRandomNumber(-1, 1);
}
void Perceptron::initializeCanvas(int windowWidth, int windowHeight)
{
	xMin = -10, xMax = 10;
	yMin = -10, yMax = 10;
	xMinS = 0, xMaxS = windowWidth;
	yMinS = 0, yMaxS = windowHeight;
}
Point Perceptron::pointFromPlanToScreen(Point p, double xMin, double xMax, double yMin, double yMax, double xMinS, double xMaxS, double yMinS, double yMaxS) {
	double x = xMinS + (p.x - xMin) * (xMaxS - xMinS) / (xMax - xMin);
	double y = yMinS + (yMax - p.y) * (yMaxS - yMinS) / (yMax - yMin);
	Point result(x, y);
	return result;
}
Point Perceptron::pointFromScreenToPlan(Point p, double xMin, double xMax, double yMin, double yMax, double xMinS, double xMaxS, double yMinS, double yMaxS)
{
	double xP = ((p.x * (xMax - xMin) - xMinS) / (xMaxS - xMinS)) + xMin;
	double yP = -(((p.y * (yMax - yMin) - yMinS) / (yMaxS - yMinS)) - yMax);
	Point result(xP, yP);
	return result;
}
void Perceptron::dessinerLesRepères(SDL_Renderer* renderer, const Point& originPlan, double xMin, double xMax, double yMin, double yMax, double& xMinS, double& xMaxS, double& yMinS, double& yMaxS)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	Point originScreen = pointFromPlanToScreen(originPlan, xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS);
	originScreen.draw(renderer, Color(255, 255, 255, SDL_ALPHA_OPAQUE), 5);
	SDL_RenderDrawLine(renderer, xMinS, originScreen.y, xMaxS, originScreen.y);
	SDL_RenderDrawLine(renderer, originScreen.x, yMinS, originScreen.x, yMaxS);

}

//nbInputs: number of characteristics
Perceptron::Perceptron(int nbInputs, int windowWidth, int windowHeight)
{
	this->initializeWeights(nbInputs);
	this->initializeTeta();
	this->initializeCanvas(windowWidth, windowHeight);

	this->addRandomCandidates(candidates);
	for (int i = 0; i < candidates.size(); i++) {
		candidatesDrawable.push_back(pointFromPlanToScreen(candidates[i], xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS));
	}

	expectedType.push_back(DOG);
	expectedType.push_back(DOG);
	expectedType.push_back(CAT);
	expectedType.push_back(CAT);

}

void Perceptron::drawRepères(SDL_Renderer* renderer)
{
	this->dessinerLesRepères(renderer, originPlan, xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS);
}

void Perceptron::drawCandidates(SDL_Renderer* renderer)
{
	for (int i = 0; i < candidates.size(); i++) {
		if (expectedType[i] == CAT) {
			candidatesDrawable[i].draw(renderer, Color(255, 0, 0, SDL_ALPHA_OPAQUE), CATSIZE);
		}
		if (expectedType[i] == DOG) {
			candidatesDrawable[i].draw(renderer, Color(0, 255, 0, SDL_ALPHA_OPAQUE), DOGSIZE);
		}
	}
}



void Perceptron::addCat(int posX, int posY)
{
	Point position(posX, posY);
	candidates.push_back(pointFromScreenToPlan(position, xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS));
	candidatesDrawable.push_back(position);
	expectedType.push_back(CAT);
}

void Perceptron::addDog(int posX, int posY)
{
	Point position(posX, posY);
	candidates.push_back(pointFromScreenToPlan(position, xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS));
	candidatesDrawable.push_back(position);
	expectedType.push_back(DOG);
}

void Perceptron::update(SDL_Renderer* renderer) {
	do {
		//this->perceptronUpToDate = true;

		for (int j = 0; j < candidates.size(); j++) {
			std::vector<double> point; //pour store x, y d'un candidat
			point.push_back(candidates[j].x);
			point.push_back(candidates[j].y);
			output = this->learn(point, expectedType[j]);//somme des x,y 
			if (!output)
				//perceptronUpToDate = false;

			//drawing line 1st method
			//for (double x = xMin; x <= xMax; x += .01) {
			//	// W0 * x + W1 * y + TETA = 0 // y = -(W0 * x + TETA)/W1
			//	double y = -(this->getWeights()[0] * x + this->getTeta()) / this->getWeights()[1];
			//	Point pPlan(x, y);
			//	if (pPlan.y >= yMin && pPlan.y <= yMax) {
			//		Point pScreen = pointFromPlanToScreen(pPlan, xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS);
			//		pScreen.draw(renderer, Color(0, 50, 255, SDL_ALPHA_OPAQUE), 3);
			//	}
			//}
			
			//drawing line 2nd method
			this->yMinPoint = -(this->getWeights()[0] * xMin + this->getTeta()) /this->getWeights()[1];
			Point pPlanLow(xMin, yMinPoint);
			Point pScreenLow = pointFromPlanToScreen(pPlanLow, xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS);

			this->yMaxPoint = -(this->getWeights()[0] * xMax + this->getTeta()) / this->getWeights()[1];
			Point pPlanHigh(xMax, yMaxPoint);
			Point pScreenHigh = pointFromPlanToScreen(pPlanHigh, xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS);
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawLineF(renderer, pScreenLow.x, pScreenLow.y, pScreenHigh.x, pScreenHigh.y);
		}
		SDL_RenderPresent(renderer);
	} while (!output);
}

double Perceptron::computeSum(std::vector<double> X)
{
	double sum = 0;

	for (int i = 0; i < this->weights.size(); i++) {
		sum += X[i] * this->weights[i];
	}
	sum += teta;

	return sum;
}

double Perceptron::getSign(double x)
{
	return x >= 0 ? 1 : -1;
}

double Perceptron::getOutput(std::vector<double> X)
{
	return getSign(computeSum(X));
}

bool Perceptron::learn(std::vector<double> X, double expected)
{
	double output = getOutput(X);

	if (output != expected) { // something to learn?
		//retroaction
		for (int i = 0; i < this->weights.size(); i++) {
			this->weights[i] += (expected - output) * X[i];
		}
		this->teta += (expected - output);
	}
	return output == expected;
}

void Perceptron::displayLine()
{

}

double& Perceptron::getTeta()
{
	return this->teta;
}

void Perceptron::incrementTeta(double value)
{
	this->teta += value;
}

std::vector<double>& Perceptron::getWeights()
{
	return this->weights;
}

void Perceptron::incrementWeight(int index, double value)
{
	this->weights[index] += value;
}

void Perceptron::addRandomCandidates(std::vector<Point> candidates)
{
	for (int i = 0; i < INITIAL_CANDIDATES_COUNT; i++) {
		this->candidates.push_back(Point(getRandomNumber(xMin, xMax), getRandomNumber(xMin, xMax)));
	}
}
