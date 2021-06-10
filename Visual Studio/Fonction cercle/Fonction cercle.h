#pragma once

void UpdateCircleRadius(Circle& circle, Slider* sliders[2]);

void DrawCircle(Circle& circle, SDL_Renderer* renderer, Slider* sliders[2]);

void drawPointToTest(Point& unPoint, SDL_Renderer* renderer);

void IsPointInsideCircle(Circle& circle, Point& unPoint, SDL_Renderer* renderer);

void UpdateAllObjects(Circle& circle, SDL_Event& event, Point& unPoint, Slider* sliders[2], SDL_Renderer* renderer);
