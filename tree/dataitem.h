#ifndef DATAITEM_H
#define DATAITEM_H
#include <string>
#include <vector>
#include <iostream>

class DataItem {
	public:
		DataItem(std::vector<std::string> values);
		friend std::ostream& operator<<(std::ostream& os, const DataItem& dataitem);
	private:
		std::vector<std::string> values;
		bool positive_class;

};

#endif
