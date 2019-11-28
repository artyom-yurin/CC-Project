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

  return 0;
}
