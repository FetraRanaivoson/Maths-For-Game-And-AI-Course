#pragma once
#include "NodeType.h"
#include <vector>

enum class NodeState
{
	success, failed, progress
};

class BTNode
{
private:
	NodeType nodeType;
	std::vector<BTNode*> btNodeChildren;

	NodeState nodeState;
	
public:
	BTNode(NodeType nodeType, NodeState nodeState);

	void evaluateNode();

	bool isSuccess();
	bool isFailed();
	bool isProgressing();
};

