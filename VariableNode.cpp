#include "VariableNode.hpp"

VariableNode::VariableNode(const std::string &name,
                           std::shared_ptr<TypeNode> type, CNode *expression)
    : name(name), type(type), expression(expression) {}
