#include "BTNode.h"


BTNode::BTNode(NodeType nodeType, NodeState nodeState)
	: nodeType(nodeType), nodeState(nodeState)
{}

void BTNode::addChild(BTNode* node)
{
	this->btNodeChildren.push_back(node);
}

std::vector<BTNode*> BTNode::getChildren()
{
	return this->btNodeChildren;
}

NodeState BTNode::evaluateNode()
{
	if (this->nodeType.getType() == Type::sequencer) {
		NodeState state = NodeState::success; //Compiler happy
		for (BTNode* btNodeChild : this->btNodeChildren) {
			state = btNodeChild->evaluateNode();
			if (state != NodeState::success) { //When child failed or running
				break;
			}
		}
		return state;
	}

	else if (this->nodeType.getType() == Type::selector) {
		NodeState state = NodeState::failed; //Compiler happy
		for (BTNode* btNodeChild : this->btNodeChildren) {
			state = btNodeChild->evaluateNode();
			if (state != NodeState::failed) { //When child succeed or running
				break;
			}
		}
		return state;
	}
	else {
		return this->actor->action(this->idAction);
	}
}

bool BTNode::isSuccess()
{
	return (nodeState == NodeState::success) ? true : false;
}

bool BTNode::isFailed()
{
	return (nodeState == NodeState::failed) ? true : false;
}

bool BTNode::isProgressing()
{
	return (nodeState == NodeState::progress) ? true : false;
}
