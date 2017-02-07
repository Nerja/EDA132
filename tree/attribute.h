#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <string>
#include <vector>
#include <iostream>

class Attribute {
	public:
		Attribute(std::string name, std::vector<std::string>& attributes);
		friend std::ostream& operator<<(std::ostream& os, const Attribute& attr);
	private:
		std::string name;
		std::vector<std::string> values;
};
#endif
