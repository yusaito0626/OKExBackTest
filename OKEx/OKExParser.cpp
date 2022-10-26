#include "OKExParser.h"


void OKExParser::parsePushData(std::string str, OKExMktMsg* msg)
{
	std::map<std::string, std::string> js;
	std::map<std::string, std::string> args;
	std::list<std::string> datalst;
	std::map<std::string, std::string> data;
	std::list<std::string> asks;
	std::list<std::string> bids;
	std::list<std::string>::iterator it;
	std::list<std::string>::iterator itend;
	std::list<std::string>::iterator itask;
	std::list<std::string>::iterator itaskend;
	std::list<std::string>::iterator itbid;
	std::list<std::string>::iterator itbidend;
	std::map<std::string, std::string>::iterator argit;
	std::map<std::string, std::string>::iterator argitend;

	if (json::deserialize(str, js))
	{
		if (json::deserialize(js["arg"], args))
		{
			msg->blHasData = true;
			argitend = args.end();
			for (argit = args.begin(); argit != argitend; ++argit)
			{
				msg->args->emplace(argit->first, argit->second);
			}
			msg->args->emplace("action",js["action"]);
			if (args["channel"].substr(0, 5) == "books")
			{
				std::list<dataBooks*>::iterator booksit = msg->books->begin();
				std::list<dataBooks*>::iterator booksitend = msg->books->end();
				if (json::deserialize(js["data"], datalst))
				{
					itend = datalst.end();
					for (it = datalst.begin(); it != itend; ++it)
					{
						if (json::deserialize(*it, data))
						{
							dataBooks* dtbook = *booksit;
							dtbook->ts = stoll(data["ts"]);
							dtbook->checkSum = stoi(data["checksum"]);
							if (json::deserialize(data["asks"], asks))
							{
								itaskend = asks.end();
								std::list<msgbook*>::iterator askbookit = dtbook->asks->begin();
								std::list<msgbook*>::iterator askbookitend = dtbook->asks->end();
								for (itask = asks.begin(); itask != itaskend; ++itask)
								{
									std::list<std::string> bklst;
									if (json::deserialize(*itask, bklst))
									{
										msgbook* bk = *askbookit;
										std::list<std::string>::iterator elem = bklst.begin();
										bk->px = stod(*elem);
										elem = std::next(elem);
										bk->sz = stod(*elem);
										elem = std::next(elem);
										bk->liqOrd = stod(*elem);
										elem = std::next(elem);
										bk->numOfOrd = stoi(*elem);
										//dtbook.asks.push_back(bk);
										++askbookit;
										if (askbookit == askbookitend)
										{
											break;
										}
									}
								}
							}
							if (json::deserialize(data["bids"], bids))
							{
								itbidend = bids.end();
								std::list<msgbook*>::iterator bidbookit = dtbook->bids->begin();
								std::list<msgbook*>::iterator bidbookitend = dtbook->bids->end();
								for (itbid = bids.begin(); itbid != itbidend; ++itbid)
								{
									std::list<std::string> bklst;
									if (json::deserialize(*itbid, bklst))
									{
										msgbook* bk = *bidbookit;
										std::list<std::string>::iterator elem = bklst.begin();
										bk->px = stod(*elem);
										elem = std::next(elem);
										bk->sz = stod(*elem);
										elem = std::next(elem);
										bk->liqOrd = stod(*elem);
										elem = std::next(elem);
										bk->numOfOrd = stoi(*elem);
										//dtbook.bids.push_back(bk);
										++bidbookit;
										if (bidbookit == bidbookitend)
										{
											break;
										}
									}
								}
							}
							//msg->books.push_back(dtbook);
						}	
					}
				}
			}
			else if (args["channel"] == "trades")
			{
				if (json::deserialize(js["data"], datalst))
				{
					itend = datalst.end();
					std::list<dataTrades*>::iterator dttradeit = msg->trades->begin();
					std::list<dataTrades*>::iterator dttradeitend = msg->trades->end();
					for (it = datalst.begin(); it != itend; ++it)
					{
						if (json::deserialize(*it, data))
						{
							dataTrades* dttrade = *dttradeit;
							dttrade->ts = stoll(data["ts"]);
							dttrade->instId = data["instId"];
							dttrade->tradeId = data["tradeId"];
							dttrade->px = stod(data["px"]);
							dttrade->sz = stod(data["sz"]);
							if (data["side"] == "buy")
							{
								dttrade->side = OKExEnums::side::_BUY;
							}
							else if (data["side"] == "sell")
							{
								dttrade->side = OKExEnums::side::_SELL;
							}
							else
							{
								dttrade->side = OKExEnums::side::_NONE;
							}
							//msg->trades.push_back(dttrade);
							++dttradeit;
							if (dttradeit == dttradeitend)
							{
								break;
							}
						}
					}
				}
			}
		}
	}
}
