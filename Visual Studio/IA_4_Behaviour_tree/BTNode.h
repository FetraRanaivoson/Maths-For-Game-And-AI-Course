#pragma once
#include <iostream>
#include "NodeType.h"
#include <vector>
#include "IActor.h"
#include "SDL.h"

class IActor;

enum class NodeState
{
	success, failed, progress, undefined
};

enum Action {
	wander, checkResourcesInRange, goToResource, notApplicable
};

class BTNode
{
private:
	NodeType nodeType;
	std::vector<BTNode*> btNodeChildren;

	NodeState nodeState;

	IActor* actor;
	int idAction;

	std::string name;
	
public:
	BTNode(std::string name, NodeType nodeType);
	void addChild(BTNode* node);
	std::vector<BTNode*> getChildren();
	NodeState evaluateNode(SDL_Renderer* renderer);
	NodeState getState();

	void setActor(IActor* actor, Action action); //like addListener (droid)

	std::string getName();

};

