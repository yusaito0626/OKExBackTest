#pragma once
#include "includes.h"
#include "OKExEnums.h"
#include "OKExOrder.h"
#include "OKExMessage.h"

#include "../utils/ring.h"
#include "../utils/LockFreeQueue.h"
#include "../utils/Logging.h"

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

	double recentExec;

	book();
	book(const book& obj);
	~book();
	void updateBook(OKExEnums::side sd,msgbook* msg);
	void init(void);
	void addOrder(OKExOrder* ord);
	OKExOrder* removeOrder(std::string baseOrdId);
	OKExOrder* getTopOrder(void);
	OKExOrder* getTopOrder(OKExEnums::side sd);
	//OKExOrder* updateOrder(dataOrder* tkt);
	//bool executeOrder(dataOrder* trd);

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
	double ctVal;
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
	int displayDigits;

	bool isTrading;

	int bookDepth;
	std::map<int, book*>* books;
	std::map<int, book*>::iterator bestAsk;
	std::map<int, book*>::iterator bestBid;
	std::map<int, book*>::iterator prevBestAsk;
	std::map<int, book*>::iterator prevBestBid;
	int lowestBook;
	int highestBook;
	position longPosition;
	position shortPosition;
	double netPosition;
	double baseMid;
	double prevNetPos;

	double tradePL;
	double posPL;
	double totalPL;

	std::map<std::string, OKExOrder*>* ordList;
	std::map<std::string, OKExOrder*>* liveOrdList;
	std::atomic<bool> lckLiveOrdList;
	LockFreeQueue::SISOQueue<dataOrder*>* waitingExeQueue;

	int intradayOrdBuy;
	int intradayOrdSell;
	int intradayExeBuy;
	int intradayExeSell;
	int intradayExeAmtBuy;
	int intradayExeAmtSell;

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
	double skewWgtExecBid;

	//Optimizer
	//test
	//OKExOrder* bidOrd;
	//OKExOrder* askOrd;
	//arb
	//Pairing SWAP and Futures. USDT margined.
	//The cost of carry will be USDT rate / lever * 2 + the transaction fee(0.02% Future Maker + 0.07% SWAP Taker). Assuming the funding fee of the SWAP is 0.
	double targetRate;
	double transactionFee;
	double targetUnwindDiff;
	double unwindingRatio; //percentage of currentPosDiff
	double unitSz;
	double maxHoldingPos;
	int remainingDays;
	double avgSWAPPr;
	double SWAPSz;
	double avgFUTUREPr;
	double FUTURESz;
	double currentPosDiff;
	double swapBidPr;
	double swapAskPr;
	double realizedPL;
	OKExInstrument* pairedSwap;
	LockFreeQueue::SISOQueue<dataOrder*>* arbExecQueue;


	//Place both bid ans ask order when opening position is 0.
	//Once you got the position, bid or ask ord + unwinding;
	OKExOrder* bidOrd;
	OKExOrder* askOrd;

	OKExInstrument();
	~OKExInstrument();

	void setInstrumentData(std::map<std::string, std::string> mp);
	void setParams(std::map<std::string, std::string> params);
	void updateTrade(OKExMktMsg* msg);
	bool initializeBooks(OKExMktMsg* msg, int depth);
	bool updateBooks(OKExMktMsg* msg);
	void checkExecution(std::map<int,book*>::iterator currentbk,OKExEnums::side orgSide, double orgSz, int pr);
	bool reshapeBooks(void);
	std::map<int, book*>::iterator findBest(int pr, OKExEnums::side side);
	void updateOrders(dataOrder* dtord);
	void addNewOrder(OKExOrder* ord);
	double getPriorQuantity(OKExEnums::side side, double px);
	void calcMid(void);
	void checkWaitingExeQueue(void);

	std::string outputDailyResult(void);
	void endOfDayReset(void);

	boost::function<dataOrder* (long long _tm, std::string instId, OKExOrder* ord, double sz, double px, std::string msg)> execute;
};

