#include "SymbolTable.hpp"

bool SymbolTable::addVariable(const std::string &name,
                              std::shared_ptr<TypeNode> type,
                              CNode *expression) {
  if (isVariable(name))
    return false;

  //TODO: Allocate more for fields of record type or write function hasField
  auto variable = std::make_shared<VariableNode>(name, type, expression);

  variables_.insert(
      std::pair<std::string, std::shared_ptr<VariableNode>>(name, variable));

  return true;
}

bool SymbolTable::addFunction(
    const std::string &name, std::shared_ptr<TypeNode> return_type,
    const std::vector<std::shared_ptr<VariableNode>> &parameters) {
  if (isFunction(name))
    return false;

  auto function = std::make_shared<FunctionNode>(name, return_type, parameters);

  functions_.insert(
      std::pair<std::string, std::shared_ptr<FunctionNode>>(name, function));

  return true;
}

bool SymbolTable::isVariable(const std::string &name) {
  return variables_.find(name) != variables_.end();
}
bool SymbolTable::isFunction(const std::string &name) {
  return functions_.find(name) != functions_.end();
}

std::shared_ptr<VariableNode>
SymbolTable::getVariable(const std::string &name) {
  auto variable = variables_.find(name);
  if (variable == variables_.end()) {
    return nullptr;
  } else {
    return (*variable).second;
  }
}

std::shared_ptr<FunctionNode>
SymbolTable::getFunction(const std::string &name) {
  auto function = functions_.find(name);
  if (function == functions_.end()) {
    return nullptr;
  } else {
    return (*function).second;
  }
}

bool SymbolTable::removeFunction(const std::string &name) {
  if (!isFunction(name))
    return false;

  functions_.erase(name);

  return true;
}

bool SymbolTable::removeVariable(const std::string &name) {
  if (!isVariable(name))
    return false;

  variables_.erase(name);

  return true;
}
