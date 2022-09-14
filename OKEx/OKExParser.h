#pragma once
#include "includes.h"
#include "OKExMessage.h"
#include "../utils/json.h"

class OKExParser
{

public:
	void parseOP(void);
	void parseEvent(void);
	void parsePushData(std::string str, OKExMktMsg* msg);
	void parse(void);
};

