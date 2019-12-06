#include "IndexTable.hpp"

std::pair<int, GeneratorType> IndexTable::getIndex(const std::string &name) {
  auto index = indexes.find(name);
  if (index == indexes.end()) {
    if (parent == nullptr)
      return std::pair<int, GeneratorType>(-1, GeneratorType::Error);
    return parent->getIndex(name);
  }
  return index->second;
}

int IndexTable::addIndex(const std::string &name, GeneratorType type) {
  indexes.insert(std::pair<std::string, std::pair<int, GeneratorType>>(
      name, std::pair<int, GeneratorType>(countCurrentVariabels, type)));
  countCurrentVariabels++;
  return countCurrentVariabels - 1;
}
