#include "attribute.h"
#include "dataitem.h"
#include <vector>
#include "treebuilder.h"
#include "node.h"
#include <string>
#include <cmath>

using namespace std;

Node build_tree(vector<Attribute>& attributes, vector<DataItem>& examples) {
  return build_tree(attributes, examples, examples);
}

Node build_tree(vector<Attribute>& attributes, vector<DataItem>& examples, vector<DataItem>& parent_examples) {
  if(examples.size() < 1) {
    Node leaf_node(true, parent_examples);
    return leaf_node;
  } else if(same_classifier(examples) || attributes.size() < 1) {
    Node leaf_node(true, examples);
    return leaf_node;
  } else {
    Attribute attr = importance(attributes, examples);

    vector<pair<string, Node>> edges;
    for(string v : attr.get_values()) {
      vector<DataItem> exs = get_exs(examples, attr, v);
      vector<Attribute> attr_excluded = exclude(attributes, attr);
      Node child = build_tree(attr_excluded, exs, examples);
      edges.push_back(make_pair(v, child));
    }
    Node node(examples, edges, attr);
    return node;
  }
}

vector<Attribute> exclude(vector<Attribute> attributes, Attribute exclude) {
  vector<Attribute> new_attributes;
  for(Attribute attr : attributes)
    if(attr.get_name().compare(exclude.get_name()) != 0)
      new_attributes.push_back(attr);
  return new_attributes;
}

vector<DataItem> get_exs(vector<DataItem>& examples, Attribute& attr, string value) {
  vector<DataItem> exs;
  for(DataItem ex : examples){
      if(ex.get_value(attr.get_nbr()).compare(value) == 0)
        exs.push_back(ex);
  }
  return exs;
}

Attribute importance(vector<Attribute>& attributes, vector<DataItem>& examples) {
  //cout << "*******************************" << endl;
  Attribute best_attr = attributes[0];
  double best_value = compute_gain(best_attr, examples);
  //cout << best_attr.get_name() << " has gain " << to_string(best_value) << endl;
  for(decltype(attributes.size()) i = 1; i != attributes.size(); ++i) {
    double attr_value = compute_gain(attributes[i], examples);
    //cout << attributes[i].get_name() << " has gain " << to_string(attr_value) << endl;
    if(attr_value > best_value) {
      best_attr = attributes[i];
      best_value = attr_value;
    }
  }
  return best_attr;
}

double compute_gain(Attribute& attr, vector<DataItem>& examples) {
  //if(attr.get_name().compare("Patrons") == 0)
  //  cout << "Gain Patrons: "<< to_string(entropy(examples)) << endl;
  return entropy(examples) - remainder(attr, examples);
}

int count_positive(std::vector<DataItem>& examples) {
  int count = 0;
  for(DataItem di : examples)
    if(di.is_positive())
      count++;
  return count;
}

double log2_fix(double x) {
  if(x == 0)
    return 0;
  return log2(x);
}

double entropy(vector<DataItem>& examples) {
  double p = count_positive(examples);
  double q = p/examples.size();
  return - (q*log2_fix(q) + (1-q)*log2_fix(1-q));
}

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

bool same_classifier(std::vector<DataItem>& examples) {
  bool first_type = examples[0].is_positive();
  for(decltype(examples.size()) i = 1; i != examples.size(); ++i)
    if(examples[i].is_positive() != first_type)
      return false;
  return true;
}
