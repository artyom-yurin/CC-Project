#include "semantic_analyzer/ControlTable.hpp"

ControlTable::ControlTable() {
  parent_.reset();
  type_table_ = std::make_unique<TypeTable>();
  symbol_table_ = std::make_unique<SymbolTable>();
  type_table_->addType("integer",
                             std::make_shared<SimpleType>("integer"));
  type_table_->addType("real", std::make_shared<SimpleType>("real"));
  type_table_->addType("boolean",
                             std::make_shared<SimpleType>("boolean"));
}

ControlTable::ControlTable(ControlTable *parent) {
  parent_ = parent->shared_from_this();
  type_table_ = std::make_unique<TypeTable>();
  symbol_table_ = std::make_unique<SymbolTable>();
}

std::shared_ptr<TypeNode> ControlTable::CNode2TypeNode(CNode *type) {
  if (type->name != "type")
    return nullptr;

  CNode *realType = type->children[0];
  if (realType->name == "array_type") {
    CNode *exp = realType->children[0];
    CNode *itemType = realType->children[1];
    auto typeNode = CNode2TypeNode(itemType);
    if (typeNode == nullptr)
      return nullptr;
    return std::make_shared<ArrayType>(exp, typeNode);
  } else if (realType->name == "record_type") {
    CNode *fields = realType->children[0];
    std::vector<std::shared_ptr<VariableNode>> fields_list;
    if (!CNode2FieldList(fields, fields_list))
      return nullptr;
    return std::make_shared<RecordType>(fields_list);
  } else {
    return getType(realType->name);
  }
}

bool ControlTable::CNode2FieldList(
    CNode *fields, std::vector<std::shared_ptr<VariableNode>> &fields_list) {
  if (fields->name != "variables_declaration") {
    return false;
  }
  int amount_children = fields->children.size();
  for (int i = 0; i < amount_children; i++) {
    CNode *child = fields->children[i];
    std::shared_ptr<TypeNode> type;
    if (child->name == "variable_declaration") {
      type = CNode2TypeNode(child->children[1]);
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
  return true;
}

bool ControlTable::addAutoVariable(const std::string &name, CNode *expression) {
  if (expression == nullptr)
    return false;
  auto typeNode = std::make_shared<AutoType>();
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
std::shared_ptr<ControlTable> ControlTable::getParent() const {
  if (parent_.expired())
    return nullptr;
  return parent_.lock();
}
bool ControlTable::addType(const std::string &name, CNode *type) {
  auto typeNode = CNode2TypeNode(type);
  if (type == nullptr)
    return false;
  return type_table_->addType(name, typeNode);
}

bool ControlTable::addVariable(const std::string &name, CNode *type,
                               CNode *expression) {
  auto typeNode = CNode2TypeNode(type);
  if (typeNode == nullptr)
    return false;
  return symbol_table_->addVariable(name, typeNode, expression);
}
