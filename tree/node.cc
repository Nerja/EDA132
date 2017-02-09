#include "node.h"
#include "dataitem.h"
#include "attribute.h"
#include "prob.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

Node::Node(vector<DataItem>& ex, vector<pair<string, Node>> e, Attribute& a) : examples(ex), edges(e), attr(a) {
  leaf = false;
}

Node::Node(bool l, vector<DataItem>& ex) : examples(ex), leaf(l) {
  decltype(examples.size()) positive_count = 0;
  for(DataItem di : examples)
    if(di.is_positive())
      ++positive_count;
  positive = (positive_count) > examples.size()/2;

}

void Node::prune() {
  
}

double Node::compute_threshold(double right_part) const {
  double left_part = 1 - right_part;
  double deg_free = examples.size() - 1;
  return chi_square_cdf_inv(left_part, deg_free);
}

void Node::print_tree(string ind, ostream& os) const {
  if(leaf) {
    if(positive)
      os << ": Yes" << endl;
    else
      os << ": No" << endl;
  } else {
    for(pair<string, Node> e : edges) {
      os << ind << attr.get_name() + " = " + e.first;
      if(!e.second.leaf)
          os << endl;
      e.second.print_tree(ind + "\t", os);
    }
  }
}

ostream& operator<<(ostream& os, const Node& node) {
  node.print_tree("", os);
	return os;
}
