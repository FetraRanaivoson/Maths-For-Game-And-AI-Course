#include "BTNode.h"


BTNode::BTNode(NodeType nodeType, NodeState nodeState)
	: nodeType(nodeType), nodeState(nodeState)
{}

void BTNode::evaluateNode()
{
	if (this->nodeType.getType() == Type::isSequencer) {
		for (BTNode* btNodeChild : this->btNodeChildren) {
			if (btNodeChild->isSuccess()) {
				this->nodeState = NodeState::success;
			}
			else if (btNodeChild->isFailed()) {
				this->nodeState = NodeState::failed;
				break;
			}
			else if (btNodeChild->isProgressing()) {
				this->nodeState = NodeState::progress;
				break;
			}
		}
	}

	else if (this->nodeType.getType() == Type::isSelector) {
		for (BTNode* btNodeChild : this->btNodeChildren) {
			if (btNodeChild->isSuccess()) {
				this->nodeState = NodeState::success;
				break;
			}
			else if (btNodeChild->isFailed()) {
				this->nodeState = NodeState::failed;
			}
			else if (btNodeChild->isProgressing()) {
				this->nodeState = NodeState::progress;
				break;
			}
		}
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
