#include "dataitem.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

DataItem::DataItem(vector<string> v) : values(v) {
  string last_v = v[v.size() - 1];
  positive_class = last_v.find("yes") != string::npos;
}

ostream& operator<<(ostream& os, const DataItem& dataitem) {
		for(string v : dataitem.values)
		os << v << ", ";
	os << endl;
	return os;
}
