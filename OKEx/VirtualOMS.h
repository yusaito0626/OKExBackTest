#pragma once

#include "includes.h"
#include "GlobalVariables.h"
#include "OKExInstrument.h"
#include "OKExOrder.h"
#include "OKExEnums.h"
#include "OKExMessage.h"

#include "../utils/LockFreeQueue.h"

class VirtualOMS
{
	VirtualOMS();
	VirtualOMS(const VirtualOMS&) {};
	VirtualOMS& operator=(const VirtualOMS&) {};

	int _ordNo;
	long long tm;
	std::string getOrdId(std::string instId);
public:
	~VirtualOMS() {};

	std::string outputFilePath;
	std::map<std::string, OKExInstrument*>* insList;
	int numOfSellOrd;
	int numOfBuyOrd;
	int numOfExecSellOrd;
	int numOfExecBuyOrd;
	long long sellExecQty;
	long long buyExecQty;

	void initialize(std::map<std::string, OKExInstrument*>* _insList, std::string _outputFilePath);
	void setNewDate(void);

	//Put same parameters as live
	OKExOrder* sendNewOrder(long long _tm,std::string instId,OKExEnums::tradeMode tdMode,OKExEnums::side side, double px, int sz,OKExEnums::ordType ordtype, std::string& msg);
	OKExOrder* sendModOrder(long long _tm, std::string instId, std::string ordId,double newPx,int newSz, std::string& msg);
	OKExOrder* sendCanOrder(long long _tm, std::string instId, std::string ordId,std::string& msg);

	void checkWaitingOrdQueue(long long _tm);
	dataOrder* createAckTicket(long long _tm, ordTicket* tkt);
	dataOrder* checkExecution(OKExInstrument* ins, dataOrder* ack);
	dataOrder* execute(long long _tm, std::string instId, OKExOrder* ord, int sz, double px, std::string msg);

	void endOfDayReset(void);

	static const int ORD_POOL_SIZE = 100000;
	static const int EXE_POOL_SIZE = 500000;
	static const int TKT_POOL_SIZE = 100000;

	PoolingStack::PoolingStack<OKExOrder*>* ordPool;
	PoolingStack::PoolingStack<dataOrder*>* execPool;
	PoolingStack::PoolingStack<ordTicket*>* tktPool;

	LockFreeQueue::SISOQueue<OKExOrder*>* ordQueue;//All of orders must be queued and put back to pool at the end of the day.
	LockFreeQueue::SISOQueue<ordTicket*>* waitingOrderQueue;
	LockFreeQueue::SISOQueue<ordTicket*>* ordTktQueue;
	LockFreeQueue::SISOQueue<dataOrder*>* ackQueue;

	boost::function<dataOrder* (long long _tm, std::string instId, OKExOrder* ord, int sz, double px, std::string msg)> exeFunc;

	static VirtualOMS* getInstance(void)
	{
		static VirtualOMS* singleton;
		if (!singleton)
		{
			singleton = new VirtualOMS();
		}
		return singleton;
	}
};

extern VirtualOMS* voms;

