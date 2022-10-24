#include "OKExFeedFileReader.h"

OKExFeedFileReader* feedReader = OKExFeedFileReader::getInstance();

OKExFeedFileReader::OKExFeedFileReader()
{
	time = 0;
	insList = nullptr;
	feedCount = 0;
	lastFeedCount = 0;
}
OKExFeedFileReader::~OKExFeedFileReader()
{

}

void OKExFeedFileReader::initialize(void)
{
	time = 0;
	insList = nullptr;
	feedCount = 0;
	lastFeedCount = 0;
}

std::map<std::string, OKExInstrument*>* OKExFeedFileReader::initializeInsList(std::string masterfile)
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
	return insList;
}
void OKExFeedFileReader::readFeedFile(std::string feedFile)
{
	logWriter->addLog(Enums::logType::_INFO, "Start Reading Feed File.");
	logWriter->addLog(Enums::logType::_INFO, "File Name:" + feedFile);

	std::ifstream ifs(feedFile, std::ios::in | std::ios::binary);
	boost::iostreams::filtering_istream binaryFile;
	binaryFile.push(boost::iostreams::gzip_decompressor());
	binaryFile.push(ifs);

	//Memo: In case the file is written with binary data.
	//std::stream ifs(feedFile, std::ios::binary);
	//ifs.seekg(0,std::ios::end);
	//long long fsize = ifs.tellg();
	//if(fsize < 0)
	//{
	//    File Not Found
	//    return;
	//}
	//unsigned int msgsize = 0;
	//static const int BUF_SIZE = 1024;
	//char msg[BUF_SIZE];
	//unsigned char chsize[2];
	//int inc = 0;
	//long long pos = 0;
	//while(pos < fsize)
	// {
	//		inc = sizeof(chsize);
	//		ifs.read((char*)&msgsize,inc);
	//		pos += inc;
	//		if(msgsize > BUF_SIZE)
	//		{
	//			//Error Handling
	//		}
	//		memset(msg,0,BUF_SIZE);
	//		ifs.read(msg,msgsize);
	//		pos += msgsize;
	//		//Do same thing.
	// }
	std::string line;
	OKExMktMsg* msg = new OKExMktMsg();
	std::map<std::string, OKExInstrument*>::iterator ins;
	std::map<std::string, OKExInstrument*>::iterator insend = insList->end();
	bool blOptimize;

	while (std::getline(binaryFile, line))
	{
		OKExParser::parsePushData(line, msg);
		if (msg->blHasData)
		{
			ins = insList->find(msg->args["instId"]);
			if (ins != insend)
			{
				blOptimize = ins->second->reflectMsg(msg);
				if (blOptimize)
				{
					//Optimize
					blOptimize = false;
				}
			}
		}
		msg->init();
	}
	
}