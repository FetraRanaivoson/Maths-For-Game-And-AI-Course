#pragma once

void createPointCloud(PointObject* points[100]);

void drawPointCloud(PointObject* points[100], SDL_Renderer* renderer, SDL_Event& event);

void activateExplosionInCase(Explosion& explosion, SDL_Renderer* renderer, const Color& explosionColor, SDL_Event& event);

void listenForExplosion(PointObject* points[100], const Explosion& explosion);
