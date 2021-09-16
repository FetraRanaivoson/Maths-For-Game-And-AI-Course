#pragma once
#include <vector>
#include "BTNode.h"
class BehaviourTree
{
private:
	std::vector<BTNode*> nodes;

public:
	BehaviourTree(std::vector<BTNode*> nodes);

	void setChild(BTNode* parentNode, BTNode* childNode);

};

