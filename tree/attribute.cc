#include "attribute.h"
#include <string>
#include <iostream>

using namespace std;

Attribute::Attribute() {
	
}

Attribute::Attribute(string n, vector<string>& v) : name(n), values(v) {

}

ostream& operator<<(ostream& os, const Attribute& attr) {
	os << "@attribute " << attr.name << " {";
	for(string v : attr.values)
		os << v << ", ";
	os << "}" << endl;
	return os;
}
