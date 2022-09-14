#include "OKExParser.h"


void OKExParser::parsePushData(std::string str, OKExMktMsg* msg)
{
	std::map<std::string, std::string> js;
	std::map<std::string, std::string> args;
	std::map<std::string, std::string> data;
	std::list<std::string> asks;
	std::list<std::string> bids;
	std::map<std::string, std::string>::iterator it;
	std::map<std::string, std::string>::iterator itend;
	std::list<std::string>::iterator itask;
	std::list<std::string>::iterator itaskend;
	std::list<std::string>::iterator itbid;
	std::list<std::string>::iterator itbidend;

	if (json::deserialize(str, js))
	{
		if (json::deserialize(js["arg"], args))
		{
			msg->args = args;
			if (args["channel"].substr(0, 5) == "books")
			{
				if (json::deserialize(js["data"], data))
				{
					itend = data.end();
					for (it = data.begin(); it != itend; ++it)
					{
						dataBooks dtbook;
						dtbook.ts = stoll(data["ts"]);
						dtbook.checkSum = stoi(data["checksum"]);
						if (json::deserialize(data["asks"], asks))
						{
							itaskend = asks.end();
							for (itask = asks.begin(); itask != itaskend; ++itask)
							{
								std::list<std::string> bklst;
								if (json::deserialize(*itask, bklst))
								{
									book bk;
									std::list<std::string>::iterator elem = bklst.begin();
									bk.px = stod(*elem);
									std::next(elem);
									bk.sz = stod(*elem);
									std::next(elem);
									bk.liqOrd = stod(*elem);
									std::next(elem);
									bk.numOfOrd = stoi(*elem);
									dtbook.asks.push_back(bk);
								}
							}
						}
						if (json::deserialize(data["bids"], bids))
						{
							itbidend = bids.end();
							for (itbid = bids.begin(); itbid != itbidend; ++itbid)
							{
								std::list<std::string> bklst;
								if (json::deserialize(*itbid, bklst))
								{
									book bk;
									std::list<std::string>::iterator elem = bklst.begin();
									bk.px = stod(*elem);
									std::next(elem);
									bk.sz = stod(*elem);
									std::next(elem);
									bk.liqOrd = stod(*elem);
									std::next(elem);
									bk.numOfOrd = stoi(*elem);
									dtbook.bids.push_back(bk);
								}
							}
						}
						msg->books.push_back(dtbook);
					}
				}
			}
			else if (args["channel"] == "trades")
			{
				if (json::deserialize(js["data"], data))
				{
					itend = data.end();
					for (it = data.begin(); it != itend; ++it)
					{
						dataTrades dttrade;
						dttrade.ts = stoll(data["ts"]);
						dttrade.instId = data["instId"];
						dttrade.tradeId = data["tradeId"];
						dttrade.px = stod(data["px"]);
						dttrade.sz = stod(data["sz"]);
						if (data["side"] == "buy")
						{
							dttrade.side = OKExEnums::side::BUY;
						}
						else if (data["side"] == "sell")
						{
							dttrade.side = OKExEnums::side::SELL;
						}
						else
						{
							dttrade.side = OKExEnums::side::NONE;
						}
						msg->trades.push_back(dttrade);
					}
				}
			}
		}
	}
}
