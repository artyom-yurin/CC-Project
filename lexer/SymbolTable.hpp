#pragma once

#include <string>
#include "Token.hpp"

#define MAX_HASH 997


class Node {
    public:
        std::string value;
        int class_name;
        Node* next; 

    public:
        Node();
        Node(std::string value, int class_name);
};


class SymbolTable {
    public:
        Node* nodes[MAX_HASH];

    public:
        SymbolTable();

        int hashSum(std::string node_value);

        bool insert(std::string value, int class_name);

        int find(std::string value);
};
