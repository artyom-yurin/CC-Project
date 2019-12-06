#ifndef TUTORIAL_CONTROLTABLE_HPP
#define TUTORIAL_CONTROLTABLE_HPP

#include "semantic_analyzer/symbol_table/SymbolTable.hpp"
#include "semantic_analyzer/type_table/TypeTable.hpp"
#include <memory>

class ControlTable : public std::enable_shared_from_this<ControlTable> {
public:
  ControlTable();
  ControlTable(ControlTable *parent);
  ~ControlTable() = default;

  bool addType(const std::string &name, CNode *type);

  bool addVariable(const std::string name, CNode *type, CNode *expression);
  bool addAutoVariable(const std::string name, CNode *expression);
  bool addCounter(const std::string &name);
  bool addFunction(const std::string &name, CNode *return_type,
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

  bool check_modifiable(CNode *node);

  bool checkFunctionCall(const std::string &functionName, CNode *arguments);

  bool processingExpression(CNode *&parent, int idChild);

private:
  std::shared_ptr<TypeNode> getType(const std::string &name);

  std::shared_ptr<TypeNode> type_modifiable(CNode *node);

  std::shared_ptr<TypeNode> whatType(CNode *node);

  std::shared_ptr<TypeNode> CompareTypes(std::shared_ptr<TypeNode> typeNode1,
                                         std::shared_ptr<TypeNode> typeNode2,
                                         std::string operation);

  std::shared_ptr<FunctionNode> getFunction(const std::string &name);
  std::shared_ptr<VariableNode> getVariable(const std::string &name);
  //  std::shared_ptr<TypeNode> getType(const std::string &name);

  bool check_modifiable(CNode *node, std::shared_ptr<TypeNode> &currentType);

  bool CNode2FieldList(CNode *fields,
                       std::vector<std::shared_ptr<VariableNode>> &fields_list);

  bool CNode2ArgList(CNode *args, std::vector<CNode *> &args_list);

      std::shared_ptr<TypeNode> CNode2TypeNode(CNode *type);

  bool addType(const std::string &name, std::shared_ptr<TypeNode> type);

  bool addVariable(const std::string &name, std::shared_ptr<TypeNode> type,
                   CNode *expression);

  CNode *calculate(CNode *node);

  std::weak_ptr<ControlTable> parent_;
  std::unique_ptr<TypeTable> type_table_;
  std::unique_ptr<SymbolTable> symbol_table_;
  std::unordered_map<std::string, std::shared_ptr<ControlTable>> sub_scopes_;
};

#endif // TUTORIAL_CONTROLTABLE_HPP
