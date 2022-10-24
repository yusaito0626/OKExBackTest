#pragma once

#include "includes.h"
#include "OKExInstrument.h"
#include "OKExParser.h"
#include "OKExMessage.h"
#include "OKExOptimizer.h"
#include "../utils/json.h"
#include "../utils/Logging.h"

class OKExFeedFileReader
{
	OKExFeedFileReader();
	OKExFeedFileReader(const OKExFeedFileReader&) {};
	OKExFeedFileReader& operator=(const OKExFeedFileReader&) {};
public:
	long long time;
	std::map<std::string, OKExInstrument*>* insList;
	int feedCount;
	int lastFeedCount;

	~OKExFeedFileReader();
	void initialize(void);
	std::map<std::string, OKExInstrument*>* initializeInsList(std::string masterfile);
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