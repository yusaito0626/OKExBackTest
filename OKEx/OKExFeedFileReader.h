#pragma once

#include "includes.h"
#include "OKExInstrument.h"

class OKExFeedFileReader
{
public:
	std::map<std::string, OKExInstrument*>* insList;

	OKExFeedFileReader();
	~OKExFeedFileReader();
	void initializeInsList(std::string masterfile);
	void readFeedFile(std::string feedFile);
};

