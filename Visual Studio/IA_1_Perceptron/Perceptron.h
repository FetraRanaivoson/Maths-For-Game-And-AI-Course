#pragma once

#include "../lib_Point/Point.h"
#include "../lib_Slider/Slider.h"
#include <time.h>
#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>

constexpr auto INITIAL_CANDIDATES_COUNT = 4;
enum { CAT = -1, DOG = 1 };
enum { CATSIZE = 2, DOGSIZE = 4 };

class Perceptron {
private:
	std::vector <double> weights; //Coefficients or weights for input
	double teta; //bias
	int output = 0;
	std::vector <Point> candidates;
	std::vector <Point> candidatesDrawable;
	std::vector <double> expectedType;

	Point originPlan = { 0,0 };
	double xMin, xMax, yMin, yMax, xMinS, xMaxS, yMinS, yMaxS;

	void initializeWeights(int nbInputs);
	void initializeTeta();
	void initializeCanvas(int windowWidth, int windowHeight);

	//Utilities
	Point pointFromPlanToScreen(Point p, double xMin, double xMax, double yMin, double yMax, double xMinS, double xMaxS, double yMinS, double yMaxS);
	Point pointFromScreenToPlan(Point p, double xMin, double xMax, double yMin, double yMax, double xMinS, double xMaxS, double yMinS, double yMaxS);
	void dessinerLesRepères(SDL_Renderer* renderer, const Point& originPlan, double xMin, double xMax, double yMin, double yMax, double& xMinS, double& xMaxS, double& yMinS, double& yMaxS);


	void addRandomCandidates(std::vector <Point> candidates);
	static double getRandomNumber(double xMin, double xMax);

	bool perceptronUpToDate = true;

	//If we trace a line on canvas, at any given xMin we have a yMinPoint that intersects that line.
	double yMinPoint = 0; 
	//If we trace a line on canvas, at any given xMax we have a yMaxPoint that intersects that line
	double yMaxPoint = 0;
	//We can then just trace a line from (x0, yMinPoint) to (x1, yMaxPoint)


public:
	Perceptron(int nbInputs, int windowWidth, int windowHeight);
	void drawRepères(SDL_Renderer* renderer);
	void drawCandidates(SDL_Renderer* renderer);
	void addCat(int posX, int posY);
	void addDog(int posX, int posY);
	void update(SDL_Renderer* renderer);

	//Summation function: return linear combination of inputs X and weights W and teta
	double computeSum(std::vector <double>	X); //X1, X2

	//Return 1 or -1 based on sign of x
	double getSign(double x);

	//Return sign of the sum of X (activateFunction(computeSum(X)) == initialization
	double getOutput(std::vector <double> X);

	//Make a perceptron to learn given a candidate and the excpected result
	bool learn(std::vector<double>X, double expected);

	void displayLine();


	//Getter/setter
	double& getTeta();
	std::vector<double>& getWeights();
	void incrementTeta(double value);
	void incrementWeight(int index, double value);



};

