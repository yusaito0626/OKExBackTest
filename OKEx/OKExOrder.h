#pragma once
#include "includes.h"
#include "OKExEnums.h"
#include "OKExMessage.h"

class OKExOrder
{
public:
	std::string ordId;
	std::string baseOrdId;

	long long orgTime;
	long long updatedTime;

	bool live;
	OKExEnums::orderState status;
	OKExEnums::side side;
	double px;
	int sz;
	int openSz;
	int execSz;
	double execAmt;
	double avgPx;
	double lastPx;
	int lastSz;
	double newPx;
	double newSz;

	double priorQuantity;

	bool isUnwind;//For Arb
	std::string pairInstId;//For Arb

	OKExOrder();
	OKExOrder(const OKExOrder& obj);
	~OKExOrder();
	void init(void);
	std::string toString(void);
};

