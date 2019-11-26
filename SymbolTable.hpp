#ifndef CC_PROJECT_SYMBOLTABLE_HPP
#define CC_PROJECT_SYMBOLTABLE_HPP

#include "SymbolNode.hpp"
#include <unordered_map>

class SymbolTable {
public:
  SymbolTable();

  ~SymbolTable() = default;

  bool addVariable(const std::string &name, std::shared_ptr<TypeNode> type,
                   CNode *expression);

  bool addFunction(const std::string &name,
                   std::shared_ptr<TypeNode> return_type, CNode *parameters);

  std::shared_ptr<VariableNode> getVariable(const std::string &name);


  std::shared_ptr<FunctionNode> getFunction(const std::string &name);

  bool isVariable(const std::string &name);

  bool isFunction(const std::string &name);

  bool removeFunction(const std::string &name);

  bool removeVariable(const std::string &name);

private:
  std::unordered_map<std::string, std::shared_ptr<VariableNode>> variables_;

  std::unordered_map<std::string, std::shared_ptr<FunctionNode>> functions_;
};

#endif // CC_PROJECT_SYMBOLTABLE_HPP
