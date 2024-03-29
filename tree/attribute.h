#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <string>
#include <vector>
#include <iostream>

class Attribute {
	public:
		Attribute();
		Attribute(int nbr, std::string name, std::vector<std::string>& attributes);
		friend std::ostream& operator<<(std::ostream& os, const Attribute& attr);
		friend bool operator==(const Attribute& first, const Attribute& second);
		std::vector<std::string> get_values();
		int get_nbr() const;
		std::string get_name() const;
	private:
		int nbr;
		std::string name;
		std::vector<std::string> values;
};
bool operator==(const Attribute& first, const Attribute& second);
bool operator!=(const Attribute& first, const Attribute& second);
#endif
