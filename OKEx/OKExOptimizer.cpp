#include "OKExOptimizer.h"

OKExOptimizer* optimizer = OKExOptimizer::getInstance();


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

}
double OKExOptimizer::calcTradeQtyImbalance(OKExInstrument* ins)
{

}
double OKExOptimizer::calcSkewWgtQtyImbalance(OKExInstrument* ins)
{

}
