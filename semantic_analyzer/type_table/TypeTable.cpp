#include "semantic_analyzer/type_table/TypeTable.hpp"

bool TypeTable::isType(const std::string &cname) {
  return types.find(cname) != types.end();
}

bool TypeTable::addType(const std::string &cname,
                              std::shared_ptr<TypeNode> type) {
  if (isType(cname))
    return false;

  types.insert(std::pair<std::string, std::shared_ptr<TypeNode>>(cname, type));

  return true;
}

std::shared_ptr<TypeNode> TypeTable::getType(const std::string &cname) {
  auto type = types.find(cname);
  if (type == types.end()) {
    return nullptr;
  } else {
    return (*type).second;
  }
}

bool TypeTable::removeType(const std::string &cname) {
  if (!isType(cname))
    return false;

  types.erase(cname);

  return true;
}
