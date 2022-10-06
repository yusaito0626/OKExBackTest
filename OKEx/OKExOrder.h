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
	int px;
	double sz;
	double openSz;
	double execSz;
	double execAmt;
	double avgPx;
	int lastPx;
	double lastSz;
	int newPx;
	double newSz;

	OKExOrder();
	OKExOrder(const OKExOrder& obj);
	~OKExOrder();
	void init(void);
};

