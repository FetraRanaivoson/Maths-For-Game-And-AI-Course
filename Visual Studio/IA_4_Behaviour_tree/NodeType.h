#pragma once

static enum class Type
{
	isSequencer, isSelector, isAction
};

class NodeType
{
private:
	Type type;

public:
	NodeType(Type type);
	Type getType();
};

