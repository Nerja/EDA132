#ifndef TREEBUILDER_H
#define TREEBUILDER_H
#include "attribute.h"
#include "dataitem.h"
#include "node.h"
#include <vector>
Node build_tree(std::vector<Attribute>& attributes, std::vector<DataItem>& examples);
Node build_tree(std::vector<Attribute>& attributes, std::vector<DataItem>& examples, std::vector<DataItem>& parent_examples);
Attribute importance(std::vector<Attribute>& attributes, std::vector<DataItem>& examples);
bool same_classifier(std::vector<DataItem>& examples);
#endif
