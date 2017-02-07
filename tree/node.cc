#include "node.h"
#include "dataitem.h"
#include "attribute.h"
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

void Node::print_tree() const {
  print_tree("");
}

void Node::print_tree(string ind) const {
  if(leaf) {
    if(positive)
      cout << ": Yes" << endl;
    else
      cout << ": No" << endl;
  } else {
    for(pair<string, Node> e : edges) {
      cout << ind << attr.get_name() + " = " + e.first;
      if(!e.second.leaf)
          cout << endl;
      e.second.print_tree(ind + "\t");
    }
  }
}

ostream& operator<<(ostream& os, const Node& node) {
  if(node.leaf) {
    if(node.positive)
      os << ": Yes" << endl;
    else
      os << ": No" << endl;
  } else {
    for(pair<string, Node> e : node.edges) {
      os << node.attr.get_name() + " = " + e.first;
      if(!e.second.leaf)
        os << endl << "\t" << e.second;
      else
        os << e.second;
    }
  }
	return os;
}
