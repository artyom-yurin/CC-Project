#include "common/Node.hpp"
//#include "lexer/Lexer.hpp"
//#include "grammar/Parser.hpp"
#include <fstream>
#include <iostream>
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

void changeChild(CNode *&src_node, CNode *res_node) {
  delete src_node;
  src_node = res_node;
}

CNode *calculate(CNode *node) {
  CNode *res_node = nullptr;
  if (node->name == "expression") {
    res_node = calculate(node->children[0]);

    if (res_node != node->children[0])
      changeChild(node->children[0], res_node);

    if (node->children.size() == 3) {
      return node;
    }
    return node->children[0];
  } else if (node->name == "relation") {
    res_node = calculate(node->children[0]);

    if (res_node != node->children[0])
      changeChild(node->children[0], res_node);
    if (node->children.size() == 3) {
      return node;
    }
    return node->children[0];

  } else if (node->name == "simple") {
    res_node = calculate(node->children[0]);

    if (res_node != node->children[0])
      changeChild(node->children[0], res_node);
    if (node->children.size() == 3) {
      return node;
    }
    return node->children[0];
  } else if (node->name == "factor") {
    res_node = calculate(node->children[0]);
    if (res_node != node->children[0])
      changeChild(node->children[0], res_node);

    if (node->children.size() == 3) {
      return node;
    }
    return node->children[0];
  } else if (node->name == "summand") {
    return calculate(node->children[0]);

  } else if (node->name == "integer") {
    return node;
  }
  return nullptr;
}

int main(int argc, char *argv[]) {
  /*if (argc != 2) {
    std::cerr << "Invalid number of args" << std::endl;
    std::cerr << "Usage: " << argv[0] << " <path_to_source>" << std::endl;
    return 1;
  }

  std::ifstream src_file(argv[1]);
  if (!src_file.is_open())
  {
    std::cerr << "File don't open" << std::endl;
    return 1;
  }
  std::stringstream buffer;

  buffer << src_file.rdbuf();
  Lexer *lexer = new Lexer(buffer.str());
  CNode* root = nullptr;
  yy::parser parser(lexer, (void**)&root);
  parser.parse();
  if (root == nullptr)
    return 1;

  print_tree(root);*/
  std::cout << "work\n";
  CNode *expression = new CNode("expression");
  CNode *relation = new CNode("relation");
  CNode *simple1 = new CNode("simple");
  CNode *simple2 = new CNode("simple");
  CNode *factor1 = new CNode("factor");
  CNode *summand1 = new CNode("summand");
  CNode *integer1 = new CNode("integer");
  CNode *number1 = new CNode("20");

  CNode *mul = new CNode("*");
  CNode *factor2 = new CNode("factor");
  CNode *summand2 = new CNode("summand");
  CNode *integer2 = new CNode("integer");
  CNode *number2 = new CNode("8");

  CNode *less = new CNode("<");
  CNode *simple3 = new CNode("simple");
  CNode *factor3 = new CNode("factor");
  CNode *summand3 = new CNode("summand");
  CNode *integer3 = new CNode("integer");
  CNode *number3 = new CNode("10");

  expression->children.push_back(relation);

  relation->children.push_back(simple1);
  relation->children.push_back(less);
  relation->children.push_back(simple2);

  simple1->children.push_back(simple3);
  simple1->children.push_back(mul);
  simple1->children.push_back(factor1);

  simple3->children.push_back(factor3);
  factor3->children.push_back(summand3);
  summand3->children.push_back(integer3);
  integer3->children.push_back(number3);

  factor1->children.push_back(summand1);
  summand1->children.push_back(integer1);
  integer1->children.push_back(number1);

  simple2->children.push_back(factor2);
  factor2->children.push_back(summand2);
  summand2->children.push_back(integer2);
  integer2->children.push_back(number2);

  CNode *res = calculate(simple1);
  if (res == nullptr) {
    std::cout << "KEK\n";
  }
  std::cout << res->name << "\n";
  std::cout << res->children[0]->name << "\n";

  return 0;
}
