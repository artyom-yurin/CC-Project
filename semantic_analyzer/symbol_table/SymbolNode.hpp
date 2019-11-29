#ifndef CC_PROJECT_SYMBOLNODE_HPP
#define CC_PROJECT_SYMBOLNODE_HPP

#include "common/Node.hpp"
#include <string>

class TypeNode;

class VariableNode{
public:
  VariableNode(const std::string &variableName,
               std::shared_ptr<TypeNode> variableType, CNode *Expression);
  ~VariableNode() = default;

  std::string toString();

  std::shared_ptr<TypeNode> variable_type_;
  std::string variable_name_;
  CNode *default_value_;
};

class FunctionNode{
public:
  FunctionNode(const std::string &functionName,
               std::shared_ptr<TypeNode> returnType,
               const std::vector<std::shared_ptr<VariableNode>> &parameters);
  ~FunctionNode() = default;

  std::string toString();

  std::shared_ptr<TypeNode> return_type_;
  std::string function_name_;
  std::vector<std::shared_ptr<VariableNode>> parameters_;
};

#endif // CC_PROJECT_SYMBOLNODE_HPP
