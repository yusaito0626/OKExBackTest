#include "OKExOrder.h"

OKExOrder::OKExOrder()
{
	ordId = "";
	baseOrdId = "";

	orgTime = 0;
	updatedTime = 0;

	live = false;
	status = OKExEnums::orderState::_NONE;
	side = OKExEnums::side::_NONE;
	px = 0.0;
	sz = 0.0;
	openSz = 0.0;
	execSz = 0.0;
	execAmt = 0.0;
	avgPx = 0.0;
	lastPx = 0.0;
	lastSz = 0.0;
	newPx = 0.0;
	newSz = 0.0;

	priorQuantity = 0.0;

	isUnwind = false;
	pairInstId = "";
}
OKExOrder::OKExOrder(const OKExOrder& obj)
{
	ordId = obj.ordId;
	baseOrdId = obj.baseOrdId;

	orgTime = obj.orgTime;
	updatedTime = obj.updatedTime;

	live = obj.live;
	status = obj.status;
	side = obj.side;
	px = obj.px;
	sz = obj.sz;
	openSz = obj.openSz;
	execSz = obj.execSz;
	execAmt = obj.execAmt;
	avgPx = obj.avgPx;
	lastPx = obj.lastPx;
	lastSz = obj.lastSz;
	newPx = obj.newPx;
	newSz = obj.newSz;

	priorQuantity = obj.priorQuantity;

	isUnwind = obj.isUnwind;
	pairInstId = obj.pairInstId;
}
OKExOrder::~OKExOrder()
{

}
void OKExOrder::init(void)
{
	ordId = "";
	baseOrdId = "";

	orgTime = 0;
	updatedTime = 0;

	live = false;
	status = OKExEnums::orderState::_NONE;
	side = OKExEnums::side::_NONE;
	px = 0.0;
	sz = 0.0;
	openSz = 0.0;
	execSz = 0.0;
	execAmt = 0.0;
	avgPx = 0.0;
	lastPx = 0.0;
	lastSz = 0.0;
	newPx = 0.0;
	newSz = 0.0;

	priorQuantity = 0.0;

	isUnwind = false;
	pairInstId = "";
}


std::string OKExOrder::toString(void)
{
	return  ordId + ","
		+ baseOrdId + ","
		+ std::to_string(orgTime) + ","
		+ std::to_string(updatedTime) + ","
		+ std::to_string((int)status) + ","
		+ std::to_string((int)side) + ","
		+ std::to_string(px) + ","
		+ std::to_string(sz) + ","
		+ std::to_string(openSz) + ","
		+ std::to_string(execSz) + ","
		+ std::to_string(execAmt) + ","
		+ std::to_string(avgPx) + ","
		+ std::to_string(lastPx) + ","
		+ std::to_string(lastSz);
}