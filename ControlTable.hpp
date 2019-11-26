#ifndef TUTORIAL_CONTROLTABLE_HPP
#define TUTORIAL_CONTROLTABLE_HPP

#include "SymbolTable.hpp"
#include "TypeTable.hpp"
#include <memory>

class ControlTable {
public:
  ControlTable();
  ~ControlTable() = default;

  bool addSimpleType(const std::string &name,
                     const std::string originalTypeName);
  bool addArrayType(const std::string &name, const std::string &originalName,
                    CNode *expression);
  bool addRecordType(const std::string &name, CNode *fields);

  bool addVariable(const std::string &name, const std::string &type);
  bool addFunction(const std::string &name, const std::string &return_type);

  bool isVariable(const std::string &name);
  bool isFunction(const std::string &name);

  bool isType(const std::string &name);

  std::shared_ptr<ControlTable> getSubControlTable() const;
private:
  ControlTable(std::shared_ptr<ControlTable> parent);
  VariableNode getVariable(const std::string &name);
  FunctionNode getFunction(const std::string &name);

  TypeNode getType(const std::string &name);

  std::weak_ptr<ControlTable> parent_;
  std::unique_ptr<TypeTable> type_table_;
  std::unique_ptr<SymbolTable> symbol_table_;
};

#endif // TUTORIAL_CONTROLTABLE_HPP
