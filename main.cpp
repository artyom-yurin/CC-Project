#include "common/Node.hpp"
#include "grammar/Parser.hpp"
#include "lexer/Lexer.hpp"
#include "semantic_analyzer/expressions/expressionProcessing.hpp"
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

bool check_reachable(CNode *node);

bool check_expression(CNode *node) {
  if (node->name == "expression") {
    if (!check_expression(node->children[0]))
      return false;

    if (node->children.size() == 3) {
      if (!check_expression(node->children[2]))
        return false;
    }
    return true;
  } else if (node->name == "relation") {
    if (!check_expression(node->children[0]))
      return false;

    if (node->children.size() == 3) {
      if (!check_expression(node->children[2]))
        return false;
    }
    return true;
  } else if (node->name == "simple") {
    if (!check_expression(node->children[0]))
      return false;

    if (node->children.size() == 3) {
      if (!check_expression(node->children[2]))
        return false;
    }
    return true;
  } else if (node->name == "not_factor") {
    return check_expression(node->children[1]);
  } else if (node->name == "unary_factor") {
    return check_expression(node->children[1]);
  } else if (node->name == "factor") {
    if (!check_expression(node->children[0]))
      return false;

    if (node->children.size() == 3) {
      if (!check_expression(node->children[2]))
        return false;
    }
    return true;
  } else if (node->name == "summand") {
    return check_expression(node->children[0]);
  } else if (node->name == "modifiable_primary_array" || node->name == "modifiable_primary_field" || node->name == "modifiable_primary") {
    return currentTable->check_modifiable(node);
  } else if (node->name == "integer" || node->name == "boolean" ||
             node->name == "real") {
    return true;
  }
  return false;
}

bool check_statements(CNode *node) {
  CNode *statement = node->children[0];
  if (statement->name == "return") {
    // first processing
    auto ret_value = statement->children[0];
    if (!processingExpression(ret_value, 0)) {
      return false;
    }
    return check_expression(ret_value->children[0]);
  } else if (statement->name == "assignment") {
    if (!currentTable->check_modifiable(statement->children[0])){
      return false;
    }
    return processingExpression(statement, 1);
  } else if (statement->name == "routine_call") {
    std::string functionName = statement->children[0]->name;
    return currentTable->checkFunctionCall(functionName,
                                           statement->children[1]);
  } else if (statement->name == "while_loop") {
    return true; // TODO: implement
  } else if (statement->name == "for_loop") {
    return true; // TODO: implement
  } else if (statement->name == "if_statement") {
    if(!processingExpression(statement, 0)){
      return false;
    }
    std::string key = currentTable->addSubScope();
    if (key.empty())
    {
      return false;
    }
    currentTable = currentTable->getSubScopeTable(key);
    if (!check_reachable(statement->children[1])){
      return false;
    }
    currentTable = currentTable->getParent();
    if (statement->children[2] == nullptr)
    {
      return true;
    }
    key = currentTable->addSubScope();
    if (key.empty())
    {
      return false;
    }
    currentTable = currentTable->getSubScopeTable(key);
    if (!check_reachable(statement->children[2]->children[0])){
      return false;
    }
    currentTable = currentTable->getParent();
    return true;
  }
  return false;
}

bool check_simple_declaration(CNode *node) {
  CNode *dec = node->children[0];
  if (dec->name == "variable_declaration_auto") {
    if (dec->children.size() != 2) {
      std::cerr << "Something wrong with CNode " << node->name << std::endl;
      return false;
    }
    return currentTable->addAutoVariable(dec->children[0]->name,
                                         dec->children[1]);
  } else if (dec->name == "variable_declaration") {
    if (dec->children.size() != 3) {
      std::cerr << "Something wrong with CNode " << node->name << std::endl;
      return false;
    }
    return currentTable->addVariable(dec->children[0]->name, dec->children[1],
                                     dec->children[2]);
  } else if (dec->name == "type_declaration") {
    if (dec->children.size() != 2) {
      std::cerr << "Something wrong with CNode " << node->name << std::endl;
      return false;
    }
    return currentTable->addType(dec->children[0]->name, dec->children[1]);
  }
  return false;
}

bool check_routine_declaration(CNode *node) {
  if (node->children.size() != 4) {
    std::cerr << "Something wrong with CNode " << node->name << std::endl;
    return false;
  }
  std::string functionName = node->children[0]->name;
  CNode *parameters = node->children[1];
  CNode *returnType = node->children[2];
  if (!currentTable->addFunction(functionName, returnType, parameters)) {
    std::cerr << "Cannot create function " << functionName << std::endl;
    return false;
  }
  currentTable = currentTable->getSubScopeTable(functionName);
  std::cout << "Processing body of function " << functionName << "\n";
  CNode *body = node->children[3];
  if (check_reachable(body)) {
    currentTable = currentTable->getParent();
    std::cout << "Body of function " << functionName << " was processed\n";
    return true;
  }
  return false;
}

bool check_reachable(CNode *node) {
  if (node == nullptr)
    return true;

  if (node->name == "simple_declaration") {
    return check_simple_declaration(node);
  } else if (node->name == "routine_declaration") {
    return check_routine_declaration(node);
  } else if (node->name == "statement") {
    return check_statements(node);
  } else if (node->name == "program" || node->name == "body") {
    for (int i = 0; i < node->children.size(); i++) {
      if (!check_reachable(node->children[i])) {
        std::cerr << "ERROR: in " << node->name << " with child "
                  << node->children[i]->name << std::endl;
        return false;
      }
    }
    return true;
  }
  std::cerr << "ERROR: Unknown CNode type" << std::endl;
  return false;
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
  std::cout << "Check reachable of components\n";
  if (!check_reachable(root)) {
    std::cerr << "ERROR: see above" << std::endl;
    return 1;
  }
  std::cout << "Everything is correct\n";
  print_tree(root);
  //  assert(originalTable->isType("x"));
  //  assert(originalTable->isVariable("g"));
  //  assert(originalTable->isVariable("h"));
  //  assert(originalTable->isFunction("main"));
  //  auto mainTable = originalTable->getSubScopeTable("main");
  //  assert(mainTable != nullptr);
  //  assert(mainTable->isVariable("g"));
  //  assert(mainTable->isVariable("y"));
  //  assert(originalTable->isFunction("some"));
  //  auto someTable = originalTable->getSubScopeTable("some");
  //  assert(someTable != nullptr);
  return 0;
}
