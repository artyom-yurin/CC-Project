#include "CAnalyzer.hpp"
#include <common/Node.hpp>
#include <iostream>

bool CAnalayzer::check_expression(CNode *node) {
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
  } else if (node->name == "modifiable_primary_array" ||
             node->name == "modifiable_primary_field" ||
             node->name == "modifiable_primary") {
    return currentTable->check_modifiable(node);
  } else if (node->name == "integer" || node->name == "boolean" ||
             node->name == "real") {
    return true;
  }
  return false;
}

bool CAnalayzer::check_statements(CNode *node) {
  CNode *statement = node->children[0];
  if (statement->name == "return") {
    // first processing
    auto ret_value = statement->children[0];
    if (!currentTable->processingExpression(ret_value, 0)) {
      return false;
    }
    return check_expression(ret_value->children[0]);
  } else if (statement->name == "assignment") {
    if (!currentTable->check_modifiable(statement->children[0])) {
      return false;
    }
    return currentTable->processingExpression(statement, 1);
  } else if (statement->name == "routine_call") {
    std::string functionName = statement->children[0]->name;
    return currentTable->checkFunctionCall(functionName,
                                           statement->children[1]);
  } else if (statement->name == "while_loop") {
    if (!currentTable->processingExpression(statement, 0)) {
      return false;
    }
    std::string key = currentTable->addSubScope();
    if (key.empty()) {
      return false;
    }
    currentTable = currentTable->getSubScopeTable(key);

    if (!check_reachable(statement->children[1])) {
      return false;
    }
    currentTable = currentTable->getParent();
    return true;
  } else if (statement->name == "for_loop") {
    CNode *range = statement->children[1];
    std::vector<CNode *> new_range = {};
    if (range->children[0] != nullptr) {
      new_range.push_back(range->children[1]);
      new_range.push_back(range->children[2]);
    } else {
      new_range.push_back(range->children[2]);
      new_range.push_back(range->children[1]);
      delete range->children[0];
    }
    range->children = new_range;
    if (!currentTable->processingExpression(range, 0) ||
        !currentTable->processingExpression(range, 1)) {
      return false;
    }
    std::string key = currentTable->addSubScope();
    if (key.empty()) {
      return false;
    }
    currentTable = currentTable->getSubScopeTable(key);

    if (!currentTable->addCounter(statement->children[0]->name)) {
      return false;
    }
    if (!check_reachable(statement->children[2])) {
      return false;
    }
    currentTable = currentTable->getParent();
    return true;
  } else if (statement->name == "if_statement") {
    if (!currentTable->processingExpression(statement, 0)) {
      return false;
    }
    std::string key = currentTable->addSubScope();
    if (key.empty()) {
      return false;
    }
    currentTable = currentTable->getSubScopeTable(key);
    if (!check_reachable(statement->children[1])) {
      return false;
    }
    currentTable = currentTable->getParent();
    if (statement->children[2] == nullptr) {
      return true;
    }
    key = currentTable->addSubScope();
    if (key.empty()) {
      return false;
    }
    currentTable = currentTable->getSubScopeTable(key);
    if (!check_reachable(statement->children[2]->children[0])) {
      return false;
    }
    currentTable = currentTable->getParent();
    return true;
  }
  return false;
}

bool CAnalayzer::check_simple_declaration(CNode *node) {
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

bool CAnalayzer::check_routine_declaration(CNode *node) {
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

bool CAnalayzer::check_reachable(CNode *node) {
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
CAnalayzer::CAnalayzer() {
  originalTable = std::make_shared<ControlTable>();
  currentTable = originalTable;
}
std::shared_ptr<ControlTable> CAnalayzer::getOriginalTable() {
  return originalTable;
}
