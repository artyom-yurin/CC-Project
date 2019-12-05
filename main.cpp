#include "common/Node.hpp"
#include "lexer/Lexer.hpp"
#include "grammar/Parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

void print_node(CNode *node, int margin) {
    if (node == nullptr) return;
    for (int i = 0; i < margin; i++)
        std::cout << "   ";
    std::cout << "<" << node->name << ">\n";
    for (int j = 0; j < node->children.size(); j++)
        print_node(node->children[j], margin + 1);
}

void print_tree(CNode *root) {
    print_node(root, 0);
}

int main(int argc, char *argv[]) {
//  if (argc != 2) {
//    std::cerr << "Invalid number of args" << std::endl;
//    std::cerr << "Usage: " << argv[0] << " <path_to_source>" << std::endl;
//    return 1;
//  }
//
//  std::ifstream src_file(argv[1]);
//  if (!src_file.is_open())
//  {
//    std::cerr << "File don't open" << std::endl;
//    return 1;
//  }
//  std::stringstream buffer;
//
//  buffer << src_file.rdbuf();
//  Lexer *lexer = new Lexer(buffer.str());
//  CNode* root = nullptr;
//  yy::parser parser(lexer, (void**)&root);
//  parser.parse();
//  if (root == nullptr)
//    return 1;
//
//  print_tree(root);
    std::string str = argv[1];
    std::vector<std::string> array;
    std::istringstream is(argv[1]);
    std::string s;
    while (std::getline(is, s, '/')) {
        array.push_back(s);
    }
    std::string fpath = array[2];
    int dot = fpath.find(".txt");
    std::string path = fpath.substr(0, dot);
    std::cout << path;
//  return 0;
}

void translate_expression(CNode *node) {
    if (node->name == "expression") {
        translate_expression(node->children[0]);
        if (node->children.size() == 3) {
            translate_expression(node->children[2]);
            std::string op = node->children[1]->name;
            if (op == "and") {

            } else if (op == "or") {

            } else if (op == "xor") {

            }
        } //TODO or xor
        return;
    } else if (node->name == "relation") {
        translate_expression(node->children[0]);
        if (node->children.size() == 3) {
            translate_expression(node->children[2]);
            std::string op = node->children[1]->name;
            if (op == "<") {

            } else if (op == "<=") {

            } else if (op == ">") {

            } else if (op == ">=") {

            } else if (op == "=") {

            } else if (op == "/=") {

            }
        }
        return;
    } else if (node->name == "simple") {
        translate_expression(node->children[0]);
        if (node->children.size() == 3) {
            translate_expression(node->children[2]);
            std::string op = node->children[1]->name;
            if (op == "*") {

            } else if (op == "/") {

            } else if (op == "%") {

            }
        }
        return;
    } else if (node->name == "not_factor") {
        translate_expression(node->children[1]);
        // TODO not operation
        return;
    } else if (node->name == "unary_factor") {
        // TODO minus
        return;
    } else if (node->name == "factor") {
        translate_expression(node->children[0]);
        translate_expression(node->children[2]);
        std::string op = node->children[1]->name;
        if (op == "+") {

        } else if (op == "-") {

        }
        return;
    } else if (node->name == "summand") {
        return translate_expression(node->children[0]);

        return;
    } else if (node->name == "modifiable_primary_array" || node->name == "modifiable_primary_field" ||
               node->name == "modifiable_primary") {
        // TODO load variable
        return;
    } else if (node->name == "integer"){
        std::cout << "sipush " << node->children[0];
//        std::cout << "istore " << index; // TODO
    } else if (node->name == "boolean"){
        std::string v = node->children[0]->name;
        if (v == "true"){
            std::cout << "sipush 1";
//            std::cout << "istore " << index; // TODO
        } else if (v == "false"){
            std::cout << "sipush 0";
//            std::cout << "istore " << index; // TODO
        }

    } else if (node->name == "real") {
        std::cout << "ldc ", node->children[0];
//        std::cout << "fstore " << index; // TODO

        // TODO load constant
        return;
    }
}