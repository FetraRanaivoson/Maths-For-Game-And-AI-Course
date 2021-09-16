#pragma once
#include "BTNode.h"

enum NodeState;

class IActor
{
public:
	virtual NodeState action(int idAction) = 0;
};

