#include "attribute.h"
#include "dataitem.h"
#include <vector>
#include "treebuilder.h"
#include "node.h"
#include <string>

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
    }

    Node leaf_node(true, examples);
    return leaf_node;
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

Attribute importance(std::vector<Attribute>& attributes, std::vector<DataItem>& examples) {
  return attributes[0];
}

bool same_classifier(std::vector<DataItem>& examples) {
  bool first_type = examples[0].is_positive();
  for(decltype(examples.size()) i = 1; i != examples.size(); ++i)
    if(examples[i].is_positive() != first_type)
      return false;
  return true;
}
