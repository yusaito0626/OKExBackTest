#include "VirtualOMS.h"

VirtualOMS* voms = VirtualOMS::getInstance();

VirtualOMS::VirtualOMS()
{
	insList = nullptr;
	numOfSellOrd = 0;
	numOfBuyOrd = 0;
	numOfExecSellOrd = 0;
	numOfExeBuyOrd = 0;
	sellExecQty = 0;
	buyExecQty = 0;

	_ordNo = 0;
	tm = 0;

	ordPool = new PoolingStack::PoolingStack<OKExOrder*>();
	execPool = new PoolingStack::PoolingStack<dataOrder*>();
	tktPool = new PoolingStack::PoolingStack<ordTicket*>();

	int i;
	for (i = 0; i < ORD_POOL_SIZE; ++i)
	{
		ordPool->push(new OKExOrder());
	}
	for (i = 0; i < EXE_POOL_SIZE; ++i)
	{
		execPool->push(new dataOrder());
	}
	for (i = 0; i < TKT_POOL_SIZE; ++i)
	{
		tktPool->push(new ordTicket());
	}

	ordQueue = new LockFreeQueue::SISOQueue<OKExOrder*>();
	waitingOrderQueue = new LockFreeQueue::SISOQueue<ordTicket*>();
	ordTktQueue = new LockFreeQueue::SISOQueue<ordTicket*>();
	ackQueue = new LockFreeQueue::SISOQueue<dataOrder*>();
}

std::string VirtualOMS::getOrdId(std::string instId)
{
	return instId + std::to_string(GlobalVariables::OKEx::today.iday) + (boost::format("%08d") % ++_ordNo).str();
}

void VirtualOMS::initialize(std::map<std::string, OKExInstrument*>* _insList)
{
	insList = _insList;
}

OKExOrder* VirtualOMS::sendNewOrder(long long _tm, std::string instId, OKExEnums::tradeMode tdMode, OKExEnums::side side, double px, double sz, OKExEnums::ordType ordtype, std::string& msg)
{
	tm = _tm;
	std::map<std::string, OKExInstrument*>::iterator itend = insList->end();
	std::map<int, book*>::iterator bkitend;
	if (insList->find(instId) == itend)
	{
		msg = "Unknown instId. instId:" + instId;
		return nullptr;
	}
	if (side == OKExEnums::side::_NONE)
	{
		msg = "Irregular side.";
		return nullptr;
	}
	OKExInstrument* ins = insList->at(instId);
	bkitend = ins->books->end();
	if (ins->books->find((int)(px * ins->priceUnit)) == bkitend)
	{
		msg = "Unknown price. px:" + std::to_string(px);
		return nullptr;
	}
	if (sz <= 0 || sz >= GlobalVariables::OKEx::maxOrdSize)
	{
		msg = "Irregular size. sz" + std::to_string(sz);
		return nullptr;
	}
	OKExOrder* ord = ordPool->pop();
	if (!ord)
	{
		//Never Let This Happen!!
		for (int i = 0; i < ORD_POOL_SIZE; ++i)
		{
			ordPool->push(new OKExOrder());
		}
		ord = ordPool->pop();
	}
	ordTicket* tkt = tktPool->pop();
	if (!tkt)
	{
		//Never Let This Happen!!
		for (int i = 0; i < TKT_POOL_SIZE; ++i)
		{
			tktPool->push(new ordTicket());
		}
		tkt = tktPool->pop();
	}
	ord->ordId = getOrdId(instId);
	ord->baseOrdId = ord->ordId;
	ord->live = false;
	ord->openSz = sz;
	ord->orgTime = tm;
	ord->px = px;
	ord->side = side;
	ord->status = OKExEnums::orderState::_WAIT_NEW;
	ord->sz = sz;
	ord->updatedTime = tm;

	//This ticket is what we send to OKEx
	tkt->clOrdId = ord->ordId;
	tkt->instId = instId;
	tkt->ordId = ord->ordId;
	tkt->ordType = ordtype;
	tkt->px = px;
	tkt->side = side;
	tkt->sz = sz;
	tkt->tdMode = tdMode;
	tkt->tktType = OKExEnums::ticketType::_SEND_NEW;

	ordTktQueue->Enqueue(tkt);
	waitingOrderQueue->Enqueue(tkt);
	ordQueue->Enqueue(ord);
	ins->ordList->emplace(ord->baseOrdId, ord);
	ins->liveOrdList->emplace(ord->baseOrdId, ord);
	return ord;
}
OKExOrder* VirtualOMS::sendModOrder(long long _tm, std::string instId, std::string ordId, double newPx, double newSz, std::string& msg)
{
	tm = _tm;
	std::map<std::string, OKExInstrument*>::iterator itend = insList->end();
	std::map<std::string, OKExOrder*>::iterator orditend;
	if (insList->find(instId) == itend)
	{
		msg = "Unknown instId. instId:" + instId;
		return nullptr;
	}
	OKExInstrument* ins = insList->at(instId);
	if (ins->ordList->find(ordId) == orditend)
	{
		msg = "Unknown ordId. ordId:" + ordId;
		return nullptr;
	}
	OKExOrder* ord = ins->ordList->at(ordId);
	switch (ord->status)
	{
	case OKExEnums::orderState::_FILLED:
	case OKExEnums::orderState::_CANCELED:
	case OKExEnums::orderState::_NONE:
		msg = "The order is no longer live. ord:" + ordId;
		return nullptr;
		break;
	case OKExEnums::orderState::_WAIT_NEW:
	case OKExEnums::orderState::_WAIT_AMD:
	case OKExEnums::orderState::_WAIT_CAN:
		msg = "The order is waiting the ack. ord:" + ordId;
		return nullptr;
		break;
	default:
		break;
	}
	std::map<int, book*>::iterator bkitend = ins->books->end();
	if (ins->books->find((int)(newPx * ins->priceUnit)) == bkitend)
	{
		msg = "Unknown price. px:" + std::to_string(newPx);
		return nullptr;
	}
	if (newSz <= 0 || newSz >= GlobalVariables::OKEx::maxOrdSize)
	{
		msg = "Irregular size. sz" + std::to_string(newSz);
		return nullptr;
	}
	ordTicket* tkt = tktPool->pop();
	if (!tkt)
	{
		//Never Let This Happen!!
		for (int i = 0; i < TKT_POOL_SIZE; ++i)
		{
			tktPool->push(new ordTicket());
		}
		tkt = tktPool->pop();
	}
	std::string newOrdId = getOrdId(instId);
	ord->ordId = newOrdId;
	ord->newPx = newPx;
	ord->newSz = newSz;
	ord->status = OKExEnums::orderState::_WAIT_AMD;
	ord->updatedTime = tm;

	tkt->ordId = newOrdId;
	tkt->clOrdId = ord->baseOrdId;//Use clOrdId as baseOrdId
	tkt->instId = instId;
	tkt->px = newPx;
	tkt->sz = newSz;
	tkt->tktType = OKExEnums::ticketType::_SEND_MOD;
	ordTktQueue->Enqueue(tkt);
	waitingOrderQueue->Enqueue(tkt);
	return ord;
}
OKExOrder* VirtualOMS::sendCanOrder(long long _tm, std::string instId, std::string ordId, std::string& msg)
{
	tm = _tm;
	std::map<std::string, OKExInstrument*>::iterator itend = insList->end();
	std::map<std::string, OKExOrder*>::iterator orditend;
	if (insList->find(instId) == itend)
	{
		msg = "Unknown instId. instId:" + instId;
		return nullptr;
	}
	OKExInstrument* ins = insList->at(instId);
	if (ins->ordList->find(ordId) == orditend)
	{
		msg = "Unknown ordId. ordId:" + ordId;
		return nullptr;
	}
	OKExOrder* ord = ins->ordList->at(ordId);
	switch (ord->status)
	{
	case OKExEnums::orderState::_FILLED:
	case OKExEnums::orderState::_CANCELED:
	case OKExEnums::orderState::_NONE:
		msg = "The order is no longer live. ord:" + ordId;
		return nullptr;
		break;
	default:
		break;
	}

	ordTicket* tkt = tktPool->pop();
	if (!tkt)
	{
		//Never Let This Happen!!
		for (int i = 0; i < TKT_POOL_SIZE; ++i)
		{
			tktPool->push(new ordTicket());
		}
		tkt = tktPool->pop();
	}
	std::string newOrdId = getOrdId(instId);
	ord->ordId = newOrdId;
	ord->updatedTime = tm;
	ord->status = OKExEnums::orderState::_WAIT_CAN;

	tkt->ordId = newOrdId;
	tkt->clOrdId = ord->baseOrdId;
	tkt->instId = instId;
	tkt->tktType = OKExEnums::ticketType::_SEND_CAN;
	ordTktQueue->Enqueue(tkt);
	waitingOrderQueue->Enqueue(tkt);
	return ord;
}

dataOrder* VirtualOMS::createAckTicket(ordTicket* ord)
{
	return nullptr;
}
dataOrder* VirtualOMS::execute(std::string instId, OKExOrder* ord, double sz, double px, std::string msg)
{
	return nullptr;
}