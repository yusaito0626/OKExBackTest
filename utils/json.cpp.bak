#include "json.h"

bool json::deserialize(std::string str, std::map<std::string, std::string>& mp)
{
	size_t start = str.find("{");
	size_t end = str.find("}");
	size_t current = 0;
	bool lastElement = false;

	if (start != std::string::npos && end != std::string::npos)
	{
		//std::string rest = str.substr(start + 1);
		current = start + 1;
		std::string key;
		std::string val;
		size_t comma = str.find(",",current);
		size_t bracket = str.find("[", current);
		size_t curly = str.find("{", current);
		size_t colon = std::string::npos;
		size_t valend = std::string::npos;
		while (comma != std::string::npos)
		{
			if (comma < bracket && comma < curly)//the element is neither list nor map
			{
				colon = str.find(":",current);
				key = str.substr(current, colon - current);
				if (key.find(" ") == 0)
				{
					key = key.substr(1, key.size() - 1);
				}
				if (key.find("\"") == 0)
				{
					key = key.substr(1, key.size() - 2);
				}
				val = str.substr(colon + 1, comma - colon - 1);
				if (val.find(" ") == 0)
				{
					val = val.substr(1, val.size() - 1);
				}
				if (val.find("\"") == 0)
				{
					val = val.substr(1, val.size() - 2);
				}
				mp[key] = val;
				current = comma + 1;
				comma = str.find(",",current);
				if (comma == std::string::npos)
				{
					lastElement = true;
				}
			}
			else if(bracket < curly)//the element is list
			{
				colon = str.find(":", current);
				key = str.substr(current, colon - current);
				if (key.find(" ") == 0)
				{
					key = key.substr(1, key.size() - 1);
				}
				if (key.find("\"") == 0)
				{
					key = key.substr(1, key.size() - 2);
				}
				valend = findEndOfBrackets(str, colon + 1);
				if (valend != std::string::npos)
				{
					val = str.substr(colon + 1, valend - colon);
					mp[key] = val;
					current = valend + 2;
					comma = str.find(",", current);
					bracket = str.find("[", current);
				}
				else
				{
					mp.clear();
					return false;
				}
			}
			else
			{
				colon = str.find(":", current);
				key = str.substr(current, colon - current);
				if (key.find(" ") == 0)
				{
					key = key.substr(1, key.size() - 1);
				}
				if (key.find("\"") == 0)
				{
					key = key.substr(1, key.size() - 2);
				}
				valend = findEndOfBrackets(str, colon + 1);
				if (valend != std::string::npos)
				{
					val = str.substr(colon + 1, valend - colon);
					mp[key] = val;
					current = valend + 2;
					comma = str.find(",", current);
					curly = str.find("{", current);
				}
				else
				{
					mp.clear();
					return false;
				}
			}
		}
		if (lastElement)
		{
			if (bracket != std::string::npos || curly != std::string::npos)//the last element is list or map
			{
				colon = str.find(":", current);
				key = str.substr(current, colon - current);
				if (key.find(" ") == 0)
				{
					key = key.substr(1, key.size() - 1);
				}
				if (key.find("\"") == 0)
				{
					key = key.substr(1, key.size() - 2);
				}
				valend = findEndOfBrackets(str, colon + 1);
				if (valend != std::string::npos)
				{
					val = str.substr(colon + 1, valend - colon);
					mp[key] = val;
				}
				else
				{
					mp.clear();
					return false;
				}
			}
			else
			{
				colon = str.find(":", current);
				key = str.substr(current, colon - current);
				if (key.find(" ") == 0)
				{
					key = key.substr(1, key.size() - 1);
				}
				if (key.find("\"") == 0)
				{
					key = key.substr(1, key.size() - 2);
				}
				val = str.substr(colon + 1, comma - colon - 1);
				val = val.substr(0, val.size() - 1);//Exclude the last "}"
				if (val.find(" ") == 0)
				{
					val = val.substr(1, val.size() - 1);
				}
				if (val.find("\"") == 0)
				{
					val = val.substr(1, val.size() - 2);
				}
				mp[key] = val;
				//key = str.substr(current + 1, colon - 1);
				//val = str.substr(colon + 1, comma - colon - 1);
				//if (val.find("\"") == 0)
				//{
				//	val = val.substr(1, val.size() - 2);
				//}
				//mp[key] = val;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool json::serialize(std::map<std::string, std::string> mp, std::string& str)
{
	std::map<std::string, std::string>::iterator it;
	std::map<std::string, std::string>::iterator itend = mp.end();
	str = "{";
	for (it = mp.begin(); it != itend; ++it)
	{
		str += "\"" + it->first + "\":\"" + it->second + "\",";
	}
	str = str.substr(0, str.size() - 1) + "}";
	return true;
}

bool json::deserialize(std::string str, std::list<std::string>& lst)
{
	size_t start = str.find("[");
	size_t end = str.find("]");
	size_t current = 0;
	bool lastElement = false;
	if (start != std::string::npos && end != std::string::npos)
	{
		//std::string rest = str.substr(start + 1);
		current = start + 1;
		std::string val;
		size_t comma = str.find(",", current);
		size_t bracket = str.find("[", current);
		size_t curly = str.find("{", current);
		size_t valend = std::string::npos;
		while (comma != std::string::npos)
		{
			if (comma < bracket && comma < curly)//the element is neither list nor map
			{
				val = str.substr(current, comma - current);
				if (val.find(" ") == 0)
				{
					val = val.substr(1, val.size() - 1);
				}
				if (val.find("\"") == 0)
				{
					val = val.substr(1, val.size() - 2);
				}
				lst.push_back(val);
				current = comma + 1;
				comma = str.find(",", current);
				if (comma == std::string::npos)
				{
					lastElement = true;
				}
			}
			else if (bracket < curly)//the element is list
			{
				valend = findEndOfBrackets(str, current);
				if (valend != std::string::npos)
				{
					val = str.substr(current, valend - current + 1);
					lst.push_back(val);
					current = valend + 2;
					comma = str.find(",", current);
					bracket = str.find("[", current);
				}
				else
				{
					lst.clear();
					return false;
				}
			}
			else
			{
				valend = findEndOfBrackets(str, current);
				if (valend != std::string::npos)
				{
					val = str.substr(current, valend - current + 1);
					lst.push_back(val);
					current = valend + 2;
					comma = str.find(",", current);
					curly = str.find("{", current);
				}
				else
				{
					lst.clear();
					return false;
				}
			}
		}
		if (lastElement)
		{
			if (bracket != std::string::npos || curly != std::string::npos)//the last element is list or map
			{
				valend = findEndOfBrackets(str, current);
				if (valend != std::string::npos)
				{
					val = str.substr(current, valend - current + 1);
					lst.push_back(val);
				}
				else
				{
					lst.clear();
					return false;
				}
			}
			else
			{
				val = str.substr(current, comma - current);
				val = val.substr(0, val.size() - 1);//Exclude the last "]"
				if (val.find(" ") == 0)
				{
					val = val.substr(1, val.size() - 1);
				}
				if (val.find("\"") == 0)
				{
					val = val.substr(1, val.size() - 2);
				}
				lst.push_back(val);
			}
		}
		
		return true;
	}
	else
	{
		return false;
	}
}
bool json::serialize(std::list<std::string> lst, std::string& str)
{
	std::list<std::string>::iterator it;
	std::list<std::string>::iterator itend = lst.end();
	str = "[";
	for (it = lst.begin(); it != itend; ++it)
	{
		str += "\"" + *it + "\",";
	}
	str = str.substr(0, str.size() - 1) + "]";
	return true;
}

size_t json::findEndOfBrackets(std::string str)
{
	size_t bracket_start = str.find("[");
	size_t curly_start = str.find("{");
	size_t bracket_end = std::string::npos;
	size_t curly_end = std::string::npos;
	size_t current = 0;
	int count = 0;
	std::stack<char> bracket_list;
	char last_bracket;
	if (bracket_start == std::string::npos && curly_start == std::string::npos)
	{
		return std::string::npos;
	}
	else if(bracket_start < curly_start)
	{
		++count;
		last_bracket = '[';
		current = bracket_start + 1;
	}
	else if (bracket_start > curly_start)
	{
		++count;
		last_bracket = '{';
		current = curly_start + 1;
	}
	else
	{
		return std::string::npos;
	}

	while (current <= str.size())
	{
		bracket_start = str.find("[",current);
		curly_start = str.find("{", current);
		bracket_end = str.find("]", current);
		curly_end = str.find("}", current);
		switch (last_bracket)
		{
		case '[':
			if (bracket_end < bracket_start && bracket_end < curly_start)
			{
				if (bracket_end > curly_end)
				{
					return std::string::npos;
				}
				current = bracket_end + 1;
				--count;
				if (bracket_list.empty())
				{
					return bracket_end;
				}
				else
				{
					last_bracket = bracket_list.top();
					bracket_list.pop();
				}
			}
			else if (bracket_start < curly_start)
			{
				current = bracket_start + 1;
				++count;
				bracket_list.push(last_bracket);
				last_bracket = '[';
			}
			else if (bracket_start > curly_start)
			{
				current = curly_start + 1;
				++count;
				bracket_list.push(last_bracket);
				last_bracket = '{';
			}
			break;
		case '{':
			if (curly_end < bracket_start && curly_end < curly_start)
			{
				if (curly_end > bracket_end)
				{
					return std::string::npos;
				}
				current = curly_end + 1;
				--count;
				if (bracket_list.empty())
				{
					return curly_end;
				}
				else
				{
					last_bracket = bracket_list.top();
					bracket_list.pop();
				}
			}
			else if (bracket_start < curly_start)
			{
				current = bracket_start + 1;
				++count;
				bracket_list.push(last_bracket);
				last_bracket = '[';
			}
			else if (bracket_start > curly_start)
			{
				current = curly_start + 1;
				++count;
				bracket_list.push(last_bracket);
				last_bracket = '{';
			}
			break;
		default:
			return std::string::npos;
			break;
		}
	}
	if (count == 0)
	{
		return current - 1;
	}
	else
	{
		return std::string::npos;
	}
}

size_t json::findEndOfBrackets(std::string str, size_t off)
{
	size_t bracket_start = str.find("[", off);
	size_t curly_start = str.find("{", off);
	size_t bracket_end = std::string::npos;
	size_t curly_end = std::string::npos;
	size_t current = 0;
	int count = 0;
	std::stack<char> bracket_list;
	char last_bracket;
	if (bracket_start == std::string::npos && curly_start == std::string::npos)
	{
		return std::string::npos;
	}
	else if (bracket_start < curly_start)
	{
		++count;
		last_bracket = '[';
		current = bracket_start + 1;
	}
	else if (bracket_start > curly_start)
	{
		++count;
		last_bracket = '{';
		current = curly_start + 1;
	}
	else
	{
		return std::string::npos;
	}

	while (current <= str.size())
	{
		bracket_start = str.find("[", current);
		curly_start = str.find("{", current);
		bracket_end = str.find("]", current);
		curly_end = str.find("}", current);
		switch (last_bracket)
		{
		case '[':
			if (bracket_end < bracket_start && bracket_end < curly_start)
			{
				if (bracket_end > curly_end)
				{
					return std::string::npos;
				}
				current = bracket_end + 1;
				--count;
				if (bracket_list.empty())
				{
					return bracket_end;
				}
				else
				{
					last_bracket = bracket_list.top();
					bracket_list.pop();
				}
			}
			else if (bracket_start < curly_start)
			{
				current = bracket_start + 1;
				++count;
				bracket_list.push(last_bracket);
				last_bracket = '[';
			}
			else if (bracket_start > curly_start)
			{
				current = curly_start + 1;
				++count;
				bracket_list.push(last_bracket);
				last_bracket = '{';
			}
			break;
		case '{':
			if (curly_end < bracket_start && curly_end < curly_start)
			{
				if (curly_end > bracket_end)
				{
					return std::string::npos;
				}
				current = curly_end + 1;
				--count;
				if (bracket_list.empty())
				{
					return curly_end;
				}
				else
				{
					last_bracket = bracket_list.top();
					bracket_list.pop();
				}
			}
			else if (bracket_start < curly_start)
			{
				current = bracket_start + 1;
				++count;
				bracket_list.push(last_bracket);
				last_bracket = '[';
			}
			else if (bracket_start > curly_start)
			{
				current = curly_start + 1;
				++count;
				bracket_list.push(last_bracket);
				last_bracket = '{';
			}
			break;
		default:
			return std::string::npos;
			break;
		}
	}
	if (count == 0)
	{
		return current - 1;
	}
	else
	{
		return std::string::npos;
	}
}