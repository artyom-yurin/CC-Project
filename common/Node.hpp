#ifndef CC_PROJECT_NODE_HPP
#define CC_PROJECT_NODE_HPP

#include <string>
#include <vector>
#include <memory>

class CNode {
public:
      std::string name;
      std::vector<CNode*> children;

      CNode(const std::string &name);
};

#endif // CC_PROJECT_NODE_HPP
