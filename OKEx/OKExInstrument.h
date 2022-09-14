#pragma once
#include "includes.h"
#include "OKExEnums.h"
#include "OKExOrder.h"
#include "OKExMessage.h"
class book
{
public:
	OKExEnums::side side;
	double px;
	double sz;
	double szSell;
	double szBuy;
	int liqOrd;
	int numOfOrdBuy;
	int numOfOrdSell;
	std::map<std::string, OKExOrder*> liveOrders;
	double szMyOrd;
	int numOfMyOrd;
	long long ts;

	book();
	~book();
	void init(void);

};

class position
{
public:
	OKExEnums::instType instType;
	OKExEnums::tradeMode trdMode;
	OKExEnums::posSide posSide;
	double pos;
	double baseBal;
	double quoteBl;
	double posCcy;
	double posId;
	double availPos;
	double avgPx;
	double upl;
	double uplRatio;

	std::string instId;
	std::string tradeId;
	double lever;
	double liqPx;
	double markPx;
	double imr;
	double margin;
	double mgnRatio;
	double mmr;
	double liab;
	double liabCcy;
	double interest;
	double notionalUsd;
	double optVal;
	double adl;
	std::string ccy;
	double last;
	double usdPx;
	double deltaBS;
	double deltaPA;
	double gammaBS;
	double gammaPA;
	double thetaBS;
	double thetaPA;
	double vegaBS;
	double vegaPA;
	long long cTime;
	long long uTime;
	long long pTime;

	position();
	~position();
	void init(void);
};

class OKExInstrument
{
public:
	std::string instId;
	OKExEnums::instType instType;
	std::string baseCcy;
	std::string quoteCcy;
	std::string settleCcy;
	std::string uly;
	int category;
	int ctMulti;
	OKExEnums::ctType ctType;
	int ctVal;
	std::string ctValCcy;
	long long expTime;
	int lever;
	long long listTime;
	double lotSz;
	int maxIcebergSz;
	int maxLmtSz;
	int maxMktSz;
	int maxStopSz;
	int maxTriggerSz;
	int maxTwapSz;
	double minSz;
	OKExEnums::insState state;
	double tickSz;

	bool isTrading;

	int bookDepth;
	std::map<int, book> books;
	std::map<int, book>::iterator bestAsk;
	std::map<int, book>::iterator bestBid;
	position longPosition;
	position shortPosition;
	double netPosition;

	std::map<std::string, OKExOrder*>* odrList;
	std::map<std::string, OKExOrder*>* liveOdrList;

	int tradedCntBuy;
	int tradedCntSell;
	int tradedQtyBuy;
	int tradedQtySell;
	int tradedAmtBuy;
	int tradedAmtSell;

	double last;
	double mid;

	long long ts;
	long long lastOptTs;

	//Factors
	double bookImbalance;
	double currentRV;
	double exeImbalance;
	double sret;
	int currentTradedQtyBuy;
	int currentTradedQtySell;

	double realizedVolatility;
	int execAskCnt;
	int execBidCnt;
	double execAskVol;
	double execBidVol;
	double execAskAmt;
	double execBidAmt;

	OKExInstrument();
	~OKExInstrument();

	void setInstrumentData(std::map<std::string, std::string> mp);
	void setParams(std::list<std::string> params);
	void updateTrade(dataTrades* trade);
	void updateBooks(dataBooks* books);
	void updateOrders(dataOrder* ord);

	void updateRings(void);
};

