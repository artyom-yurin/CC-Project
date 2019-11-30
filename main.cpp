#include "common/Node.hpp"
#include "grammar/Parser.hpp"
#include "lexer/Lexer.hpp"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <semantic_analyzer/ControlTable.hpp>
#include <sstream>

void print_node(CNode *node, int margin) {
  if (node == nullptr)
    return;
  for (int i = 0; i < margin; i++)
    std::cout << "   ";
  std::cout << "<" << node->name << ">\n";
  for (int j = 0; j < node->children.size(); j++)
    print_node(node->children[j], margin + 1);
}

void print_tree(CNode *root) { print_node(root, 0); }

std::shared_ptr<ControlTable> originalTable;
std::shared_ptr<ControlTable> currentTable;

bool check_reachable(CNode *node) {
  if (node == nullptr)
    return true;

  if (node->name == "simple_declaration") {
    CNode *dec = node->children[0];
    if (dec->name == "variable_declaration_auto") {
      if (dec->children.size() != 2)
        return false;
      return currentTable->addAutoVariable(dec->children[0]->name,
                                           dec->children[1]);
    } else if (dec->name == "variable_declaration") {
      if (dec->children.size() != 3)
        return false;
      return currentTable->addVariable(dec->children[0]->name, dec->children[1], dec->children[2]);
    } else if (dec->name == "type_declaration") {
      if (dec->children.size() != 2)
        return false;
      return currentTable->addType(dec->children[0]->name, dec->children[1]);
    }
  } else if (node->name == "routine_declaration") {
    return true;
  } else if (node->name == "program") {
    for (int i = 0; i < node->children.size(); i++) {
      if (!check_reachable(node->children[i])){
        std::cerr << "Error here" << i << "\n";
        return false;
      }
    }
    return true;
  }
  return true;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Invalid number of args" << std::endl;
    std::cerr << "Usage: " << argv[0] << " <path_to_source>" << std::endl;
    return 1;
  }

  std::ifstream src_file(argv[1]);
  if (!src_file.is_open()) {
    std::cerr << "File don't open" << std::endl;
    return 1;
  }
  std::stringstream buffer;

  buffer << src_file.rdbuf();
  Lexer *lexer = new Lexer(buffer.str());
  CNode *root = nullptr;
  yy::parser parser(lexer, (void **)&root);
  parser.parse();
  if (root == nullptr)
    return 1;

  print_tree(root);
  originalTable = std::make_shared<ControlTable>();
  currentTable = originalTable;
  if(!check_reachable(root))
  {
    std::cerr << "Semntic error:" << "тип понятно" << std::endl;
    return 1;
  }
  std::cout << "Гений\n";

  assert(currentTable->isType("x"));
  assert(currentTable->isType("y"));
  assert(currentTable->isVariable("g"));
  assert(currentTable->isVariable("e"));
  assert(currentTable->isVariable("h"));
  return 0;
}
