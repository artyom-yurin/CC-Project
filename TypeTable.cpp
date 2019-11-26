#include "TypeTable.hpp"

TypeTable::TypeTable() { types = {}; }

bool TypeTable::isType(const std::string &cname) {
  return types.find(cname) != types.end();
}

bool TypeTable::addSimpleType(const std::string &cname,
                              std::shared_ptr<TypeNode> type) {
  if (isType(cname))
    return false;

  types.insert(std::pair<std::string, std::shared_ptr<TypeNode>>(cname, type));

  return true;
}

bool TypeTable::addArrayType(const std::string &cname, CNode *expression,
                             std::shared_ptr<TypeNode> type) {
  if (isType(cname))
    return false;

  auto arrayType = std::make_shared<ArrayType>(expression, type);

  types.insert(
      std::pair<std::string, std::shared_ptr<TypeNode>>(cname, arrayType));

  return true;
}

bool TypeTable::addRecordType(
    const std::string &cname,
    const std::vector<std::shared_ptr<VariableNode>> &fields) {
  if (isType(cname))
    return false;

  auto recordType = std::make_shared<RecordType>(fields);

  types.insert(
      std::pair<std::string, std::shared_ptr<TypeNode>>(cname, recordType));

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
