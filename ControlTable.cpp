#include "ControlTable.hpp"
ControlTable::ControlTable() {
  parent_.reset();
  type_table_ = std::make_unique<TypeTable>();
  symbol_table_ = std::make_unique<SymbolTable>();
  type_table_->addSimpleType("integer",
                             std::make_shared<SimpleType>("integer"));
  type_table_->addSimpleType("real", std::make_shared<SimpleType>("real"));
  type_table_->addSimpleType("boolean",
                             std::make_shared<SimpleType>("boolean"));
}
ControlTable::ControlTable(std::shared_ptr<ControlTable> parent) {
  parent_ = parent;
  type_table_ = std::make_unique<TypeTable>();
  symbol_table_ = std::make_unique<SymbolTable>();
}
bool ControlTable::addSimpleType(const std::string &name,
                                 const std::string originalTypeName) {
  auto type = getType(originalTypeName);
  if (type == nullptr)
    return false;
  return type_table_->addSimpleType(name, type);
}
bool ControlTable::addArrayType(const std::string &name,
                                const std::string &item_type,
                                CNode *expression) {
  auto type = getType(item_type);
  if (type == nullptr)
    return false;
  return type_table_->addArrayType(name, expression, type);
}
bool ControlTable::addRecordType(const std::string &name, CNode *fields) {
  // TODO convert fields to list
  std::vector<std::shared_ptr<VariableNode>> fields_list = {};
  return type_table_->addRecordType(name, fields_list);
}
bool ControlTable::addVariable(const std::string &name, const std::string &type,
                               CNode *expression) {
  auto typeNode = getType(type);
  if (typeNode == nullptr)
    return false;
  return symbol_table_->addVariable(name, typeNode, expression);
}
bool ControlTable::addFunction(const std::string &name,
                               const std::string &return_type,
                               CNode *paramaters) {
  auto typeNode = getType(return_type);
  if (typeNode == nullptr)
    return false;
  // TODO convert parameters to list
  std::vector<std::shared_ptr<VariableNode>> paramaters_list = {};
  return symbol_table_->addFunction(name, typeNode, paramaters_list);
}

bool ControlTable::isVariable(const std::string &name) {
  if (symbol_table_->isVariable(name))
    return true;
  if (!parent_.expired())
    return parent_.lock()->isVariable(name);
  return false;
}

bool ControlTable::isFunction(const std::string &name) {
  if (symbol_table_->isFunction(name))
    return true;
  if (!parent_.expired())
    return parent_.lock()->isFunction(name);
  return false;
}

bool ControlTable::isType(const std::string &name) {
  if (type_table_->isType(name))
    return true;
  if (!parent_.expired())
    return parent_.lock()->isType(name);
  return false;
}

std::shared_ptr<ControlTable>
ControlTable::getSubScopeTable(const std::string &scope_name) const {
  auto scope = sub_scopes_.find(scope_name);
  if (scope == sub_scopes_.end()) {
    return nullptr;
  } else {
    return (*scope).second;
  }
}



bool ControlTable::addSubScope(const std::string &scope_name, std::shared_ptr<ControlTable> sub_scope) {
  auto scope = sub_scopes_.find(scope_name);
  if (scope != sub_scopes_.end())
    return false;
  sub_scopes_.insert(std::pair<std::string, std::shared_ptr<ControlTable>>(
      scope_name, sub_scope));
  return true;
}

std::string ControlTable::addSubScope(std::shared_ptr<ControlTable> sub_scope) {
  std::string key_scope = std::to_string(sub_scopes_.size() + 1);
  if (addSubScope(key_scope, sub_scope))
    return key_scope;
  return "";
}

std::shared_ptr<VariableNode>
ControlTable::getVariable(const std::string &name) {
  auto result = symbol_table_->getVariable(name);
  if (result != nullptr)
    return result;
  if (!parent_.expired())
    return parent_.lock()->getVariable(name);
  return nullptr;
}

std::shared_ptr<FunctionNode>
ControlTable::getFunction(const std::string &name) {
  auto result = symbol_table_->getFunction(name);
  if (result != nullptr)
    return result;
  if (!parent_.expired())
    return parent_.lock()->getFunction(name);
  return nullptr;
}
std::shared_ptr<TypeNode> ControlTable::getType(const std::string &name) {
  auto result = type_table_->getType(name);
  if (result != nullptr)
    return result;
  if (!parent_.expired())
    return parent_.lock()->getType(name);
  return nullptr;
}
