#include "parser.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

void parse(string file, vector<Attribute>& attributes, vector<DataItem>& examples) {
	ifstream input(file);
	if(!input) {
		cerr << "FILE NOT FOUND" << endl;
		exit(1);
	}
	string line;
	if(!getline(input, line) || line.find("@relation") == string::npos) {
		cerr << "Expected @relation" << endl;
		exit(1);
	}
	while(getline(input, line) && line.find("@attribute") == string::npos);

	do {
		parse_attribute(line, attributes);
	} while(getline(input, line) && line.find("@attribute") != string::npos);

	while(getline(input, line) && line.find("@data") == string::npos);

	while(getline(input, line)) {
		parse_dataitem(line, examples);
	}

	input.close();
}

void parse_attribute(string line, vector<Attribute>& attributes) {
	replace(line.begin(), line.end(), '{', ' ');
	replace(line.begin(), line.end(), '}', ' ');
	replace(line.begin(), line.end(), ',', ' ');
	istringstream iss(line);
	string name;
  iss >> name >> name; // Reads name and skip the begining @attribute
	vector<string> values;
	string value;

	while(iss >> value)
		values.push_back(value);

	Attribute attr(name, values);
	attributes.push_back(attr);

	cout << attr;
}

void parse_dataitem(string line, vector<DataItem>& examples){
	replace(line.begin(), line.end(), ',', ' ');

	istringstream iss(line);

	vector<string> values;
	string value;

	while(iss >> value)
		values.push_back(value);

	DataItem dataitem(values);
	examples.push_back(dataitem);

	cout << dataitem;
}
