#include "attribute.h"
#include "dataitem.h"
#include <vector>
#include "treebuilder.h"
#include "node.h"
#include <string>
#include <cmath>

using namespace std;

//Wrapper method for the ID3 algorithm
Node build_tree(vector<Attribute>& attributes, vector<DataItem>& examples) {
  return build_tree(attributes, examples, examples);
}

//Recursive method for building the tree with ID3
//Corresponds to the psuedo code algorithm given in the book
Node build_tree(vector<Attribute>& attributes, vector<DataItem>& examples, vector<DataItem>& parent_examples) {
  if(examples.size() < 1) { //First basecase in the book
    Node leaf_node(true, parent_examples);
    return leaf_node;
  } else if(same_classifier(examples) || attributes.size() < 1) { //Second&third basecase in the book
    Node leaf_node(true, examples);
    return leaf_node;
  } else {
    //Finds the best attribute
    Attribute attr = importance(attributes, examples);

    //Adds the subtrees computed by recursively calling the build_tree method
    vector<pair<string, Node>> edges;
    for(string v : attr.get_values()) {
      vector<DataItem> exs = get_exs(examples, attr, v);
      vector<Attribute> attr_excluded = exclude(attributes, attr);
      Node child = build_tree(attr_excluded, exs, examples);
      edges.push_back(make_pair(v, child));
    }
    Node node(examples, edges, attr);     //Creates a new node
    return node;
  }
}

//Returns all attributes except the exclude attribute
vector<Attribute> exclude(vector<Attribute> attributes, Attribute exclude) {
  vector<Attribute> new_attributes;
  for(Attribute attr : attributes)
    if(attr != exclude)
      new_attributes.push_back(attr);
  return new_attributes;
}

//Returns the dataexamples extracted from examples with attribute value equal to the
//parameter value
vector<DataItem> get_exs(vector<DataItem>& examples, Attribute& attr, string value) {
  vector<DataItem> exs;
  for(DataItem ex : examples){
      if(ex.get_value(attr.get_nbr()).compare(value) == 0)
        exs.push_back(ex);
  }
  return exs;
}

//Returns the best attribute from attributes selected by highest gain
Attribute importance(vector<Attribute>& attributes, vector<DataItem>& examples) {
  //cout << "*******************************" << endl;
  Attribute best_attr = attributes[0];
  double best_value = compute_gain(best_attr, examples);
  for(decltype(attributes.size()) i = 1; i != attributes.size(); ++i) {
    double attr_value = compute_gain(attributes[i], examples);
    if(attr_value > best_value) {
      best_attr = attributes[i];
      best_value = attr_value;
    }
  }
  return best_attr;
}

//Computes the information gain for attr according to the formula
//in section 18.3.4
double compute_gain(Attribute& attr, vector<DataItem>& examples) {
  return entropy(examples) - remainder(attr, examples);
}

//Computes nbr positive examples in examples.
//Btw nice return type going on over here!
auto count_positive(std::vector<DataItem>& examples) -> decltype(examples.size()) {
  int count = 0;
  for(DataItem di : examples)
    if(di.is_positive())
      count++;
  return count;
}

//log2(x) should be log2(x) = 0 for x = 0
double log2_fix(double x) {
  if(x == 0)
    return 0;
  return log2(x);
}

//Computes the entropy for examples according to section 18.3.4
double entropy(vector<DataItem>& examples) {
  double p = count_positive(examples);
  double q = p/examples.size();
  return - (q*log2_fix(q) + (1-q)*log2_fix(1-q));
}

//Computes the remainder for attribute attr and examples
//according to section 18.3.4
double remainder(Attribute& attr, vector<DataItem>& examples) {
  double rem = 0;
  for(string value : attr.get_values()) {
    vector<DataItem> exs = get_exs(examples, attr, value);
    if(exs.size() == 0)
      continue;
    double pk = count_positive(exs);
    double nk = exs.size() - pk;
    rem += ((pk+nk)/examples.size()) * entropy(exs);
  }
  return rem;
}

//Returns true if all examples in examples has the same classifier
bool same_classifier(std::vector<DataItem>& examples) {
  bool first_type = examples[0].is_positive();
  for(decltype(examples.size()) i = 1; i != examples.size(); ++i)
    if(examples[i].is_positive() != first_type)
      return false;
  return true;
}
