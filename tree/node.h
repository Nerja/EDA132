#ifndef NODE_H
#define NODE_H
#include "dataitem.h"
#include "attribute.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class Node {

public:
  Node(std::vector<DataItem>& ex, std::vector<std::pair<std::string, Node>> edges, Attribute& attr);
  Node(bool leaf, std::vector<DataItem>& examples);
  friend std::ostream& operator<<(std::ostream& os, const Node& node);
  void prune(double threshold);
  std::vector<DataItem> get_examples();
private:
  std::vector<DataItem> examples;
  std::vector<std::pair<std::string, Node>> edges;
  bool leaf;
  bool positive;
  Attribute attr;
  void print_tree(std::string ind, std::ostream& os) const;
  double compute_threshold(double right_part) const;
  double compute_delta();
  bool only_leaf_children() const;
};

#endif
