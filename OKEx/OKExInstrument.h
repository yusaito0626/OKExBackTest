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
	std::map<std::string, OKExOrder*>* liveOrders;
	double szMyOrd;
	int numOfMyOrd;
	long long ts;

	book();
	book(const book& obj);
	~book();
	void updateBook(OKExEnums::side sd,msgbook* msg);
	void init(void);
	void addOrder(OKExOrder* ord);
	OKExOrder* removeOrder(std::string baseOrdId);
	OKExOrder* getTopOrder(void);
	OKExOrder* updateOrder(dataOrder* tkt);
	bool executeOrder(dataOrder* trd);

};

class position
{
public:
	OKExEnums::instType instType;
	OKExEnums::tradeMode trdMode;
	OKExEnums::posSide posSide;
	double pos;
	double baseBal;
	double quoteBal;
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
	int priceUnit;

	bool isTrading;

	int bookDepth;
	std::map<int, book*>* books;
	std::map<int, book*>::iterator bestAsk;
	std::map<int, book*>::iterator bestBid;
	int lowestBook;
	int highestBook;
	position longPosition;
	position shortPosition;
	double netPosition;

	std::map<std::string, OKExOrder*>* ordList;
	std::map<std::string, OKExOrder*>* liveOrdList;

	int tradedCntBuy;
	int tradedCntSell;
	int tradedQtyBuy;
	int tradedQtySell;
	int tradedAmtBuy;
	int tradedAmtSell;

	double last;
	double mid;
	double open;
	double high;
	double low;

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
	void updateTrade(OKExMktMsg* msg);
	void initializeBooks(OKExMktMsg* msg, int depth);
	bool updateBooks(OKExMktMsg* msg);
	void reshapeBooks(void);
	std::map<int, book*>::iterator findBest(int pr, OKExEnums::side side);
	bool reflectMsg(OKExMktMsg* msg);
	void updateOrders(dataOrder* dtord);

	void updateRings(void);
};

