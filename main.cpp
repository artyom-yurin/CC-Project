#include "common/Node.hpp"
#include "grammar/Parser.hpp"
#include "lexer/Lexer.hpp"
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
  for (CNode *child : src_node->children) {
    if (res_node != child)
      delete child;
  }
  delete src_node;
  src_node = res_node;
}

int toInteger(const CNode *node) {
  if (node->name == "integer") {
    return std::stoi(node->children[0]->name);
  } else if (node->name == "real") {
    double d = std::stod(node->children[0]->name);
    int i = (int)d;
    if ((i + 0.5) <= d)
      i++;
    return i;
  } else if (node->name == "boolean") {
    if (node->children[0]->name == "true") {
      return 1;
    }
    return 0;
  }
  std::cerr << "Unknown type of CNode" << std::endl;
  std::exit(1);
}

bool toBoolean(const CNode *node) {
  if (node->name == "integer") {
    int g = std::stoi(node->children[0]->name);
    if (g == 1)
      return true;
    else if (g == 0)
      return false;
    else {
      std::cerr << "Cannot convert " << g << " to boolean" << std::endl;
      exit(1);
    }
  } else if (node->name == "real") {
    std::cerr << "Real " << node->children[0]->name
              << " cannot be converted to boolean" << std::endl;
    exit(1);
  } else if (node->name == "boolean") {
    return node->children[0]->name == "true";
  }
  std::cerr << "Unknown type of CNode" << std::endl;
  std::exit(1);
}

double toReal(const CNode *node) {
  if (node->name == "integer") {
    return std::stod(node->children[0]->name);
  } else if (node->name == "real") {
    return std::stod(node->children[0]->name);
  } else if (node->name == "boolean") {
    return node->children[0]->name == "true";
  }
  std::cerr << "Unknown type of CNode" << std::endl;
  std::exit(1);
}

CNode *calculate(CNode *node) {
  CNode *res_node = nullptr;
  if (node->name == "expression") {
    res_node = calculate(node->children[0]);

    if (res_node != node->children[0])
      changeChild(node->children[0], res_node);

    if (node->children.size() == 3) {
      auto second_node = calculate(node->children[2]);
      if (second_node != node->children[2])
        changeChild(node->children[2], second_node);

      if (!(res_node->name == "integer" || res_node->name == "boolean")) {
        if (res_node->name == "real") {
          std::cerr << "Real " << res_node->children[0]->name
                    << " cannot be converted to boolean" << std::endl;
          exit(1);
        }
        return node;
      }

      if (!(second_node->name == "integer" || second_node->name == "boolean")) {
        if (second_node->name == "real") {
          std::cerr << "Real " << second_node->children[0]->name
                    << " cannot be converted to boolean" << std::endl;
          exit(1);
        }
        return node;
      }

      bool real_l = toBoolean(res_node);
      bool real_r = toBoolean(second_node);

      std::string op = node->children[1]->name;
      bool res;
      if (op == "and") {
        res = real_l && real_r;
      } else if (op == "or") {
        res = real_l || real_r;
      } else if (op == "xor") {
        res = (real_l || real_r) && !(real_l && real_r);
      } else {
        // ERROR
        return node;
      }

      CNode *resultNode = new CNode("boolean");
      if (res) {
        resultNode->children.push_back(new CNode("true"));
      } else {
        resultNode->children.push_back(new CNode("false"));
      }
      return resultNode;
    }
    return node->children[0];
  } else if (node->name == "relation") {
    res_node = calculate(node->children[0]);

    if (res_node != node->children[0])
      changeChild(node->children[0], res_node);
    if (node->children.size() == 3) {
      auto second_node = calculate(node->children[2]);
      if (second_node != node->children[2])
        changeChild(node->children[2], second_node);

      if (!(res_node->name == "integer" || res_node->name == "real")) {
        if (res_node->name == "boolean") {
          std::cerr << "Cannot use comparing with boolean" << std::endl;
          exit(1);
        }
        return node;
      }

      if (!(second_node->name == "integer" || second_node->name == "real")) {
        if (second_node->name == "boolean") {
          std::cerr << "Cannot use comparing with boolean" << std::endl;
          exit(1);
        }
        return node;
      }

      double real_l = toReal(res_node);
      double real_r = toReal(second_node);

      std::string op = node->children[1]->name;
      bool res;
      if (op == "<") {
        res = real_l < real_r;
      } else if (op == "<=") {
        res = real_l <= real_r;
      } else if (op == ">") {
        res = real_l > real_r;
      } else if (op == ">=") {
        res = real_l >= real_r;
      } else if (op == "=") {
        res = real_l == real_r;
      } else if (op == "/=") {
        res = real_l != real_r;
      } else {
        // ERROR
        exit(1);
      }

      CNode *resultNode = new CNode("boolean");
      if (res) {
        resultNode->children.push_back(new CNode("true"));
      } else {
        resultNode->children.push_back(new CNode("false"));
      }
      return resultNode;
    }
    return node->children[0];

  } else if (node->name == "simple") {
    res_node = calculate(node->children[0]);

    if (res_node != node->children[0])
      changeChild(node->children[0], res_node);
    if (node->children.size() == 3) {
      auto second_node = calculate(node->children[2]);
      if (second_node != node->children[2])
        changeChild(node->children[2], second_node);

      if (!(res_node->name == "integer" || res_node->name == "real")) {
        if (res_node->name == "boolean") {
          std::cerr << "Cannot use arithmetic operations with boolean"
                    << std::endl;
          exit(1);
        }
        return node;
      }

      if (!(second_node->name == "integer" || second_node->name == "real")) {
        if (second_node->name == "boolean") {
          std::cerr << "Cannot use arithmetic operations with boolean"
                    << std::endl;
          exit(1);
        }
        return node;
      }

      std::string op = node->children[1]->name;
      if (res_node->name == "integer" && second_node->name == "integer") {
        int l = toInteger(res_node);
        int r = toInteger(second_node);
        int res = 0;
        if (op == "/") {
          if (r == 0) {
            std::cerr << "Сannot be divided by zero" << std::endl;
            exit(1);
          }
          res = l / r;
        } else if (op == "*") {
          res = l * r;
        } else if (op == "%") {
          if (r == 0) {
            std::cerr << "Сannot be divided by zero" << std::endl;
            exit(1);
          }
          res = l % r;
        }

        CNode *resultNode = new CNode("integer");
        resultNode->children.push_back(new CNode(std::to_string(res)));
        return resultNode;
      } else {
        double l = toReal(res_node);
        double r = toReal(second_node);

        double res = 0;
        if (op == "/") {
          if (r == 0) {
            std::cerr << "Сannot be divided by zero" << std::endl;
            exit(1);
          }
          res = l / r;
        } else if (op == "*") {
          res = l * r;
        } else if (op == "%") {
          std::cerr << "Not mod operation for real numbers" << std::endl;
          exit(1);
        }

        CNode *resultNode = new CNode("real");
        resultNode->children.push_back(new CNode(std::to_string(res)));
        return resultNode;
      }
    }
    return node->children[0];
  } else if (node->name == "not_factor") {
    auto res = calculate(node->children[1]);
    if (res != node->children[1])
      changeChild(node->children[1], res);

    if (res->name == "real") {
      std::cerr << "Real " << res->children[0]->name
                << " cannot be converted to boolean" << std::endl;
      exit(1);
    }

    if (!(res->name == "integer" || res->name == "boolean")) {
      return node;
    }

    bool real_a = toBoolean(res);

    real_a = !real_a;

    CNode *resultNode = new CNode("boolean");
    if (real_a) {
      resultNode->children.push_back(new CNode("true"));
    } else {
      resultNode->children.push_back(new CNode("false"));
    }
    return resultNode;
  } else if (node->name == "unary_factor") {
    auto res = calculate(node->children[1]);
    if (res != node->children[1])
      changeChild(node->children[1], res);

    if (res->name == "boolean") {
      std::cerr << "Cannot use unary signs with Boolean: "
                << res->children[0]->name << std::endl;
      exit(1);
    }

    std::string op = node->children[0]->name;
    std::string a = res->children[0]->name;
    if (res->name == "integer") {
      if (op == "+") {
        return res;
      }
      int result = -std::stoi(a);
      CNode *resultNode = new CNode("integer");
      resultNode->children.push_back(new CNode(std::to_string(result)));
      return resultNode;
    } else if (res->name == "real") {
      if (op == "+") {
        return res;
      }
      double result = -std::stod(a);
      CNode *resultNode = new CNode("real");
      resultNode->children.push_back(new CNode(std::to_string(result)));
      return resultNode;
    } else {
      return node;
    }

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

  } else if (node->name == "integer" || node->name == "boolean" ||
             node->name == "real" || node->name == "modifiable_primary" ||
             node->name == "modifiable_primary_array" ||
             node->name == "modifiable_primary_field") {
    return node;
  }
  return nullptr;
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
  auto ex =
      root->children[0]->children[3]->children[0]->children[0]->children[0];
  std::cout << ex->name << "\n";
  CNode *res = calculate(ex->children[0]);
  if (res != ex->children[0])
    changeChild(ex->children[0], res);
  if (res == nullptr) {
    std::cout << "KEK\n";
  }
  std::cout << res->name << "\n";
  std::cout << res->children[0]->name << "\n";
  print_tree(root);

  //  std::cout << "work\n";
  //  CNode *expression = new CNode("expression");
  //  CNode *relation = new CNode("relation");
  //  CNode *simple1 = new CNode("simple");
  //  CNode *simple2 = new CNode("simple");
  //  CNode *factor1 = new CNode("factor");
  //  CNode *summand1 = new CNode("summand");
  //  CNode *integer1 = new CNode("integer");
  //  CNode *number1 = new CNode("20");
  //
  //  CNode *mul = new CNode("*");
  //  CNode *factor2 = new CNode("factor");
  //  CNode *summand2 = new CNode("summand");
  //  CNode *integer2 = new CNode("integer");
  //  CNode *number2 = new CNode("8");
  //
  //  CNode *less = new CNode("<");
  //  CNode *simple3 = new CNode("simple");
  //  CNode *factor3 = new CNode("factor");
  //  CNode *summand3 = new CNode("summand");
  //  CNode *integer3 = new CNode("integer");
  //  CNode *number3 = new CNode("10");
  //
  //  expression->children.push_back(relation);
  //
  //  relation->children.push_back(simple1);
  //  relation->children.push_back(less);
  //  relation->children.push_back(simple2);
  //
  //  simple1->children.push_back(simple3);
  //  simple1->children.push_back(mul);
  //  simple1->children.push_back(factor1);
  //
  //  simple3->children.push_back(factor3);
  //  factor3->children.push_back(summand3);
  //  summand3->children.push_back(integer3);
  //  integer3->children.push_back(number3);
  //
  //  factor1->children.push_back(summand1);
  //  summand1->children.push_back(integer1);
  //  integer1->children.push_back(number1);
  //
  //  simple2->children.push_back(factor2);
  //  factor2->children.push_back(summand2);
  //  summand2->children.push_back(integer2);
  //  integer2->children.push_back(number2);
  //
  //  CNode *res = calculate(simple1);
  //  if (res == nullptr) {
  //    std::cout << "KEK\n";
  //  }
  //  std::cout << res->name << "\n";
  //  std::cout << res->children[0]->name << "\n";

  return 0;
}
