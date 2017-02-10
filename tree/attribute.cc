#include "attribute.h"
#include <string>
#include <iostream>

using namespace std;

Attribute::Attribute() {}

//Creates an attribute with the given parameters.
Attribute::Attribute(int number, string n, vector<string>& v) : nbr(number), name(n), values(v) {}

//Returns the number describing the ordering
int Attribute::get_nbr() const {
  return nbr;
}

//Returns the possible attribute values
vector<string> Attribute::get_values() {
  return values;
}

//returns name
string Attribute::get_name() const {
  return name;
}

//Overloading the << operator
ostream& operator<<(ostream& os, const Attribute& attr) {
	os << "@attribute " << attr.name << " {";
	for(string v : attr.values)
		os << v << ", ";
	os << "}" << endl;
	return os;
}

//Overloaded == for attribute, comparing the names of the attributes
bool operator==(const Attribute& first, const Attribute& second) {
  return first.name.compare(second.name) == 0;
}

//Overloaded != for attribute, comparing the names of the attributes
bool operator!=(const Attribute& first, const Attribute& second) {
  return !(first == second);
}
