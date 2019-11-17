#ifndef CC_PROJECT_TYPETABLE_HPP
#define CC_PROJECT_TYPETABLE_HPP

#include "TypeNode.hpp"
#include <unordered_map>

class TypeTable {
public:
  TypeTable();

  ~TypeTable() = default;

  bool addSimpleType(const std::string &cname, const std::string &originalName);

  bool addArrayType(const std::string &cname, CNode *expression, const std::string& type);

  bool addRecordType(const std::string &cname, CNode *record);

  std::shared_ptr<const TypeNode> getType(const std::string &cname);

  bool isType(const std::string &cname);

  bool removeType(const std::string &cname);

private:
  std::unordered_map<std::string, std::shared_ptr<TypeNode>> types;
};

#endif // CC_PROJECT_TYPETABLE_HPP
