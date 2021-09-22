#pragma once
#include "BTNode.h"
#include "SDL.h"
enum class NodeState;

class IActor
{
public:
	virtual NodeState action(int idAction, SDL_Renderer* renderer) = 0;
};

