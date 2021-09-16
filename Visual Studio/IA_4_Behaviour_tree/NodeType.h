#pragma once

static enum class Type
{
	sequencer, selector, action
};

class NodeType
{
private:
	Type type;

public:
	NodeType(Type type);
	Type getType();
};

