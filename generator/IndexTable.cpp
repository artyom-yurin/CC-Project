#include "IndexTable.hpp"

std::pair<int, Type> IndexTable::getIndex(const std::string &name) {
    auto index = indexes.find(name);
    if (index == indexes.end()) {
        if (parent == nullptr)
            return -1;
        return parent->getIndex(name);
    }
    return index->second;
}

int IndexTable::addIndex(const std::string &name, Type type) {
    indexes.
            insert(std::make_pair<std::string, std::pair<int, Type>>(
            name,
            std::make_pair<int, Type>(countCurrentVariabels, type))
    );
    countCurrentVariabels++;
    return countCurrentVariabels - 1;
}