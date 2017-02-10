#include "node.h"
#include "dataitem.h"
#include "attribute.h"
#include "prob.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "treebuilder.h"
#include <cmath>

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
  if(!leaf) {
    //Let all children prune
    for(pair<string, Node&> child : edges)
      child.second.prune();
    if(only_children()) { //Consider prune
      double threshold = compute_threshold(0.1);
      double delta = compute_delta();
      if(delta < threshold) { //prune
        leaf = true;
        positive = count_positive(examples) > examples.size()/2;
        edges.clear();
      }
    }
  }
}

bool Node::only_children() const {
  for(pair<string, Node> child : edges)
    if(!child.second.leaf)
      return false;
  return true;
}

double Node::compute_delta() {
  double delta = 0;
  double p = count_positive(examples);
  double n = examples.size() - p;
  for(pair<string, Node> child : edges) {
    vector<DataItem> child_examples = child.second.get_examples();
    double child_size = child_examples.size();
    double pk = count_positive(child_examples);
    double nk = child_size - pk;
    double hatpk = p * (child_size/examples.size());
    double hatnk = n * (child_size/examples.size());
    delta += pow(pk - hatpk,2)/hatpk + pow(nk - hatnk,2)/hatnk;
  }
  return delta;
}

vector<DataItem> Node::get_examples() {
  return examples;
}

double Node::compute_threshold(double right_part) const {
  double left_part = 1-right_part;
  double deg_free = edges.size() - 1;
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
