#ifndef DATAITEM_H
#define DATAITEM_H
#include <string>
#include <vector>
#include <iostream>

class DataItem {
	public:
		DataItem(std::vector<std::string> values);
		friend std::ostream& operator<<(std::ostream& os, const DataItem& dataitem);
		bool is_positive() const;
		std::vector<std::string> get_values();
		std::string get_value(int nbr);
	private:
		std::vector<std::string> values;
		bool positive_class;

};

#endif
