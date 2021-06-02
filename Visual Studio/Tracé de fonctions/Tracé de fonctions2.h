#pragma once

void UpdateSliders(Slider* sliders[7], SDL_Renderer* renderer, SDL_Event& event);

void displaySinusFunction(double xMin, double xMax, Slider* sliders[7], double yMin, double yMax, SDL_Renderer* renderer);

void DessinerLesRepères(SDL_Renderer* renderer, const Point& originPlan, double xMin, double xMax, double yMin, double yMax);
