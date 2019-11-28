#include "ControlTable.hpp"

ControlTable::ControlTable() {
  parent_ = nullptr;
  type_table_ = std::make_unique<TypeTable>();
  symbol_table_ = std::make_unique<SymbolTable>();
  type_table_->addSimpleType("integer",
                             std::make_shared<SimpleType>("integer"));
  type_table_->addSimpleType("real", std::make_shared<SimpleType>("real"));
  type_table_->addSimpleType("boolean",
                             std::make_shared<SimpleType>("boolean"));
}

ControlTable::ControlTable(ControlTable *parent) {
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
  std::vector<std::shared_ptr<VariableNode>> fields_list = {};
  if (fields->name != "variables_declaration") {
    return false;
  }
  int amount_children = fields->children.size();
  for (int i = 0; i < amount_children; i++) {
    CNode *child = fields->children[i];
    std::shared_ptr<TypeNode> type;
    if (child->name == "variable_declaration") {
      type = getType(child->children[1]->name);
      if (type == nullptr)
        return false;
    } else if (child->name == "variable_declaration_auto") {
      type = std::make_shared<AutoType>();
    } else {
      return false;
    }

    auto new_field = std::make_shared<VariableNode>(child->children[0]->name,
                                                    type, child->children[1]);
    fields_list.push_back(new_field);
  }
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
                               CNode *parameters) {
  auto typeNode = getType(return_type);
  if (typeNode == nullptr)
    return false;
  std::vector<std::shared_ptr<VariableNode>> parameters_list = {};
  if (parameters->name != "parameters")
    return false;

  for (int i = 0; i < parameters->children.size(); i++) {
    if (parameters->children[i]->name != "parameter_declaration") {
      return false;
    }
    if (parameters->children[i]->children.size() != 2)
      return false;
    auto name = parameters->children[i]->children[0]->name; // x
    auto type = getType(parameters->children[i]->children[1]->name);
    if (type == nullptr) {
      return false;
    }
    auto parameter = std::make_shared<VariableNode>(name, type, nullptr);
    parameters_list.push_back(parameter);
  }
  return symbol_table_->addFunction(name, typeNode, parameters_list);
}

bool ControlTable::isVariable(const std::string &name) {
  if (symbol_table_->isVariable(name))
    return true;
  if (parent_ != nullptr)
    return parent_->isVariable(name);
  return false;
}

bool ControlTable::isFunction(const std::string &name) {
  if (symbol_table_->isFunction(name))
    return true;
  if (parent_ != nullptr)
    return parent_->isFunction(name);
  return false;
}

bool ControlTable::isType(const std::string &name) {
  if (type_table_->isType(name))
    return true;
  if (parent_ != nullptr)
    return parent_->isType(name);
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

bool ControlTable::addSubScope(const std::string &scope_name) {
  auto scope = sub_scopes_.find(scope_name);
  if (scope != sub_scopes_.end())
    return false;
  auto sub_scope = std::make_shared<ControlTable>(this);
  sub_scopes_.insert(std::pair<std::string, std::shared_ptr<ControlTable>>(
      scope_name, sub_scope));
  return true;
}

std::string ControlTable::addSubScope() {
  std::string key_scope = std::to_string(sub_scopes_.size() + 1);
  if (addSubScope(key_scope))
    return key_scope;
  return "";
}

std::shared_ptr<VariableNode>
ControlTable::getVariable(const std::string &name) {
  auto result = symbol_table_->getVariable(name);
  if (result != nullptr)
    return result;
  if (parent_ != nullptr)
    return parent_->getVariable(name);
  return nullptr;
}

std::shared_ptr<FunctionNode>
ControlTable::getFunction(const std::string &name) {
  auto result = symbol_table_->getFunction(name);
  if (result != nullptr)
    return result;
  if (parent_ != nullptr)
    return parent_->getFunction(name);
  return nullptr;
}

std::shared_ptr<TypeNode> ControlTable::getType(const std::string &name) {
  auto result = type_table_->getType(name);
  if (result != nullptr)
    return result;
  if (parent_ != nullptr)
    return parent_->getType(name);
  return nullptr;
}
