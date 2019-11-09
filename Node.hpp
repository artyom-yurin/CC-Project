#include <string>
#include <vector>
#include <memory>

class CNode {
public:
      std::string name;
      std::vector<CNode*> children;

      CNode(const std::string &name);
};
