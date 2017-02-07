#include "attribute.h"
#include "dataitem.h"
#include <vector>
#include "treebuilder.h"
#include "node.h"

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

    

    Node leaf_node(true, examples);
    return leaf_node;
  }
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
