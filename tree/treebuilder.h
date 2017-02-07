#ifndef TREEBUILDER_H
#define TREEBUILDER_H
#include "attribute.h"
#include "dataitem.h"
#include "node.h"
#include <vector>
#include <string>

Node build_tree(std::vector<Attribute>& attributes, std::vector<DataItem>& examples);
Node build_tree(std::vector<Attribute>& attributes, std::vector<DataItem>& examples, std::vector<DataItem>& parent_examples);
Attribute importance(std::vector<Attribute>& attributes, std::vector<DataItem>& examples);
std::vector<DataItem> get_exs(std::vector<DataItem>& examples, Attribute& attr, std::string value);
bool same_classifier(std::vector<DataItem>& examples);
std::vector<Attribute> exclude(std::vector<Attribute> attributes, Attribute exclude);
#endif
