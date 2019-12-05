#include "common/Node.hpp"
#include "grammar/Parser.hpp"
#include "lexer/Lexer.hpp"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <semantic_analyzer/CAnalyzer.hpp>
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

  CAnalayzer analyzer;
  std::cout << "Check reachable of components\n";
  if (!analyzer.check_reachable(root)) {
    std::cerr << "ERROR: see above" << std::endl;
    return 1;
  }
  std::cout << "Everything is correct\n";
  print_tree(root);
  return 0;
}
