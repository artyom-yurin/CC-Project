#ifndef CC_PROJECT_TYPETABLE_HPP
#define CC_PROJECT_TYPETABLE_HPP

#include "semantic_analyzer/type_table/TypeNode.hpp"
#include <unordered_map>

class TypeTable {
public:
  TypeTable() = default;

  ~TypeTable() = default;

  bool addType(const std::string &cname, std::shared_ptr<TypeNode> type);

  std::shared_ptr<TypeNode> getType(const std::string &cname);

  bool isType(const std::string &cname);

  bool removeType(const std::string &cname);

private:
  std::unordered_map<std::string, std::shared_ptr<TypeNode>> types;
};

#endif // CC_PROJECT_TYPETABLE_HPP
