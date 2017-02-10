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

//Creates a non-leaf node
Node::Node(vector<DataItem>& ex, vector<pair<string, Node>> e, Attribute& a) : examples(ex), edges(e), attr(a) {
  leaf = false;
}

//Creates a leaf node
Node::Node(bool l, vector<DataItem>& ex) : examples(ex), leaf(l) {
  decltype(examples.size()) positive_count = 0;
  for(DataItem di : examples)
    if(di.is_positive())
      ++positive_count;
  positive = (positive_count) > examples.size()/2;
}

//Applies chi square pruning recursively on the tree
//with the given input parameter to the chi squared method
void Node::prune(double pr_threshold) {
  if(!leaf) {
    //Let all children prune
    int i = 0;
    for(pair<string, Node> child : edges) {
        child.second.prune(pr_threshold);
        edges[i++] = child;
    }
    if(only_leaf_children()) { //Consider prune
      double threshold = compute_threshold(pr_threshold);
      double delta = compute_delta();
      if(delta < threshold) { //prune(make leaf node)
        leaf = true;
        positive = count_positive(examples) > examples.size()/2;
        edges.clear();
      }
    }
  }
}

//Checks if the node only has leaf children
bool Node::only_leaf_children() const {
  for(pair<string, Node> child : edges)
    if(!child.second.leaf)
      return false;
  return true;
}

//Computes the delta using the formula in the book
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

//Returns the examples for this node
vector<DataItem> Node::get_examples() {
  return examples;
}

//Computes threshold using a method from
//https://people.sc.fsu.edu/~jburkardt/cpp_src/prob/prob.cpp
double Node::compute_threshold(double right_part) const {
  double left_part = 1-right_part;
  double deg_free = edges.size() - 1;
  return chi_square_cdf_inv(left_part, deg_free);
}

//Recursive method for printing the tree to outputstream os
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

//Overloaded method for <<
ostream& operator<<(ostream& os, const Node& node) {
  node.print_tree("", os);
	return os;
}
