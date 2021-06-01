#pragma once

void placeSliders(Slider* sliders[7]);

void drawAndUpdateSliders(Slider* sliders[7], SDL_Renderer* renderer, SDL_Event& event);

void updatePointsForLinearFunction(Point& pointA, SDL_Event& event, Point& pointB);

void displayAxis(SDL_Rect& rect, Slider* sliders[7], SDL_Renderer* renderer);

void updatePointsForSquareFunction(Point  points[1000], SDL_Event& event);
