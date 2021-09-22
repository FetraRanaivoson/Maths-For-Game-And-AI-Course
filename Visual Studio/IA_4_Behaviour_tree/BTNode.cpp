#include "BTNode.h"



BTNode::BTNode(std::string name, NodeType nodeType)
	: name(name), nodeType(nodeType)
{}

void BTNode::addChild(BTNode* node)
{
	this->btNodeChildren.push_back(node);
}

std::vector<BTNode*> BTNode::getChildren()
{
	return this->btNodeChildren;
}

NodeState BTNode::evaluateNode(SDL_Renderer* renderer)
{
	if (this->nodeType.getType() == Type::sequencer) {
		NodeState state = NodeState::success; //Compiler happy
		for (BTNode* btNodeChild : this->btNodeChildren) {
			state = btNodeChild->evaluateNode(renderer);
			if (state != NodeState::success) { //When child failed or running
				//std::cout << "A child failed or running" << std::endl;
				break;
			}
		}
		this->nodeState = state;
		return state;
	}

	else if (this->nodeType.getType() == Type::selector) {
		NodeState state = NodeState::failed; //Compiler happy
		for (BTNode* btNodeChild : this->btNodeChildren) {
			state = btNodeChild->evaluateNode(renderer);
			if (state != NodeState::failed) { //When child succeed or running
				//std::cout << "A child succeed or running" << std::endl;
				break;
			}
		}
		this->nodeState = state;
		return state;
	}
	else {
		return this->actor->action(this->idAction, renderer);
	}
}

NodeState BTNode::getState()
{
	return this->nodeState;
}


void BTNode::setActor(IActor* actor, Action action)
{
	this->actor = actor;
	this->idAction = action;
}

std::string BTNode::getName()
{
	return this->name;
}
