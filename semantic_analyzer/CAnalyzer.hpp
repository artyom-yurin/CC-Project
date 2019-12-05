#ifndef ICOMPILER_ANALYZER_HPP
#define ICOMPILER_ANALYZER_HPP

#include "ControlTable.hpp"

class CAnalayzer {

public:
  CAnalayzer();
  ~CAnalayzer() = default;

  bool check_reachable(CNode *node);

  std::shared_ptr<ControlTable> getOriginalTable();

private:
  bool check_expression(CNode *node);
  bool check_statements(CNode *node);
  bool check_simple_declaration(CNode *node);
  bool check_routine_declaration(CNode *node);
  std::shared_ptr<ControlTable> originalTable;
  std::shared_ptr<ControlTable> currentTable;
};

#endif // ICOMPILER_ANALYZER_HPP
