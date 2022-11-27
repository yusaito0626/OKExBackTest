#include "VirtualOMS.h"

VirtualOMS* voms = VirtualOMS::getInstance();

VirtualOMS::VirtualOMS()
{
	insList = nullptr;
	numOfSellOrd = 0;
	numOfBuyOrd = 0;
	numOfExecSellOrd = 0;
	numOfExecBuyOrd = 0;
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

void VirtualOMS::initialize(std::map<std::string, OKExInstrument*>* _insList, std::string _outputFilePath)
{
	insList = _insList;
	exeFunc = boost::bind(&VirtualOMS::execute, this, _1, _2, _3, _4, _5, _6);
	outputFilePath = _outputFilePath;
}

void VirtualOMS::setNewDate(void)
{
	std::map<std::string, OKExInstrument*>::iterator it;
	std::map<std::string, OKExInstrument*>::iterator itend = insList->end();
	for (it = insList->begin(); it != itend; ++it)
	{
		it->second->execute = exeFunc;
	}
}

OKExOrder* VirtualOMS::sendNewOrder(long long _tm, std::string instId, OKExEnums::tradeMode tdMode, OKExEnums::side side, double px, int sz, OKExEnums::ordType ordtype, std::string& msg)
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
	if (ins->books->find((int)(px * ins->priceUnit)) == bkitend && px > 0)
	{
		msg = "Unknown price. px:" + std::to_string(px);
		return nullptr;
	}
	
	if (sz <= 0 || sz >= (int)round(GlobalVariables::OKEx::maxOrdSize / ins->lotSz))//
	{
		msg = "Irregular size. sz" + std::to_string((double)sz * ins->lotSz);
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
	tkt->idate = GlobalVariables::OKEx::today.iday;
	tkt->ts = tm;
	tkt->clOrdId = ord->ordId;
	tkt->instId = instId;
	tkt->ordId = ord->ordId;
	tkt->ordType = ordtype;
	tkt->px = px;
	tkt->side = side;
	tkt->sz = (double)sz * ins->lotSz;
	tkt->tdMode = tdMode;
	tkt->tktType = OKExEnums::ticketType::_SEND_NEW;

	ordTktQueue->Enqueue(tkt);
	waitingOrderQueue->Enqueue(tkt);
	ordQueue->Enqueue(ord);
	ins->addNewOrder(ord);
	return ord;
}
OKExOrder* VirtualOMS::sendModOrder(long long _tm, std::string instId, std::string ordId, double newPx, int newSz, std::string& msg)
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
	orditend = ins->ordList->end();
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
	if (ins->books->find((int)(newPx * ins->priceUnit)) == bkitend && newPx > 0)
	{
		msg = "Unknown price. px:" + std::to_string(newPx);
		return nullptr;
	}
	if (newSz < 0 || newSz >= (int)round(GlobalVariables::OKEx::maxOrdSize / ins->lotSz))//
	{
		msg = "Irregular size. sz" + std::to_string((double)newSz * ins->lotSz);
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

	tkt->idate = GlobalVariables::OKEx::today.iday;
	tkt->ts = tm;
	tkt->ordId = newOrdId;
	tkt->clOrdId = ord->baseOrdId;//Use clOrdId as baseOrdId
	tkt->instId = instId;
	tkt->px = newPx;
	tkt->sz = (double)newSz * ins->lotSz;
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
	orditend = ins->ordList->end();
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

	tkt->idate = GlobalVariables::OKEx::today.iday;
	tkt->ts = tm;
	tkt->ordId = newOrdId;
	tkt->clOrdId = ord->baseOrdId;
	tkt->instId = instId;
	tkt->tktType = OKExEnums::ticketType::_SEND_CAN;
	ordTktQueue->Enqueue(tkt);
	waitingOrderQueue->Enqueue(tkt);
	return ord;
}


void VirtualOMS::checkWaitingOrdQueue(long long _tm)
{
	OKExInstrument* ins = nullptr;
	ordTicket* tkt = nullptr;
	dataOrder* ack = nullptr;
	while (waitingOrderQueue->Count() > 0)
	{
		tkt = waitingOrderQueue->Peak();
		if (tkt->idate == GlobalVariables::OKEx::today.iday)
		{
			if (_tm > tkt->ts + GlobalVariables::OKEx::latency)
			{
				tkt = waitingOrderQueue->Dequeue();
				ins = insList->at(tkt->instId);
				ack = createAckTicket(_tm, tkt);
				if (ack->tktType != OKExEnums::ticketType::_EXEC)
				{
					ins->updateOrders(ack);
				}
				//ackQueue->Enqueue(ack);
			}
			else
			{
				break;
			}
		}
		else
		{
			tkt = waitingOrderQueue->Dequeue();
			ins = insList->at(tkt->instId);
			ack = createAckTicket(_tm, tkt);
			if (ack->tktType != OKExEnums::ticketType::_EXEC)
			{
				ins->updateOrders(ack);
			}
			//ackQueue->Enqueue(ack);
		}
	}
}



dataOrder* VirtualOMS::createAckTicket(long long _tm, ordTicket* tkt)
{
	dataOrder* dtord = execPool->pop();
	if (!dtord)
	{
		for (int i = 0; i < EXE_POOL_SIZE; ++i)
		{
			execPool->push(new dataOrder());
		}
		dtord = execPool->pop();
	}
	ackQueue->Enqueue(dtord);
	OKExInstrument* ins = insList->at(tkt->instId);
	OKExOrder* objord = ins->ordList->at(tkt->clOrdId);
	dataOrder* exec;
	switch (tkt->tktType)
	{
	case OKExEnums::ticketType::_SEND_NEW:
		dtord->tktType = OKExEnums::ticketType::_ACK_NEW;
		dtord->ordId = getOrdId(tkt->instId);
		dtord->clOrdId = tkt->clOrdId;
		dtord->ccy = tkt->ccy;
		dtord->instId = tkt->instId;
		dtord->ordType = tkt->ordType;
		dtord->px = tkt->px;
		dtord->sz = tkt->sz;
		dtord->side = tkt->side;
		dtord->posSide = tkt->posSide;
		dtord->state = OKExEnums::orderState::_LIVE;
		dtord->uTime = tm;
		dtord->tag = tkt->tag;
		dtord->tdMode = tkt->tdMode;
		switch (dtord->side)
		{
		case OKExEnums::side::_BUY:
			++numOfBuyOrd;
			break;
		case OKExEnums::side::_SELL:
			++numOfSellOrd;
			break;
		default:
			break;
		}
		exec = checkExecution(ins, dtord);
		if (exec)
		{
			dtord = exec;
		}
		break;
	case OKExEnums::ticketType::_SEND_MOD:
		dtord->tktType = OKExEnums::ticketType::_ACK_MOD;
		dtord->ordId = getOrdId(tkt->instId);
		dtord->clOrdId = tkt->clOrdId;
		dtord->ccy = tkt->ccy;
		dtord->instId = tkt->instId;
		dtord->ordType = tkt->ordType;
		dtord->px = tkt->px;
		dtord->sz = tkt->sz;
		dtord->side = tkt->side;
		dtord->posSide = tkt->posSide;
		if ((int)(dtord->sz / ins->lotSz) - objord->execSz > 0)
		{
			dtord->state = OKExEnums::orderState::_PARTIALLY_FILLED;
		}
		else if (objord->execSz > 0)
		{
			dtord->state = OKExEnums::orderState::_FILLED;
		}
		else
		{
			dtord->state = OKExEnums::orderState::_LIVE;
		}
		dtord->uTime = tm;
		dtord->tag = tkt->tag;
		dtord->tdMode = tkt->tdMode;
		exec = checkExecution(ins, dtord);
		if (exec)
		{
			dtord = exec;
		}
		break;
	case OKExEnums::ticketType::_SEND_CAN:
		dtord->tktType = OKExEnums::ticketType::_ACK_MOD;
		dtord->ordId = getOrdId(tkt->instId);
		dtord->clOrdId = tkt->clOrdId;
		dtord->ccy = tkt->ccy;
		dtord->instId = tkt->instId;
		dtord->ordType = tkt->ordType;
		dtord->px = tkt->px;
		dtord->sz = tkt->sz;
		dtord->side = tkt->side;
		dtord->posSide = tkt->posSide;
		if (objord->execSz > 0)
		{
			dtord->state = OKExEnums::orderState::_FILLED;
		}
		else
		{
			dtord->state = OKExEnums::orderState::_LIVE;
		}
		dtord->uTime = tm;
		dtord->tag = tkt->tag;
		dtord->tdMode = tkt->tdMode;
		break;
	default:
		break;
	}
	return dtord;
}

dataOrder* VirtualOMS::checkExecution(OKExInstrument* ins, dataOrder* ack)
{
	std::string msg = "The order took the opposite books";
	OKExOrder* ord = nullptr;
	dataOrder* exec = nullptr;
	int ackSz = (int)(ack->sz / ins->lotSz);
	int bookSz = 0;
	if (ack->px > 0)
	{
		switch (ack->side)
		{
		case OKExEnums::side::_BUY:
			if ((int)(ack->px * ins->priceUnit) < ins->bestAsk->first)
			{
				return nullptr;
			}
			else if ((int)(ack->px * ins->priceUnit) == ins->bestAsk->first)
			{
				ord = ins->ordList->at(ack->clOrdId);
				bookSz = (int)(ins->bestAsk->second->sz / ins->lotSz);
				if (ackSz - ord->execSz > bookSz)
				{
					if (ack->tktType != OKExEnums::ticketType::_EXEC)
					{
						ins->updateOrders(ack);
					}
					exec = execute(tm, ins->instId, ord, bookSz, ack->px, msg);
				}
				else
				{
					if (ack->tktType != OKExEnums::ticketType::_EXEC)
					{
						ins->updateOrders(ack);
					}
					exec = execute(tm, ins->instId, ord, ackSz - ord->execSz, ack->px, msg);
				}
			}
			else
			{
				ord = ins->ordList->at(ack->clOrdId);
				if (ack->tktType != OKExEnums::ticketType::_EXEC)
				{
					ins->updateOrders(ack);
				}
				exec = execute(tm, ins->instId, ord, ackSz - ord->execSz, ack->px, msg);
			}
			break;
		case OKExEnums::side::_SELL:
			if ((int)(ack->px * ins->priceUnit) > ins->bestBid->first)
			{
				return nullptr;
			}
			else if ((int)(ack->px * ins->priceUnit) == ins->bestBid->first)
			{
				ord = ins->ordList->at(ack->clOrdId);
				bookSz = (int)(ins->bestBid->second->sz / ins->lotSz);
				if (ackSz - ord->execSz > bookSz)
				{
					if (ack->tktType != OKExEnums::ticketType::_EXEC)
					{
						ins->updateOrders(ack);
					}
					exec = execute(tm, ins->instId, ord, bookSz, ack->px, msg);
				}
				else
				{
					if (ack->tktType != OKExEnums::ticketType::_EXEC)
					{
						ins->updateOrders(ack);
					}
					exec = execute(tm, ins->instId, ord, ackSz - ord->execSz, ack->px, msg);
				}
			}
			else
			{
				ord = ins->ordList->at(ack->clOrdId);
				ins->updateOrders(ack);
				exec = execute(tm, ins->instId, ord, ackSz - ord->execSz, ack->px, msg);
			}
			break;
		default:
			break;
		}
	}
	else//Market Order
	{
		std::map<int, book*>::iterator bk;
		std::map<int, book*>::iterator bkend = ins->books->end();
		std::map<int, book*>::iterator bkbegin = ins->books->begin();
		int sz = ackSz;
		int exeSz = 0;
		std::string msg;
		ord = ins->ordList->at(ack->clOrdId);
		switch (ack->side)
		{
		case OKExEnums::side::_BUY:
			bk = ins->bestAsk;
			while (sz > 0)
			{
				if (bk == bkend)
				{
					break;
				}
				exeSz = (int)(bk->second->sz / ins->lotSz);
				if (exeSz > sz)
				{
					exeSz = sz;
				}
				exec  = execute(tm, ins->instId, ord, exeSz, bk->second->px, msg);
				sz -= exeSz;
				++bk;
			}
			break;
		case OKExEnums::side::_SELL:
			bk = ins->bestBid;
			while (sz > 0)
			{
				if (bk == ins->books->end())
				{
					break;
				}
				exeSz = (int)(bk->second->sz / ins->lotSz);
				if (exeSz > sz)
				{
					exeSz = sz;
				}
				exec = execute(tm, ins->instId, ord, exeSz, bk->second->px, msg);
				sz -= exeSz;
				if (bk == bkbegin)
				{
					bk = bkend;
				}
				else
				{
					--bk;
				}
			}
			break;
		default:
			break;
		}
	}
	
	ins->checkWaitingExeQueue();
	return exec;
}
dataOrder* VirtualOMS::execute(long long _tm, std::string instId, OKExOrder* ord, int sz, double px, std::string msg)
{
	_tm = tm;
	OKExInstrument* ins = insList->at(instId);
	if (ord->sz - ord->execSz <= 0)
	{
		return nullptr;
	}
	if (sz > ord->sz - ord->execSz)
	{
		sz = ord->sz - ord->execSz;
	}
	if (px <= 0)
	{
		return nullptr;
	}
	if (sz <= 0)
	{
		return nullptr;
	}
	dataOrder* exec = execPool->pop();
	if (!exec)
	{
		for (int i = 0; i < EXE_POOL_SIZE; ++i)
		{
			execPool->push(new dataOrder());
		}
		exec = execPool->pop();
	}
	ackQueue->Enqueue(exec);
	exec->tktType = OKExEnums::ticketType::_EXEC;
	exec->clOrdId = ord->baseOrdId;
	exec->ordId = getOrdId(instId);
	exec->side = ord->side;
	exec->fillPx = px;
	exec->fillSz = (double)sz * ins->lotSz;
	exec->fillTime = tm;
	exec->uTime = tm;
	exec->msg = msg;
	if (ord->sz - ord->execSz - sz > 0)
	{
		exec->state = OKExEnums::orderState::_PARTIALLY_FILLED;
	}
	else
	{
		exec->state = OKExEnums::orderState::_FILLED;
		ord->live = false;
	}
	ackQueue->Enqueue(exec);
	ins->waitingExeQueue->Enqueue(exec);
	return exec;
}

void VirtualOMS::endOfDayReset(void)
{
	std::ofstream ordFile = std::ofstream(outputFilePath + "\\order_" + GlobalVariables::OKEx::suffix + GlobalVariables::OKEx::today.strday + ".csv");
	std::ofstream tktFile = std::ofstream(outputFilePath + "\\ticket_" + GlobalVariables::OKEx::suffix + GlobalVariables::OKEx::today.strday + ".csv");
	std::ofstream ackFile = std::ofstream(outputFilePath + "\\ack_" + GlobalVariables::OKEx::suffix + GlobalVariables::OKEx::today.strday + ".csv");
	std::ofstream exeFile = std::ofstream(outputFilePath + "\\exe_" + GlobalVariables::OKEx::suffix + GlobalVariables::OKEx::today.strday + ".csv");

	OKExOrder* ord;
	ordTicket* tkt;
	dataOrder* ack;

	while (ordQueue->Count() > 0)
	{
		ord = ordQueue->Dequeue();
		ordFile << ord->toString() << std::endl;
		ordFile.flush();
		ord->init();
		ordPool->push(ord);
	}

	while (waitingOrderQueue->Count() > 0)
	{
		ordTktQueue->Enqueue(waitingOrderQueue->Dequeue());
	}
	while (ordTktQueue->Count() > 0)
	{
		tkt = ordTktQueue->Dequeue();
		tktFile << tkt->toString() << std::endl;
		tktFile.flush();
		tkt->init();
		tktPool->push(tkt);
	}

	while (ackQueue->Count() > 0)
	{
		ack = ackQueue->Dequeue();
		ackFile << ack->toString() << std::endl;
		ackFile.flush();
		if (ack->fillSz > 0)
		{
			exeFile << ack->toString() << std::endl;
		}
		ack->init();
		execPool->push(ack);
	}

	numOfSellOrd = 0;
	numOfBuyOrd = 0;
	numOfExecSellOrd = 0;
	numOfExecBuyOrd = 0;
	sellExecQty = 0;
	buyExecQty = 0;
}