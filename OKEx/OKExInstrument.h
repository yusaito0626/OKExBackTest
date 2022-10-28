#pragma once
#include "includes.h"
#include "OKExEnums.h"
#include "OKExOrder.h"
#include "OKExMessage.h"

#include "../utils/ring.h"

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
	std::string alias;
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
	double tradedQtyBuy;
	double tradedQtySell;
	double tradedAmtBuy;
	double tradedAmtSell;
	double skewWgtTradedQtyBuy;
	double skewWgtTradedQtySell;


	double last;
	double mid;
	double open;
	double high;
	double low;
	double MAMid;

	long long ts;
	long long lastOptTs;

	//Factors
	double bookImbalance;
	double currentRV;
	double execImbalance;
	double tradeQtyImbalance;
	double skewWgtExecImbalance;
	double hret;
	int currentTradedQtyBuy;
	int currentTradedQtySell;

	//Setting
	int BITicks;
	double BIDecayingParam;
	int EIPeriod;
	double EIDecayingParam;
	int TQIPeriod;
	double TQIDecayingParam;
	int SkewWgtEIPeriod;
	double SkewWgtEIDecayingParam;
	int MAPeriod;
	int RVPeriod;
	int histReturnPeriod;

	//Ring Buffer
	static const int RINGSIZE = 180;
	int ringIdx;
	int ringDataCount;
	long long lastRingUpdatedTime;
	ring<double>* RVRing[60];
	ring<double>* netPosRing[60];
	ring<double>* bookImbalanceRing[60];
	ring<double>* execImbalanceRing[60];
	ring<int>* bestAskRing[60];
	ring<int>* bestBidRing[60];
	ring<int>* execBidCntRing[60];
	ring<int>* execAskCntRing[60];
	ring<double>* execBidQtyRing[60];
	ring<double>* execAskQtyRing[60];
	ring<double>* SkewWgtExecBidQtyRing[60];
	ring<double>* SkewWgtExecAskQtyRing[60];
	ring<int>* tradeCntBuyRing[60];
	ring<int>* tradeCntSellRing[60];
	ring<double>* tradeQtyBuyRing[60];
	ring<double>* tradeQtySellRing[60];
	ring<double>* SkewWgtTradeQtyBuyRing[60];
	ring<double>* SkewWgtTradeQtySellRing[60];
	ring<double>* midRing[60];
	ring<double>* midMARing[60];

	double realizedVolatility;
	int execAskCnt;
	int execBidCnt;
	double execAskVol;
	double execBidVol;
	double execAskAmt;
	double execBidAmt;
	double skewWgtExecAsk;
	double skewWgtExecBid;;

	OKExInstrument();
	~OKExInstrument();

	void setInstrumentData(std::map<std::string, std::string> mp);
	void setParams(std::map<std::string, std::string> params);
	void updateTrade(OKExMktMsg* msg);
	void initializeBooks(OKExMktMsg* msg, int depth);
	bool updateBooks(OKExMktMsg* msg);
	void reshapeBooks(void);
	std::map<int, book*>::iterator findBest(int pr, OKExEnums::side side);
	bool reflectMsg(OKExMktMsg* msg);
	void updateOrders(dataOrder* dtord);
	double getPriorQuantity(OKExEnums::side side, double px);
};

