#pragma once
#include <string>
#include <map>
#include <list>
#include <stack>
namespace json
{
	bool deserialize(std::string str, std::map<std::string, std::string>& mp);
	bool serialize(std::map<std::string, std::string> mp, std::string& str);
	bool deserialize(std::string str, std::list<std::string>& lst);
	bool serialize(std::list<std::string> lst, std::string& str);
	size_t findEndOfBrackets(std::string str);
	size_t findEndOfBrackets(std::string str,size_t off);
};

