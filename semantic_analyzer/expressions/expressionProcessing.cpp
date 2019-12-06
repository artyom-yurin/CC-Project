#include "expressionProcessing.hpp"
#include <iostream>

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

      if (res_node->name == "integer" && second_node->name == "integer")
      {
        int real_l = toInteger(res_node);
        int real_r = toInteger(second_node);

        std::string op = node->children[1]->name;
        int res;
        if (op == "+") {
          res = real_l + real_r;
        } else if (op == "-") {
          res = real_l - real_r;
        } else {
          // ERROR
          exit(1);
        }

        CNode *resultNode = new CNode("integer");
        resultNode->children.push_back(new CNode(std::to_string(res)));
        return resultNode;
      }
      else {
        double real_l = toReal(res_node);
        double real_r = toReal(second_node);

        std::string op = node->children[1]->name;
        double res;
        if (op == "+") {
          res = real_l + real_r;
        } else if (op == "-") {
          res = real_l - real_r;
        } else {
          // ERROR
          exit(1);
        }

        CNode *resultNode = new CNode("real");
        resultNode->children.push_back(new CNode(std::to_string(res)));
        return resultNode;
      }
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

bool processingExpression(CNode*&parent, int idChild){
  CNode *res = calculate(parent->children[idChild]);
  if (res == nullptr)
    return false;
  if (res != parent->children[idChild])
    changeChild(parent->children[idChild], res);
  return true;
}
