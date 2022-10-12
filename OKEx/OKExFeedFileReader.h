#pragma once

#include "includes.h"
#include "OKExInstrument.h"
#include "../utils/json.h"

class OKExFeedFileReader
{
	OKExFeedFileReader();
	OKExFeedFileReader(const OKExFeedFileReader&) {};
	OKExFeedFileReader& operator=(const OKExFeedFileReader&) {};
public:
	std::map<std::string, OKExInstrument*>* insList;

	~OKExFeedFileReader();
	void initializeInsList(std::string masterfile);
	void readFeedFile(std::string feedFile);

	static OKExFeedFileReader* getInstance(void)
	{
		static OKExFeedFileReader* singleton;
		if (!singleton)
		{
			singleton = new OKExFeedFileReader();
		}
		return singleton;
	}
};

extern OKExFeedFileReader* feedReader;