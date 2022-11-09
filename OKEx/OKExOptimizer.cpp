#include "OKExOptimizer.h"

OKExOptimizer* optimizer = OKExOptimizer::getInstance();

void OKExOptimizer::initilaize(std::map<std::string, OKExInstrument*>* _insList)
{

}


void OKExOptimizer::optimize(OKExInstrument* ins)
{
	
}

void OKExOptimizer::test(OKExInstrument* ins)
{
	//Simple Testing Optimization
	std::string msg;
	std::map<int, book*>::iterator bkend = ins->books->end();
	if (ins->bestBid != bkend)
	{
		std::map<int, book*>::iterator targetBuyPr = ins->books->find(ins->bestBid->first - 10);
		if (targetBuyPr != bkend)
		{
			if (!ins->bidOrd)
			{
				ins->bidOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_BUY, targetBuyPr->second->px, 1, OKExEnums::ordType::_LIMIT, msg);
			}
			else if (ins->bidOrd->status != OKExEnums::orderState::_WAIT_NEW && ins->bidOrd->status != OKExEnums::orderState::_WAIT_AMD && ins->bidOrd->status != OKExEnums::orderState::_WAIT_CAN)
			{
				if (ins->bidOrd->status == OKExEnums::orderState::_CANCELED || ins->bidOrd->status == OKExEnums::orderState::_FILLED)
				{
					ins->bidOrd = nullptr;
				}
				else if (ins->bidOrd->px != ins->bestBid->second->px)
				{
					ins->bidOrd = voms->sendModOrder(ins->ts, ins->instId, ins->bidOrd->baseOrdId, targetBuyPr->second->px, ins->bidOrd->sz, msg);
				}
			}
		}
		else if (ins->bidOrd)
		{
			if (ins->bidOrd->status == OKExEnums::orderState::_CANCELED || ins->bidOrd->status == OKExEnums::orderState::_FILLED)
			{
				ins->bidOrd = nullptr;
			}
			else
			{
				voms->sendCanOrder(ins->ts, ins->instId, ins->bidOrd->baseOrdId, msg);
			}
		}
	}
	
	if (ins->bestAsk != ins->books->end())
	{
		std::map<int, book*>::iterator targetSellPr = ins->books->find(ins->bestAsk->first + 10);
		if (targetSellPr != bkend)
		{
			if (!ins->askOrd)
			{
				ins->askOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_SELL, targetSellPr->second->px, 1, OKExEnums::ordType::_LIMIT, msg);
			}
			else if (ins->askOrd->status != OKExEnums::orderState::_WAIT_NEW && ins->askOrd->status != OKExEnums::orderState::_WAIT_AMD && ins->askOrd->status != OKExEnums::orderState::_WAIT_CAN)
			{
				if (ins->askOrd->status == OKExEnums::orderState::_CANCELED || ins->askOrd->status == OKExEnums::orderState::_FILLED)
				{
					ins->askOrd = nullptr;
				}
				else if (ins->askOrd->px != ins->bestAsk->second->px)
				{
					ins->askOrd = voms->sendModOrder(ins->ts, ins->instId, ins->askOrd->baseOrdId, targetSellPr->second->px, ins->askOrd->sz, msg);
				}
			}
		}
		else if (ins->askOrd)
		{
			if (ins->askOrd->status == OKExEnums::orderState::_CANCELED || ins->askOrd->status == OKExEnums::orderState::_FILLED)
			{
				ins->askOrd = nullptr;
			}
			else
			{
				voms->sendCanOrder(ins->ts, ins->instId, ins->askOrd->baseOrdId, msg);
			}
		}
	}
}
void OKExOptimizer::arb(OKExInstrument* ins)
{
	std::string msg;
	if (ins->pairedSwap->bestAsk != ins->pairedSwap->books->end())
	{
		ins->swapAskPr = ins->pairedSwap->bestAsk->second->px;
	}
	else
	{
		ins->swapAskPr = 0;
	}
	if (ins->pairedSwap->bestBid != ins->pairedSwap->books->end())
	{
		ins->swapBidPr = ins->pairedSwap->bestBid->second->px;
	}
	else
	{
		ins->swapBidPr = 0;
	}
	if (ins->netPosition == 0)//Open Orders
	{
		if(ins->swapBidPr > 0)//Buy Order
		{
			double targetPr = floor(ins->swapBidPr * (1 - ins->remainingDays / 365 * ins->targetRate) * ins->priceUnit) / ins->priceUnit;
			double sz = ins->unitSz;
			if (ins->netPosition + sz > ins->maxHoldingPos)
			{
				sz = ins->maxHoldingPos - ins->netPosition;
				if (sz < 0)
				{
					sz = 0;
				}
			}
			if (targetPr > ins->bestBid->second->px)
			{
				targetPr = ins->bestBid->second->px;
			}
			if (!ins->bidOrd)
			{
				if (sz > 0 && targetPr > 0)
				{
					ins->bidOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_BUY, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
				}
			}
			else
			{
				if (ins->bidOrd->status == OKExEnums::orderState::_CANCELED || ins->bidOrd->status == OKExEnums::orderState::_FILLED)
				{
					if (sz > 0 && targetPr > 0)
					{
						ins->bidOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_BUY, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
					}
					else
					{
						ins->bidOrd = nullptr;
					}
				}
				else
				{
					if (targetPr != ins->bidOrd->px || sz < ins->bidOrd->sz)
					{
						if (targetPr <= 0)
						{
							sz = 0;//Cancel
						}
						voms->sendModOrder(ins->ts, ins->instId, ins->bidOrd->baseOrdId, targetPr, sz, msg);
					}
				}
			}
		}
		if (ins->swapAskPr > 0)//Sell Order
		{
			double targetPr = ceil(ins->swapAskPr * (1 + ins->remainingDays / 365 * ins->targetRate) * ins->priceUnit) / ins->priceUnit;
			double sz = ins->unitSz;
			if (ins->netPosition + sz > ins->maxHoldingPos)
			{
				sz = ins->maxHoldingPos - ins->netPosition;
				if (sz < 0)
				{
					sz = 0;
				}
			}
			if (targetPr < ins->bestAsk->second->px)
			{
				targetPr = ins->bestAsk->second->px;
			}
			if (!ins->askOrd)
			{
				if (sz > 0 && targetPr > 0)
				{
					ins->askOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_SELL, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
				}
			}
			else
			{
				if (ins->askOrd->status == OKExEnums::orderState::_CANCELED || ins->askOrd->status == OKExEnums::orderState::_FILLED)
				{
					if (sz > 0 && targetPr > 0)
					{
						ins->askOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_SELL, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
					}
					else
					{
						ins->askOrd = nullptr;
					}
				}
				else
				{
					if (targetPr != ins->askOrd->px || sz < ins->askOrd->sz)
					{
						if (targetPr <= 0)
						{
							sz = 0;//Cancel
						}
						voms->sendModOrder(ins->ts, ins->instId, ins->askOrd->baseOrdId, targetPr, sz, msg);
					}
				}
			}
		}
	}
	else if (ins->netPosition > 0)//Open Future:Buy SWAP:Sell, Unwind Future:Sell SWAP:Buy
	{
		if (ins->swapBidPr > 0)//Open
		{
			double targetPr = floor(ins->swapBidPr * (1 - ins->remainingDays / 365 * ins->targetRate) * ins->priceUnit) / ins->priceUnit;
			double sz = ins->unitSz;
			if (ins->netPosition + sz > ins->maxHoldingPos)
			{
				sz = ins->maxHoldingPos - ins->netPosition;
				if (sz < 0)
				{
					sz = 0;
				}
			}
			if (targetPr > ins->bestBid->second->px)
			{
				targetPr = ins->bestBid->second->px;
			}
			if (!ins->bidOrd)
			{
				if (sz > 0 && targetPr > 0)
				{
					ins->bidOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_BUY, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
				}
			}
			else
			{
				if (ins->bidOrd->status == OKExEnums::orderState::_CANCELED || ins->bidOrd->status == OKExEnums::orderState::_FILLED)
				{
					if (sz > 0 && targetPr > 0)
					{
						ins->bidOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_BUY, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
					}
					else
					{
						ins->bidOrd = nullptr;
					}
				}
				else
				{
					if (targetPr != ins->bidOrd->px || sz < ins->bidOrd->sz)
					{
						if (targetPr <= 0)
						{
							sz = 0;//Cancel
						}
						voms->sendModOrder(ins->ts, ins->instId, ins->bidOrd->baseOrdId, targetPr, sz, msg);
					}
				}
			}
		}
		if (ins->swapAskPr > 0)//Unwind
		{
			ins->targetUnwindDiff = ins->currentPosDiff * ins->unwindingRatio;
			double targetPr = ceil(ins->swapAskPr + ins->targetUnwindDiff * ins->priceUnit) / ins->priceUnit;
			double sz = ins->unitSz;
			if (sz > ins->netPosition)//Net Position is positive.
			{
				sz = ins->netPosition;
			}
			if (targetPr < ins->bestAsk->second->px)
			{
				targetPr = ins->bestAsk->second->px;
			}
			if (!ins->askOrd)
			{
				if (sz > 0 && targetPr > 0)
				{
					ins->askOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_SELL, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
				}
			}
			else
			{
				if (ins->askOrd->status == OKExEnums::orderState::_CANCELED || ins->askOrd->status == OKExEnums::orderState::_FILLED)
				{
					if (sz > 0 && targetPr > 0)
					{
						ins->askOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_SELL, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
					}
					else
					{
						ins->askOrd = nullptr;
					}
				}
				else
				{
					if (targetPr != ins->askOrd->px || sz < ins->askOrd->sz)
					{
						if (targetPr <= 0)
						{
							sz = 0;//Cancel
						}
						voms->sendModOrder(ins->ts, ins->instId, ins->askOrd->baseOrdId, targetPr, sz, msg);
					}
				}
			}
		}
	}
	else if (ins->netPosition < 0)//Open Future:Sell SWAP:Buy, Unwind Future:Buy SWAP:Sell
	{
		if (ins->swapAskPr > 0)//Open
		{
			double targetPr = floor(ins->swapAskPr * (1 + ins->remainingDays / 365 * ins->targetRate) * ins->priceUnit) / ins->priceUnit;
			double sz = ins->unitSz;
			if ( - ins->netPosition + sz > ins->maxHoldingPos)
			{
				sz = ins->maxHoldingPos + ins->netPosition;
				if (sz < 0)
				{
					sz = 0;
				}
			}
			if (targetPr < ins->bestAsk->second->px)
			{
				targetPr = ins->bestAsk->second->px;
			}
			if (!ins->askOrd)
			{
				if (sz > 0 && targetPr > 0)
				{
					ins->askOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_SELL, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
				}
			}
			else
			{
				if (ins->askOrd->status == OKExEnums::orderState::_CANCELED || ins->askOrd->status == OKExEnums::orderState::_FILLED)
				{
					if (sz > 0 && targetPr > 0)
					{
						ins->askOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_SELL, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
					}
					else
					{
						ins->askOrd = nullptr;
					}
				}
				else
				{
					if (targetPr != ins->askOrd->px || sz < ins->askOrd->sz)
					{
						if (targetPr <= 0)
						{
							sz = 0;//Cancel
						}
						voms->sendModOrder(ins->ts, ins->instId, ins->askOrd->baseOrdId, targetPr, sz, msg);
					}
				}
			}
		}
		if (ins->swapBidPr > 0)//Unwind
		{
			ins->targetUnwindDiff = ins->currentPosDiff * ins->unwindingRatio;
			double targetPr = ceil(ins->swapBidPr - ins->targetUnwindDiff * ins->priceUnit) / ins->priceUnit;
			double sz = ins->unitSz;
			if (sz > - ins->netPosition)//Net Position is negative.
			{
				sz = - ins->netPosition;
			}
			if (targetPr < ins->bestBid->second->px)
			{
				targetPr = ins->bestBid->second->px;
			}
			if (!ins->bidOrd)
			{
				if (sz > 0 && targetPr > 0)
				{
					ins->bidOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_BUY, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
				}
			}
			else
			{
				if (ins->bidOrd->status == OKExEnums::orderState::_CANCELED || ins->bidOrd->status == OKExEnums::orderState::_FILLED)
				{
					if (sz > 0 && targetPr > 0)
					{
						ins->bidOrd = voms->sendNewOrder(ins->ts, ins->instId, OKExEnums::tradeMode::_CROSS, OKExEnums::side::_BUY, targetPr, sz, OKExEnums::ordType::_LIMIT, msg);
					}
					else
					{
						ins->bidOrd = nullptr;
					}
				}
				else
				{
					if (targetPr != ins->bidOrd->px || sz < ins->bidOrd->sz)
					{
						if (targetPr <= 0)
						{
							sz = 0;//Cancel
						}
						voms->sendModOrder(ins->ts, ins->instId, ins->bidOrd->baseOrdId, targetPr, sz, msg);
					}
				}
			}
		}
	}
}
void OKExOptimizer::arbExeTrigger(dataOrder* exec, OKExInstrument* ins)
{
	if (ins->instType == OKExEnums::instType::_FUTURES)//Send Market Order of SWAP
	{
		std::string msg;
		OKExOrder* ord = ins->ordList->at(exec->clOrdId);
		OKExInstrument* pairFtr = ins->pairedSwap;
		OKExEnums::side side;
		switch (ord->side)
		{
		case OKExEnums::side::_BUY:
			side = OKExEnums::side::_SELL;
			break;
		case OKExEnums::side::_SELL:
			side = OKExEnums::side::_BUY;
			break;
		default:
			break;
		}
		OKExOrder* newOrd = voms->sendNewOrder(ins->ts, pairFtr->instId, OKExEnums::tradeMode::_CROSS, side, 0, exec->fillSz, OKExEnums::ordType::_MARKET, msg);
		newOrd->pairInstId = ins->instId;
		newOrd->isUnwind = ord->isUnwind;
		if (ord->isUnwind)
		{
			ins->FUTURESz -= exec->fillSz;
			if (ins->FUTURESz < ins->lotSz)
			{
				ins->FUTURESz = 0;
				ins->avgFUTUREPr = 0;
			}
		}
		else
		{
			ins->avgFUTUREPr= (ins->avgFUTUREPr* ins->FUTURESz+ exec->fillPx * exec->fillSz) / (ins->FUTURESz + exec->fillSz);
			ins->FUTURESz += exec->fillSz;
		}
	}
	else if (ins->instType == OKExEnums::instType::_SWAP)//Record Execution information to the future
	{
		OKExOrder* ord = ins->ordList->at(exec->clOrdId);
		OKExInstrument* pairFtr = insList->at(ord->pairInstId);
		if (ord->isUnwind)
		{
			pairFtr->SWAPSz -= exec->fillSz;
			if (pairFtr->SWAPSz < pairFtr->lotSz)
			{
				pairFtr->SWAPSz = 0;
				pairFtr->avgSWAPPr = 0;
			}
		}
		else
		{
			pairFtr->avgSWAPPr = (pairFtr->avgSWAPPr * pairFtr->SWAPSz + exec->fillPx * exec->fillSz) / (pairFtr->SWAPSz + exec->fillSz);
			pairFtr->SWAPSz += exec->fillSz;
		}
	}
}

void OKExOptimizer::calcFactors(OKExInstrument* ins)
{
	if (ins->ringDataCount > ins->RVPeriod)
	{
		int idx = 0;
		int rel = 0;
		idx = ins->ringIdx - ins->RVPeriod;
		while (idx < 0)
		{
			idx += 60;
			if (idx < 0 || idx <= ins->ringIdx)
			{
				--rel;
			}
		}
		ins->currentRV = pow(ins->realizedVolatility - ins->RVRing[idx]->relative(rel), 0.5);
	}
	if (ins->ringDataCount > ins->histReturnPeriod)
	{
		int idx = 0;
		int rel = 0;
		idx = ins->ringIdx - ins->histReturnPeriod;
		while (idx < 0)
		{
			idx += 60;
			if (idx < 0 || idx <= ins->ringIdx)
			{
				--rel;
			}
		}
		ins->hret = log(ins->mid / ins->midRing[idx]->relative(rel));
	}
	ins->bookImbalance = calcBookImbalance(ins);
	ins->execImbalance = calcExecImbalance(ins);
	ins->tradeQtyImbalance = calcTradeQtyImbalance(ins);
	ins->skewWgtExecImbalance = calcSkewWgtExecImbalance(ins);
}
void OKExOptimizer::updateRings(OKExInstrument* ins)
{
	if (ins->ts - ins->lastRingUpdatedTime > 1000)
	{
		if (ins->lastRingUpdatedTime == 0)
		{
			ins->lastRingUpdatedTime = (long long)floor((double)ins->ts / 1000) * 1000;
		}
		else
		{
			int cnt = 0;
			while (ins->ts - ins->lastRingUpdatedTime > 0)
			{
				++ins->ringIdx;
				if (ins->ringIdx > 59)
				{
					ins->ringIdx = 0;
				}
				ins->lastRingUpdatedTime += 1000;
				ins->RVRing[ins->ringIdx]->add(ins->realizedVolatility);
				ins->netPosRing[ins->ringIdx]->add(ins->netPosition);
				ins->bookImbalanceRing[ins->ringIdx]->add(ins->bookImbalance);
				ins->execImbalanceRing[ins->ringIdx]->add(ins->execImbalance);
				ins->bestAskRing[ins->ringIdx]->add(ins->bestAsk->first);
				ins->bestBidRing[ins->ringIdx]->add(ins->bestBid->first);
				ins->execBidCntRing[ins->ringIdx]->add(ins->execBidCnt);
				ins->execAskCntRing[ins->ringIdx]->add(ins->execAskCnt);
				ins->execBidQtyRing[ins->ringIdx]->add(ins->execBidVol);
				ins->execAskQtyRing[ins->ringIdx]->add(ins->execAskVol);
				ins->SkewWgtExecBidQtyRing[ins->ringIdx]->add(ins->skewWgtExecBid);
				ins->SkewWgtExecAskQtyRing[ins->ringIdx]->add(ins->skewWgtExecAsk);
				ins->tradeCntBuyRing[ins->ringIdx]->add(ins->tradedCntBuy);
				ins->tradeCntSellRing[ins->ringIdx]->add(ins->tradedCntSell);
				ins->tradeQtyBuyRing[ins->ringIdx]->add(ins->tradedQtyBuy);
				ins->tradeQtySellRing[ins->ringIdx]->add(ins->tradedQtySell);
				ins->SkewWgtTradeQtyBuyRing[ins->ringIdx]->add(ins->skewWgtTradedQtyBuy);
				ins->SkewWgtTradeQtySellRing[ins->ringIdx]->add(ins->skewWgtTradedQtySell);
				ins->midRing[ins->ringIdx]->add(ins->mid);
				++ins->ringDataCount;
				ins->MAMid = calcMidMA(ins);
				ins->midMARing[ins->ringIdx]->add(ins->MAMid);
				++cnt;
				if (cnt > 10000)
				{
					break;
				}
			}
		}
	}
}

double OKExOptimizer::calcMidMA(OKExInstrument* ins)
{
	if (ins->ringDataCount <= ins->MAPeriod)
	{
		return 0;
	}
	double MAMid = 0;
	int i = 0;
	int idx = 0;
	int rel = 0;
	while (i < ins->MAPeriod)
	{
		idx = ins->ringIdx - i;
		while (idx < 0)
		{
			idx += 60;
			if (idx < 0 || idx <= ins->ringIdx)
			{
				--rel;
			}
		}
		MAMid += ins->midRing[idx]->relative(rel);
		++i;
	}
	MAMid /= ins->MAPeriod;
	return MAMid;
}
double OKExOptimizer::calcBookImbalance(OKExInstrument* ins)
{
	std::map<int, book*>::iterator bid = ins->bestBid;
	std::map<int, book*>::iterator ask = ins->bestAsk;
	std::map<int, book*>::iterator bookend = ins->books->end();
	std::map<int, book*>::iterator bookbegin = ins->books->begin();
	int i = 0;
	double bidsum = 0;
	double asksum = 0;
	while (i < ins->BITicks)
	{
		if (bid != bookend)
		{
			bidsum += bid->second->sz * exp(-log((double)ins->bestBid->first / (double)bid->first) * ins->BIDecayingParam);
		}
		if (bid == bookbegin)
		{
			bid = bookend;
		}
		else
		{
			--bid;
		}
		if (ask != bookend)
		{
			asksum += ask->second->sz * exp(-log((double)ask->first / (double)ins->bestAsk->first) * ins->BIDecayingParam);
		}
		++i;
	}
	if (bidsum < 1)
	{
		bidsum = 1;
	}
	if (asksum < 1)
	{
		asksum = 1;
	}
	return log(bidsum / asksum);
}
double OKExOptimizer::calcExecImbalance(OKExInstrument* ins)
{
	if (ins->ringDataCount <= ins->EIPeriod)
	{
		return 0;
	}
	double wgtSumOfAskExec = 0;
	double wgtSumOfBidExec = 0;
	double currentExecAsk = ins->execAskVol;
	double currentExecBid = ins->execBidVol;
	double minexe = 1;

	int i = 0;
	int idx = 0;
	int rel = 0;
	while (i < ins->EIPeriod)
	{
		idx = ins->ringIdx - i;
		while (idx < 0)
		{
			idx += 60;
			if (idx < 0 || idx <= ins->ringIdx)
			{
				--rel;
			}
		}
		wgtSumOfAskExec += (currentExecAsk - ins->execAskQtyRing[idx]->relative(rel)) * exp(-i * ins->EIDecayingParam);
		wgtSumOfBidExec += (currentExecBid - ins->execBidQtyRing[idx]->relative(rel)) * exp(-i * ins->EIDecayingParam);
		currentExecAsk = ins->execAskQtyRing[idx]->relative(rel);
		currentExecBid = ins->execBidQtyRing[idx]->relative(rel);
		++i;
	}
	if (wgtSumOfAskExec < minexe)
	{
		wgtSumOfAskExec = minexe;
	}
	if (wgtSumOfBidExec < minexe)
	{
		wgtSumOfBidExec = minexe;
	}
	return log(wgtSumOfAskExec / wgtSumOfBidExec);
}
double OKExOptimizer::calcTradeQtyImbalance(OKExInstrument* ins)
{
	if (ins->ringDataCount <= ins->TQIPeriod)
	{
		return 0;
	}
	double wgtSumOfSellTrade = 0;
	double wgtSumOfBuyTrade = 0;
	double currentTradeSell = ins->tradedQtySell;
	double currentTradeBuy = ins->tradedQtyBuy;
	double minexe = 1;

	int i = 0;
	int idx = 0;
	int rel = 0;
	while (i < ins->EIPeriod)
	{
		idx = ins->ringIdx - i;
		while (idx < 0)
		{
			idx += 60;
			if (idx < 0 || idx <= ins->ringIdx)
			{
				--rel;
			}
		}
		wgtSumOfSellTrade += (currentTradeSell - ins->tradeQtySellRing[idx]->relative(rel)) * exp(-i * ins->TQIDecayingParam);
		wgtSumOfBuyTrade += (currentTradeBuy - ins->tradeQtyBuyRing[idx]->relative(rel)) * exp(-i * ins->TQIDecayingParam);
		currentTradeSell = ins->tradeQtySellRing[idx]->relative(rel);
		currentTradeBuy = ins->tradeQtyBuyRing[idx]->relative(rel);
		++i;
	}
	if (wgtSumOfSellTrade < minexe)
	{
		wgtSumOfSellTrade = minexe;
	}
	if (wgtSumOfBuyTrade < minexe)
	{
		wgtSumOfBuyTrade = minexe;
	}
	return log(wgtSumOfSellTrade / wgtSumOfBuyTrade);
}
double OKExOptimizer::calcSkewWgtExecImbalance(OKExInstrument* ins)
{
	if (ins->ringDataCount <= ins->SkewWgtEIPeriod)
	{
		return 0;
	}
	double wgtSumOfAskExec = 0;
	double wgtSumOfBidExec = 0;
	double currentExecAsk = ins->skewWgtExecAsk;
	double currentExecBid = ins->skewWgtExecBid;
	double minexe = 1;

	int i = 0;
	int idx = 0;
	int rel = 0;
	while (i < ins->EIPeriod)
	{
		idx = ins->ringIdx - i;
		while (idx < 0)
		{
			idx += 60;
			if (idx < 0 || idx <= ins->ringIdx)
			{
				--rel;
			}
		}
		wgtSumOfAskExec += (currentExecAsk - ins->SkewWgtExecAskQtyRing[idx]->relative(rel)) * exp(-i * ins->SkewWgtEIDecayingParam);
		wgtSumOfBidExec += (currentExecBid - ins->SkewWgtExecBidQtyRing[idx]->relative(rel)) * exp(-i * ins->SkewWgtEIDecayingParam);
		currentExecAsk = ins->SkewWgtExecAskQtyRing[idx]->relative(rel);
		currentExecBid = ins->SkewWgtExecBidQtyRing[idx]->relative(rel);
		++i;
	}
	if (wgtSumOfAskExec < minexe)
	{
		wgtSumOfAskExec = minexe;
	}
	if (wgtSumOfBidExec < minexe)
	{
		wgtSumOfBidExec = minexe;
	}
	return log(wgtSumOfAskExec / wgtSumOfBidExec);
}

