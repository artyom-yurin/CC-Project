#ifndef ICOMPILER_INDEXTABLE_HPP
#define ICOMPILER_INDEXTABLE_HPP

#include <memory>
#include <string>
#include <unordered_map>

enum GeneratorType {
  Error,
  Void,
  Integer,
  Boolean,
  Real,
  Arr_Integer,
  Arr_Boolean,
  Arr_Real
};

class IndexTable {
public:
  std::shared_ptr<IndexTable> parent;
  int countCurrentVariabels;
  std::unordered_map<std::string, std::pair<int, GeneratorType>> indexes;

  int addIndex(const std::string &name, GeneratorType type);

  std::pair<int, GeneratorType> getIndex(const std::string &name);
};

#endif // ICOMPILER_INDEXTABLE_HPP
