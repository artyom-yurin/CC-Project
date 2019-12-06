#ifndef ICOMPILER_INDEXTABLE_HPP
#define ICOMPILER_INDEXTABLE_HPP


#include <unordered_map>
#include <string>
#include <memory>

enum Type{
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
    std::unordered_map<std::string, std::pair<int,Type>> indexes;

    int addIndex(const std::string& name, Type type);

    std::pair<int,Type> getIndex(const std::string& name);
};


#endif //ICOMPILER_INDEXTABLE_HPP
