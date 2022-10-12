#include "OKExFeedFileReader.h"

OKExFeedFileReader* feedReader = OKExFeedFileReader::getInstance();

OKExFeedFileReader::OKExFeedFileReader()
{

}
OKExFeedFileReader::~OKExFeedFileReader()
{

}
void OKExFeedFileReader::initializeInsList(std::string masterfile)
{
	std::ifstream fs(masterfile);
	std::string line;
	if (fs)
	{
		if (!insList)
		{
			insList = new std::map<std::string, OKExInstrument*>();
			while (std::getline(fs, line))
			{
				std::map<std::string, std::string> mp;
				OKExInstrument* ins = new OKExInstrument();
				json::deserialize(line, mp);
				ins->setInstrumentData(mp);
				insList->emplace(ins->instId, ins);
			}
		}
		else
		{
			while (std::getline(fs, line))
			{
				std::map<std::string, std::string> mp;
				json::deserialize(line, mp);
				OKExInstrument* ins;
				if (insList->find(mp["instId"]) != insList->end())
				{
					ins = insList->at(mp["instId"]);
					ins->setInstrumentData(mp);
				}
				else
				{
					ins = new OKExInstrument();
					ins->setInstrumentData(mp);
					insList->emplace(ins->instId, ins);
				}
			}
		}
	}
}
void OKExFeedFileReader::readFeedFile(std::string feedFile)
{

}