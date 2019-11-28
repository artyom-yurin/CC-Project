#include "ControlTable.hpp"
#include <assert.h>
#include <iostream>


int main(int argc, char *argv[]) {
  std::shared_ptr<ControlTable> cc = std::make_shared<ControlTable>();
  assert(cc->isType("integer"));
  assert(cc->addSubScope("some"));
  auto ccc = cc->getSubScopeTable("some");

  assert(ccc->isType("integer"));
  assert(ccc->addSimpleType("some", "integer"));
  assert(ccc->isType("some"));
  assert(!(cc->isType("some")));
  CNode*parameters = new CNode("parameters");
  CNode*param1 = new CNode("parameter_declaration");
  param1 ->children.push_back(new CNode("x"));
  param1 ->children.push_back(new CNode("integer"));

  CNode*param2 = new CNode("parameter_declaration");
  param2->children.push_back(new CNode("y"));
  param2->children.push_back(new CNode("boolean"));
  parameters->children.push_back(param1);
  parameters->children.push_back(param2);

  assert(cc->addFunction("Test", "integer", parameters));
  return 0;
}
