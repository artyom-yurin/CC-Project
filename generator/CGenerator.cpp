#include <iostream>
#include <fstream>
#include <semantic_analyzer/ControlTable.hpp>
#include "CGenerator.hpp"
#include "IndexTable.hpp"

void CGenerator::translate_get_modifiable(CNode *node, std::ofstream &output) {
    if (node->name == "modifiable_primary") {
        auto s = current_index_table_->getIndex(node->children[0]->name);
        if (s.second == Type::Arr_Boolean)
            output << "   aload " << .first << "\n";
    } else if (node->name == "modifiable_primary_array") {
        translate_get_modifiable(node->children[0], output);
        translate_expression(node->children[1], output);
        output << "   aaload\n";
    } else if (node->name == "modifiable_primary_field") {
        // TODO POTOM
    }
}

void CGenerator::translate_expression(CNode *node, std::ofstream &output) {
    if (node->name == "expression") {
        translate_expression(node->children[0], output);
        if (node->children.size() == 3) {
            translate_expression(node->children[2], output);
            std::string op = node->children[1]->name;
            if (op == "and") {
                output << "   iand" << "\n";
            } else if (op == "or") {
                output << "   ior" << "\n";
            } else if (op == "xor") {
                output << "   ixor" << "\n";
            }
        }
        return;
    } else if (node->name == "relation") {
        translate_expression(node->children[0], output);
        if (node->children.size() == 3) {
            translate_expression(node->children[2], output);
            std::string op = node->children[1]->name;
            if (op == "<") {
                output << "fcmpg \n ifge L3 \n iconst_1 \n goto L4 \n L3: \n iconst_0  \n L4: \n return" << "\n";
            } else if (op == "<=") {
                output << "fcmpg \n ifgt L3 \n iconst_1 \n goto L4 \n L3: \n iconst_0  \n L4: \n return" << "\n";
            } else if (op == ">") {
                output << "fcmpl \n ifle L3 \n iconst_1 \n goto L4 \n L3: \n iconst_0  \n L4: \n return" << "\n";
            } else if (op == ">=") {
                output << "fcmpl \n iflt L3 \n iconst_1 \n goto L4 \n L3: \n iconst_0  \n L4: \n return" << "\n";
            } else if (op == "=") {
                output << "fcmpl \n ifne L3 \n iconst_1 \n goto L4 \n L3: \n iconst_0  \n L4: \n return" << "\n";
            } else if (op == "/=") {
                output << "fcmpl \n ifeq L3 \n iconst_1 \n goto L4 \n L3: \n iconst_0  \n L4: \n return" << "\n";
            }
        }
        return;
    } else if (node->name == "simple") {
        translate_expression(node->children[0], output);
        if (node->children.size() == 3) {
            translate_expression(node->children[2], output);
            std::string op = node->children[1]->name;
            if (op == "*") {
                output << "   imul" << "\n";
            } else if (op == "/") {
                output << "   idiv" << "\n";
            } else if (op == "%") {
                output << "   irem" << "\n";
            }
        }
        return;
    } else if (node->name == "not_factor") {
        translate_expression(node->children[1], output);
        // TODO not operation
        return;
    } else if (node->name == "unary_factor") {
        output << "   ineg" << "\n";
        return;
    } else if (node->name == "factor") {
        translate_expression(node->children[0], output);
        translate_expression(node->children[2], output);
        std::string op = node->children[1]->name;
        if (op == "+") {
            output << "   iadd" << "\n";

        } else if (op == "-") {
            output << "   isub" << "\n";
        }
        return;
    } else if (node->name == "summand") {
        translate_expression(node->children[0], output);
    } else if (node->name == "modifiable_primary_array" || node->name == "modifiable_primary_field" ||
               node->name == "modifiable_primary") {
        translate_get_modifiable(node, output);
    } else if (node->name == "integer") {
        output << "   sipush " << node->children[0]->name << "\n";
    } else if (node->name == "boolean") {
        std::string v = node->children[0]->name;
        if (v == "true") {
            output << "   sipush 1" << "\n";
        } else if (v == "false") {
            output << "   sipush 0" << "\n";
        } else if (node->name == "real") {
            output << "   ldc " << node->children[0]->name << "\n";
            return;
        }
    }
}


void CGenerator::generate(CNode *node, const std::string &filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Cannot open file" << std::endl;
        return;
    }
    outfile << ".class public temp/Test ; не трогать\n"
               ".super java/lang/Object ; не трогать\n"
               "\n"
               "; standard initializer\n"
               ".method public <init>()V\n"
               "   aload_0\n"
               "   invokenonvirtual java/lang/Object/<init>()V\n"
               "   return\n"
               ".end method\n"
               "\n"
               ".method public static main([Ljava/lang/String;)V\n"
               "   .limit locals 8\n"
               "   .limit stack 8\n";

    outfile << "   return\n"
               ".end method\n\n";
    //TODO create a file
    //TODO genrate
    translate_CNode(node, outfile);
    outfile.close();
}

void CGenerator::translate_simple_declaration(CNode *node, std::ofstream &output) {
    CNode *dec = node->children[0];
    if (dec->name == "variable_declaration_auto" || dec->name == "variable_declaration") {
        current_index_table_->addIndex(dec->children[0]->name);
        // TODO ARRAY
    }
}

CGenerator::CGenerator(std::shared_ptr<ControlTable> ct) {
    original_index_table_ = std::make_shared<IndexTable>();
    current_index_table_ = original_index_table_;
    ct_ = ct;
}

void CGenerator::translate_CNode(CNode *node, std::ofstream &output) {
    if (node->name == "simple_declaration") {
        return translate_simple_declaration(node, output);
    } else if (node->name == "routine_declaration") {
        return translate_routine_declaration(node, output);
    } else if (node->name == "statement") {
        return translate__statements(node, output);
    } else if (node->name == "program" || node->name == "body") {
        for (int i = 0; i < node->children.size(); i++) {
            translate_CNode(node->children[i], output);
        }
    }
}

void CGenerator::translate_routine_declaration(CNode *node, std::ofstream &output) {
    std::string functionName = node->children[0]->name;
    auto ct = ct_->getSubScopeTable(functionName);
    int local_size = ct_->countVariables(false);
    int stack_size = local_size + 1;
    CNode *parameters = node->children[1];
    CNode *returnType = node->children[2];

    output << ".method public static " << functionName << "()V\n"; // TODO sad Parameters and Return value
    output << "   .limit locals " << local_size << "\n"
           << "   .limit stack " << stack_size << "\n";
    //TODO IMPLEMENT
    CNode *body = node->children[3];
    translate_CNode(body, output);
    output << ".end method\n\n";
}

void CGenerator::translate__statements(CNode *node, std::ofstream &output) {
    CNode *statement = node->children[0];
    if (statement->name == "return") {
        auto ret_value = statement->children[0];
        translate_expression(ret_value->children[0], output);
        output << "   areturn\n";
    } else if (statement->name == "assignment") {
        //TODO IMPLEMENT
        /*
        translate_expression(statement->children[1]);
        output << "istore " << current_index_table_->getIndex(statement->children[0]->name); //TODO ARRAY OR USUAL / TYPE SWITCH
    */
    } else if (statement->name == "routine_call") {
        //TODO IMPLEMENT
        /*std::string functionName = statement->children[0]->name;
        return currentTable->checkFunctionCall(functionName,
                                               statement->children[1]);*/
    } else if (statement->name == "while_loop") {
        //TODO IMPLEMENT
        /*
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
        return true;*/
    } else if (statement->name == "for_loop") {






        //TODO IMPLEMENT
        /*
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
        return true;*/
    } else if (statement->name == "if_statement") {
        //TODO IMPLEMENT
        /*
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
        return true;*/
    }
}

void CGenerator::translate_set_modifiable(CNode *node, std::ofstream &output) {
    if (node->name == "modifiable_primary") {
        // TODO Swith type
        output << "   istore " << current_index_table_->getIndex(node->children[0]->name) << "\n";
    } else if (node->name == "modifiable_primary_array") {
        translate_get_modifiable(node->children[0], output);
        translate_expression(node->children[1], output);
    } else if (node->name == "modifiable_primary_field") {
        // TODO POTOM
    }
}
