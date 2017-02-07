#include "attribute.h"
#include <string>
#include <iostream>

using namespace std;

Attribute::Attribute() {

}

Attribute::Attribute(int number, string n, vector<string>& v) : nbr(number), name(n), values(v) {

}

int Attribute::get_nbr() const {
  return nbr;
}

vector<string> Attribute::get_values() {
  return values;
}

ostream& operator<<(ostream& os, const Attribute& attr) {
	os << "@attribute " << attr.name << " {";
	for(string v : attr.values)
		os << v << ", ";
	os << "}" << endl;
	return os;
}
