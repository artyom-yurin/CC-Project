#ifndef ICOMPILER_CGENERATOR_HPP
#define ICOMPILER_CGENERATOR_HPP

#include "IndexTable.hpp"
#include <common/Node.hpp>
#include "semantic_analyzer/ControlTable.hpp"

class CGenerator {
public:
  CGenerator(std::shared_ptr<ControlTable> ct);
  ~CGenerator() = default;
  void generate(CNode *node, const std::string &filename);

private:
  void translate_expression(CNode *node, std::ofstream &output);
  void translate_get_modifiable(CNode *node, std::ofstream &output);
  void translate_set_modifiable(CNode *node, std::ofstream &output);
  void translate_CNode(CNode *node, std::ofstream &output,  int& scope);
  void translate_simple_declaration(CNode *node, std::ofstream &output);
  void translate_routine_declaration(CNode *node, std::ofstream &output);
  void translate__statements(CNode *node, std::ofstream &output,  int& scope);

  std::shared_ptr<IndexTable> original_index_table_;
  std::shared_ptr<IndexTable> current_index_table_;
  std::shared_ptr<ControlTable> ct_;
  std::shared_ptr<ControlTable> current_ct_;
};

#endif // ICOMPILER_CGENERATOR_HPP
