#pragma once
#include "NodeType.h"
#include <vector>
#include "IActor.h"

class IActor;

enum NodeState
{
	success, failed, progress
};

class BTNode
{
private:
	NodeType nodeType;
	std::vector<BTNode*> btNodeChildren;

	NodeState nodeState;

	IActor* actor;
	int idAction;
	
public:
	BTNode(NodeType nodeType, NodeState nodeState);
	void addChild(BTNode* node);
	std::vector<BTNode*> getChildren();
	NodeState evaluateNode();

	bool isSuccess();
	bool isFailed();
	bool isProgressing();


};

