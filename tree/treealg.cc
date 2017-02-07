#include <iostream>
#include "parser.h"
#include "dataitem.h"
#include "attribute.h"
#include <vector>

using namespace std;

int main() {
	string filename = "weather.arff";
	vector<Attribute> attributes;
	vector<DataItem> data_items;

	parse(filename, attributes, data_items);

	

}
