#include <iostream>
#include "parser.h"
#include "dataitem.h"
#include "attribute.h"
#include "treebuilder.h"
#include "node.h"
#include <vector>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]) {
	if(argc != 3) {
			cerr << "Plz enter filename and pruning threshold" << endl;
			exit(1);
	}
	string filename(argv[1]);
	double threshold = atof(argv[2]);

	vector<Attribute> attributes;
	vector<DataItem> examples;

	//Parsing attributes and examples from the given datafile into the two
	//attribute/data item vectors
	parse(filename, attributes, examples);
	//Builds the tree with ID3 algorithm, returns the root node
	Node n = build_tree(attributes, examples);
	cout << n;
	cout << "***********PRUNING**********" << endl;
	//Prunes the tree with the chi squared method using the given threshold
	n.prune(threshold);
	cout << n;


}
