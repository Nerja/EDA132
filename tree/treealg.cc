#include <iostream>
#include "parser.h"
#include "dataitem.h"
#include "attribute.h"
#include "treebuilder.h"
#include "node.h"
#include <vector>


using namespace std;

int main() {
	string filename = "weather.arff";
	vector<Attribute> attributes;
	vector<DataItem> examples;

	parse(filename, attributes, examples);
	Node n = build_tree(attributes, examples);
	cout << n;


}