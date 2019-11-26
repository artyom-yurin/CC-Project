#ifndef CC_PROJECT_TYPENODE_HPP
#define CC_PROJECT_TYPENODE_HPP

#include "Node.hpp"
#include "SymbolNode.hpp"
#include <string>

class SymbolNode;

enum Types {
  NoType,
  Auto,
  Simple,
  Array,
  Record
};

class TypeNode {
public:
  virtual ~TypeNode() = default;
  Types getType() const;
  virtual std::string toStr() const = 0;
protected:
  Types type;
};

class CNoType : public TypeNode{
public:
  CNoType();
  ~CNoType() = default;
  std::string toStr() const override ;
};

class AutoType : public TypeNode{
public:
  AutoType();
  ~AutoType() = default;

  std::string toStr() const override ;
};

class SimpleType : public TypeNode{
public:
  SimpleType(const std::string& name);
  ~SimpleType() = default;
  std::string name;

  std::string toStr() const override ;
};

class ArrayType : public TypeNode{
public:
  ArrayType(CNode* expression, std::shared_ptr<TypeNode> type);
  ~ArrayType() = default;
  CNode *expression;
  std::shared_ptr<TypeNode> arrayType;

  std::string toStr() const override ;
};

class RecordType : public TypeNode{
public:
  RecordType(const std::vector<std::shared_ptr<VariableNode>> &fields);
  ~RecordType() = default;
  std::vector<std::shared_ptr<VariableNode>> fields;

  std::string toStr() const override ;
};

#endif // CC_PROJECT_TYPENODE_HPP
