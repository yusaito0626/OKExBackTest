#include "OKExOptimizer.h"

OKExOptimizer* optimizer = OKExOptimizer::getInstance();

void OKExOptimizer::calcFactors(OKExInstrument* ins)
{

}
void OKExOptimizer::updateRings(OKExInstrument* ins)
{

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
double OKExOptimizer::calcSkewWgtQtyImbalance(OKExInstrument* ins)
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
