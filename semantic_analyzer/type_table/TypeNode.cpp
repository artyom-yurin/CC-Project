#include "semantic_analyzer/type_table/TypeNode.hpp"
#include "semantic_analyzer/symbol_table/SymbolNode.hpp"

Types TypeNode::getType() const { return type; }

AutoType::AutoType() { this->type = Types ::Auto; }
std::string AutoType::toStr() const { return "Auto: need to determine"; }

SimpleType::SimpleType(const std::string &name) {
  this->type = Types ::Simple;
  this->name = name;
}
std::string SimpleType::toStr() const { return "Simple type: " + name; }

ArrayType::ArrayType(CNode *expression, std::shared_ptr<TypeNode>type)
{
  this->type = Types ::Array;
  this->arrayType = type;
  this->expression = expression;
}
std::string ArrayType::toStr() const { return "Array type: " + arrayType->toStr(); }

RecordType::RecordType(const std::vector<std::shared_ptr<VariableNode>> &fields) {
  this->type = Types ::Record;
  this->fields = fields;
}

std::string RecordType::toStr() const { 
    std::string result = "Record type\n"; 
    for (int i=0; i < fields.size(); i++) {
        result += fields[i]->toString() + "\n";
    }
    return result;
}
std::string NoTypeNode::toStr() const { return "No type"; }
NoTypeNode::NoTypeNode() {
  this->type = Types ::NoType;
}
