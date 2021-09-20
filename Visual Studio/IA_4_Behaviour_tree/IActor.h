#pragma once
#include "BTNode.h"

enum class NodeState;

class IActor
{
public:
	virtual NodeState action(int idAction) = 0;
};

