#include "OKExFeedFileReader.h"

OKExFeedFileReader* feedReader = OKExFeedFileReader::getInstance();

OKExFeedFileReader::OKExFeedFileReader()
{
	ts = 0;
	insList = nullptr;
	feedCount = 0;
	lastFeedCount = 0;
}
OKExFeedFileReader::~OKExFeedFileReader()
{

}

void OKExFeedFileReader::initialize(std::string outputFilePath)
{
	ts = 0;
	insList = nullptr;
	feedCount = 0;
	lastFeedCount = 0;
	outputFile = std::ofstream(outputFilePath + "\\OKExSimSummary_" + GlobalVariables::OKEx::suffix + ".csv");
	outputFile << "date,instId,open,high,low,last,sell trade qty,sell avg pr,buy trade qty,buy avg pr,pos PL,trade PL,total PL" << std::endl;
	outputFile.flush();
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
			std::map<std::string, OKExInstrument*> tempInsList;
			std::map<std::string, OKExInstrument*>::iterator it;
			std::map<std::string, OKExInstrument*>::iterator itend = insList->end();
			for (it = insList->begin(); it != itend; ++it)
			{
				tempInsList.emplace(it->first, it->second);
			}
			insList->clear();
			while (std::getline(fs, line))
			{
				std::map<std::string, std::string> mp;
				json::deserialize(line, mp);
				OKExInstrument* ins;
				if (tempInsList.find(mp["instId"]) != tempInsList.end())
				{
					ins = tempInsList.at(mp["instId"]);
					ins->setInstrumentData(mp);
					insList->emplace(ins->instId, ins);
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

void OKExFeedFileReader::readParamFile(std::string paramfile)
{
	std::ifstream fs(paramfile);
	std::string line;
	if (fs)
	{
		if (insList)
		{
			while (std::getline(fs, line))
			{
				std::map<std::string, std::string> mp;
				json::deserialize(line, mp);
				OKExInstrument* ins = findInsByAlias(mp["alias"],mp["uly"]);
				if (ins)
				{
					ins->setParams(mp);
				}
			}
		}
	}

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
			ins = insList->find(msg->args->at("instId"));
			blOptimize = reflectOneMsg(msg);
			if (blOptimize)
			{
				//Optimize
				optimizer->updateRings(ins->second);
				optimizer->calcFactors(ins->second);
				optimizer->optimize(ins->second);
				blOptimize = false;
			}
			++feedCount;
		}
		msg->init();
	}
	binaryFile.pop();
	binaryFile.pop();
}

bool OKExFeedFileReader::reflectOneMsg(OKExMktMsg* msg)
{
	std::map<std::string, OKExInstrument*>::iterator ins;
	std::map<std::string, OKExInstrument*>::iterator insend = insList->end();
	ins = insList->find(msg->args->at("instId"));
	bool blOptimize = false;
	if (ins != insend)
	{
		if (msg->args->at("channel") == "books")
		{
			if (msg->args->at("action") == "snapshot")
			{
				ins->second->initializeBooks(msg, 1000);
				ins->second->calcMid();
			}
			else if (msg->args->at("action") == "update")
			{
				blOptimize = ins->second->updateBooks(msg);
				ins->second->calcMid();
				ts = ins->second->ts;
				voms->checkWaitingOrdQueue(ts);
			}
		}
		else if (msg->args->at("channel") == "trades")
		{
			ins->second->updateTrade(msg);
			ins->second->calcMid();
			ts = ins->second->ts;
			voms->checkWaitingOrdQueue(ts);
			blOptimize = true;
		}
	}
	return blOptimize;
}

OKExInstrument* OKExFeedFileReader::findInsByAlias(std::string alias, std::string uly)
{
	if (insList)
	{
		std::map<std::string, OKExInstrument*>::iterator it;
		std::map<std::string, OKExInstrument*>::iterator itend = insList->end();
		if (uly == "")//SPOT
		{
			if (insList->find(alias) != itend)
			{
				return insList->at(alias);
			}
			else
			{
				return nullptr;
			}
		}
		else if(alias == "this_week" || alias == "next_week" || alias == "quarter" || alias == "next_quarter")//Future
		{
			for (it = insList->begin(); it != itend; ++it)
			{
				if (it->second->uly == uly && it->second->alias == alias)
				{
					break;
				}
			}
			if (it != itend)
			{
				return it->second;
			}
			else
			{
				return nullptr;
			}
		}
		else//Perpetual
		{
			if (insList->find(alias) != itend)
			{
				return insList->at(alias);
			}
			else
			{
				return nullptr;
			}
		}
	}
	else
	{
		return nullptr;
	}
}

void OKExFeedFileReader::endOfDayReset(void)
{
	std::map<std::string, OKExInstrument*>::iterator it;
	std::map<std::string, OKExInstrument*>::iterator itend = insList->end();
	
	for (it = insList->begin(); it != itend; ++it)
	{
		outputFile << GlobalVariables::OKEx::today.strday << it->second->outputDailyResult() << std::endl;
		outputFile.flush();
		it->second->endOfDayReset();
	}

	ts = 0;
	feedCount = 0;
	lastFeedCount = 0;
}