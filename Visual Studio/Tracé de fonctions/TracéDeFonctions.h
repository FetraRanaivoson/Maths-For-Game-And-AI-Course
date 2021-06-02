#pragma once


void displayAxis(SDL_Rect& rect, Slider* sliders[], SDL_Renderer* renderer);


void updatePointsForSquareFunction(Point  points[], SDL_Event& event);


void updatePointsForLinearFunction(Point& pointA, SDL_Event& event, Point& pointB);


void drawAndUpdateSliders(Slider* sliders[], SDL_Renderer* renderer, SDL_Event& event);
