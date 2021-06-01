#pragma once

void UpdateSliders(Slider* sliders[7], SDL_Renderer* renderer, SDL_Event& event);

void displaySinusFunction(double xMin, double xMax, Slider* sliders[7], double yMin, double yMax, SDL_Renderer* renderer);
