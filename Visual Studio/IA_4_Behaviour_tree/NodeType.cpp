#include "NodeType.h"

NodeType::NodeType(Type type)
{
	this->type = type;
}

Type NodeType::getType()
{
	return this->type;
}
