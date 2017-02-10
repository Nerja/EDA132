#include "dataitem.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

//Creates a data item with the given attribute values
//Decides if positive or not by checking the last value
DataItem::DataItem(vector<string> v) : values(v) {
  string last_v = v[v.size() - 1];
  positive_class = last_v.find("yes") != string::npos;
}

//Returns true if positive class
bool DataItem::is_positive() const {
  return positive_class;
}

vector<string> DataItem::get_values() {
  return values;
}

//Returns the attribute value for the i:th attribute
string DataItem::get_value(int i) {
  return values[i];
}

//Overloaded << operator for output
ostream& operator<<(ostream& os, const DataItem& dataitem) {
		for(string v : dataitem.values)
		os << v << ", ";
	os << endl;
	return os;
}
