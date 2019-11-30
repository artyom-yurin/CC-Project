#ifndef TUTORIAL_CONTROLTABLE_HPP
#define TUTORIAL_CONTROLTABLE_HPP

#include "semantic_analyzer/symbol_table/SymbolTable.hpp"
#include "semantic_analyzer/type_table/TypeTable.hpp"
#include <memory>

class ControlTable : public std::enable_shared_from_this<ControlTable>{
public:
  ControlTable();
  ControlTable(ControlTable* parent);
  ~ControlTable() = default;

  bool addSimpleType(const std::string &name,
                     const std::string originalTypeName);
  bool addArrayType(const std::string &name, const std::string &originalName,
                    CNode *expression);
  bool addRecordType(const std::string &name, CNode *fields);

  bool addVariable(const std::string &name, const std::string &type,
                   CNode *expression);
  bool addFunction(const std::string &name, const std::string &return_type,
                   CNode *parameters);

  bool isVariable(const std::string &name);
  bool isFunction(const std::string &name);

  bool isType(const std::string &name);

  std::shared_ptr<ControlTable>
  getSubScopeTable(const std::string &scope_name) const;

  // add subScope
  bool addSubScope(const std::string &scope_name);

  // for inner structures
  // index will be number of scope
  // in superscope start from 1
  // return key
  std::string addSubScope();

  std::shared_ptr<ControlTable> getParent() const;

private:
  std::shared_ptr<FunctionNode> getFunction(const std::string &name);
  std::shared_ptr<VariableNode> getVariable(const std::string &name);
  std::shared_ptr<TypeNode> getType(const std::string &name);

  std::weak_ptr<ControlTable> parent_;
  std::unique_ptr<TypeTable> type_table_;
  std::unique_ptr<SymbolTable> symbol_table_;
  std::unordered_map<std::string, std::shared_ptr<ControlTable>> sub_scopes_;
};

#endif // TUTORIAL_CONTROLTABLE_HPP
