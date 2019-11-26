#include "SymbolNode.hpp"
#include "TypeNode.hpp"

VariableNode::VariableNode(const std::string &variableName,
                           std::shared_ptr<TypeNode> variableType,
                           CNode *Expression) {
  variable_name_ = variableName;
  variable_type_ = variableType;
  default_value_ = Expression; // TODO Processing expression
}

std::string VariableNode::toString() {
  return variable_type_->toStr() + " " + variable_name_;
}

FunctionNode::FunctionNode(
    const std::string &functionName, std::shared_ptr<TypeNode> returnType,
    const std::vector<std::shared_ptr<VariableNode>> &parameters) {
  return_type_ = returnType;
  function_name_ = functionName;
  parameters_ = parameters;
}

std::string FunctionNode::toString() {
  return return_type_->toStr() + " " + function_name_;
}
