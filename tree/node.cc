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

ostream& operator<<(ostream& os, const Node& node) {
  os << "hej node " << node.leaf << endl;
	return os;
}
