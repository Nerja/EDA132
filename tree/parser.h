#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include "attribute.h"
#include "dataitem.h"
void parse(std::string file, std::vector<Attribute>& attributes, std::vector<DataItem>& examples);
void parse_attribute(std::string line, std::vector<Attribute>& attributes);
void parse_dataitem(std::string line, std::vector<DataItem>& examples); 

#endif
