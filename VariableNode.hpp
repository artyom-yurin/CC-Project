#ifndef TUTORIAL_VARIABLENODE_HPP
#define TUTORIAL_VARIABLENODE_HPP

#include "TypeNode.hpp"
#include <string>

class TypeNode;

class VariableNode {
public:
  VariableNode(const std::string &name, std::shared_ptr<TypeNode> type,
               CNode *expression);

  std::string name;
  std::shared_ptr<TypeNode> type;
  CNode* expression;
};

#endif // TUTORIAL_VARIABLENODE_HPP
