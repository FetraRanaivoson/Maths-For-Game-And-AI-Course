#include "BehaviourTree.h"

BehaviourTree::BehaviourTree(std::vector<BTNode*> nodes)
	:nodes(nodes)
{}

void BehaviourTree::setChild(BTNode* parentNode, BTNode* childNode)
{
	parentNode->addChild(childNode);
}
