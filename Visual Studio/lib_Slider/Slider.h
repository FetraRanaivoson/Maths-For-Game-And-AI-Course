#pragma once

#include <SDL.h>

class Slider {
private:
	static const int cursorWidth = 9, cursorHeight = 17;
	static const int lineThickness = 3;
	static const int nearXOffset = 10;

	double min, max, value;
	int x, y, width;	//	(x, y) for min value. (x+width, y) for max value
	int pos;			//	slider position
	bool isDragMode;

	void setCursorPosition(int x);
	void computeValue();
	void computePos();
	bool isNear(int mouseX, int mouseY);
	void update(SDL_Event &evt);

public:
	Slider(int x, int y, int width, double min, double max, double value);
	void setValue(double value);
	void setMin(double min);
	void setMax(double max);
	double getValue();
	double draw(SDL_Renderer *renderer, SDL_Event &evt);
};