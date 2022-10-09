#include "OKExInstrument.h"

book::book()
{
    side = OKExEnums::side::NONE;
    px = 0.0;
    sz = 0.0;
    szSell = 0.0;
    szBuy = 0.0;
    liqOrd = 0;
    numOfOrdBuy = 0;
    numOfOrdSell = 0;
    szMyOrd = 0.0;
    numOfMyOrd = 0;
    ts = 0;
}

book::book(const book& obj)
{
    side = obj.side;
    px = obj.px;
    sz = obj.sz;
    szSell = obj.szSell;
    szBuy = obj.szBuy;
    liqOrd = obj.liqOrd;
    numOfOrdBuy = 0;
    numOfOrdSell = 0;
    szMyOrd = 0;
    numOfMyOrd = 0;
    ts = obj.ts;
}

book::~book()
{
    side = OKExEnums::side::NONE;
    px = 0.0;
    sz = 0.0;
    szSell = 0.0;
    szBuy = 0.0;
    liqOrd = 0;
    numOfOrdBuy = 0;
    numOfOrdSell = 0;
    szMyOrd = 0.0;
    numOfMyOrd = 0;
    ts = 0;
}

void book::updateBook(OKExEnums::side sd, msgbook msg)
{
    px = msg.px;
    switch (sd)
    {
    case OKExEnums::BUY:
        szBuy = msg.sz;
        numOfOrdBuy = msg.numOfOrd;
        break;
    case OKExEnums::SELL:
        szSell = msg.sz;
        numOfOrdBuy = msg.numOfOrd;
        break;
    case OKExEnums::NONE:
    default:
        break;
    }
    if (szBuy - szSell > 0)
    {
        sz = szBuy - szSell;
        side = OKExEnums::side::BUY;
    }
    else if (szBuy - szSell < 0)
    {
        sz = szSell - szBuy;
        side = OKExEnums::side::SELL;
    }
    else
    {
        sz = 0;
        side = OKExEnums::side::NONE;
    }
    liqOrd = msg.liqOrd;
}

void book::init(void)
{
    side = OKExEnums::side::NONE;
    px = 0.0;
    sz = 0.0;
    szSell = 0.0;
    szBuy = 0.0;
    liqOrd = 0;
    numOfOrdBuy = 0;
    numOfOrdSell = 0;
    szMyOrd = 0.0;
    numOfMyOrd = 0;
    ts = 0;
}

void book::addOrder(OKExOrder* ord)
{
    ++numOfMyOrd;
    szMyOrd += ord->sz;
    liveOrders.emplace(ord->baseOrdId, ord);
}

OKExOrder* book::removeOrder(std::string baseOrdId)
{
    std::map<std::string, OKExOrder*>::iterator ordit;
    ordit = liveOrders.find(baseOrdId);
    if (ordit == liveOrders.end())
    {
        return nullptr;
    }
    else
    {
        OKExOrder* ord = ordit->second;
        --numOfMyOrd;
        szMyOrd -= ord->sz;
        liveOrders.erase(baseOrdId);
        return ord;
    }
}

OKExOrder* book::getTopOrder(void)
{
    std::map<std::string, OKExOrder*>::iterator it;
    std::map<std::string, OKExOrder*>::iterator itend = liveOrders.end();
    long long ts = 0;
    OKExOrder* ord = nullptr;
    for (it = liveOrders.begin(); it != itend; ++it)
    {
        if (ts == 0)
        {
            ts = it->second->orgTime;
            ord = it->second;
        }
        else if (ts > it->second->orgTime)
        {
            ts = it->second->orgTime;
            ord = it->second;
        }
    }
    return ord;
}
OKExOrder* book::updateOrder(dataOrder* tkt)//Return order object if the price has been changed
{
    std::map<std::string, OKExOrder*>::iterator ordit = liveOrders.find(tkt->clOrdId);
    OKExOrder* output = nullptr;
    if (ordit != liveOrders.end())
    {
        switch (ordit->second->status)
        {
        case OKExEnums::orderState::WAIT_NEW:
            ordit->second->status = OKExEnums::orderState::LIVE;
            ordit->second->live = true;
            break;
        case OKExEnums::orderState::WAIT_AMD:
            ordit->second->sz = ordit->second->newSz;
            ordit->second->openSz = ordit->second->sz - ordit->second->execSz;
            if (ordit->second->openSz <= 0)
            {
                ordit->second->openSz = 0;
                ordit->second->live = false;
                if (ordit->second->execSz > 0)
                {
                    ordit->second->status = OKExEnums::orderState::FILLED;
                }
                else
                {
                    ordit->second->status = OKExEnums::orderState::CANCELED;
                }
                liveOrders.erase(ordit->first);
            }
            else
            {
                if (ordit->second->execSz > 0)
                {
                    ordit->second->status = OKExEnums::orderState::PARTIALLY_FILLED;
                }
                else
                {
                    ordit->second->status = OKExEnums::orderState::LIVE;
                }
                if (ordit->second->px != ordit->second->newPx)
                {
                    ordit->second->px = ordit->second->newPx;
                    output = ordit->second;
                    liveOrders.erase(ordit->first);
                }
            }
            break;
        case OKExEnums::orderState::WAIT_CAN:
            ordit->second->sz = ordit->second->execSz;
            ordit->second->openSz = 0;
            ordit->second->live = false;
            if (ordit->second->execSz > 0)
            {
                ordit->second->status = OKExEnums::orderState::FILLED;
            }
            else
            {
                ordit->second->status = OKExEnums::orderState::CANCELED;
            }
            liveOrders.erase(ordit->first);
            break;
        default:
            break;
        }
    }
    return output;
}
bool book::executeOrder(dataOrder* trd)
{
    std::map<std::string, OKExOrder*>::iterator ordit = liveOrders.find(trd->clOrdId);
    if (ordit != liveOrders.end())
    {
        ordit->second->lastSz = trd->fillSz;
        ordit->second->execSz += trd->fillSz;
        ordit->second->openSz -= trd->fillSz;
        ordit->second->lastPx = trd->px;
        ordit->second->avgPx = trd->avgPx;
        switch (ordit->second->status)
        {
        case OKExEnums::orderState::WAIT_NEW:
        case OKExEnums::orderState::WAIT_AMD:
        case OKExEnums::orderState::WAIT_CAN:
            if (trd->state == OKExEnums::orderState::FILLED)
            {
                ordit->second->status = trd->state;
            }
            break;
        default:
            ordit->second->status = trd->state;
            break;
        }
        
        if (ordit->second->status == OKExEnums::orderState::CANCELED || ordit->second->status == OKExEnums::orderState::FILLED)
        {
            liveOrders.erase(ordit->first);
            return true;
        }
    }
    return false;
}

void OKExInstrument::setInstrumentData(std::map<std::string, std::string> mp)
{
	instId = mp["instId"];

    if (mp["instType"] == "SPOT")
    {
        instType = OKExEnums::instType::SPOT;
    }
    else if (mp["instType"] == "SWAP")
    {
        instType = OKExEnums::instType::SWAP;
    }
    else if (mp["instType"] == "FUTURES")
    {
        instType = OKExEnums::instType::FUTURES;
    }
    else if (mp["instType"] == "MARGIN")
    {
        instType = OKExEnums::instType::MARGIN;
    }
    else
    {
        instType = OKExEnums::instType::NONE;
    }

    baseCcy = mp["baseCcy"];
    quoteCcy = mp["quoteCcy"];
    settleCcy = mp["settleCcy"];
    uly = mp["uly"];
    
    if (mp["category"] != "")
    {
        category = stoi(mp["category"]);
    }
    if (mp["ctMult"] != "")
    {
        ctMulti = stoi(mp["ctMult"]);
    }
    if (mp["ctType"] == "inverse")
    {
        ctType = OKExEnums::ctType::INVERSE;
    }
    else
    {
        ctType = OKExEnums::ctType::LINEAR;
    }
    if (mp["ctVal"] != "")
    {
        ctVal = stod(mp["ctVal"]);
    }
    else
    {
        ctVal = 1.0;
    }
    ctValCcy = mp["ctValCcy"];
    if (mp["expTime"] != "")
    {
        expTime = stoll(mp["expTime"]);
    }
    if (mp["lever"] != "")
    {
        lever = stod(mp["lever"]);
    }
    if (mp["listTime"] != "")
    {
        listTime = stoll(mp["listTime"]);
    }
    if (mp["lotSz"] != "")
    {
        lotSz = stod(mp["lotSz"]);
    }
    if (mp["maxIcebergSz"] != "")
    {
        maxIcebergSz = stod(mp["maxIcebergSz"]);
    }
    if (mp["maxLmtSz"] != "")
    {
        maxLmtSz = stod(mp["maxLmtSz"]);
    }
    if (mp["maxMktSz"] != "")
    {
        maxMktSz = stod(mp["maxMktSz"]);
    }
    if (mp["maxStopSz"] != "")
    {
        maxStopSz = stod(mp["maxStopSz"]);
    }
    if (mp["maxTriggerSz"] != "")
    {
        maxTriggerSz = stod(mp["maxTriggerSz"]);
    }
    if (mp["maxTwapSz"] != "")
    {
        maxTwapSz = stod(mp["maxTwapSz"]);
    }
    if (mp["minSz"] != "")
    {
        minSz = stod(mp["minSz"]);
    }
    if (mp["state"] == "live")
    {
        state = OKExEnums::insState::LIVE;
    }
    else if (mp["state"] == "suspend")
    {
        state = OKExEnums::insState::SUSPEND;
    }
    else if (mp["state"] == "expired")
    {
        state = OKExEnums::insState::EXPIRED;
    }
    else if (mp["state"] == "preopen")
    {
        state = OKExEnums::insState::PREOPEN;
    }
    else if (mp["state"] == "settlement")
    {
        state = OKExEnums::insState::SETTLEMENT;
    }
    else
    {
        state = OKExEnums::insState::NONE;
    }
    if (mp["tickSz"] != "")
    {
        tickSz = stod(mp["tickSz"]);
    }
}


void OKExInstrument::setParams(std::list<std::string> params)
{

}
void OKExInstrument::updateTrade(OKExMktMsg* msg)
{
    std::list<dataTrades>::iterator it;
    std::list<dataTrades>::iterator itend = msg->trades.end();
    for (it = msg->trades.begin(); it != itend; ++it)
    {
        if (it->ts > ts)
        {
            ts = it->ts;
        }
        if (last > 0 && last != it->px)
        {
            realizedVolatility += pow(log(last / it->px),2);
        }
        last = it->px;
        if (ctType == OKExEnums::INVERSE)
        {
            if (it->side == OKExEnums::side::BUY)//this means the market order is BUY
            {
                ++execAskCnt;
                execAskVol = it->sz * ctVal / it->px;
                execAskAmt = it->sz * ctVal;
            }
            else if (it->side == OKExEnums::side::SELL)//this means the market order is SELL
            {
                ++execBidCnt;
                execBidVol = it->sz * ctVal / it->px;
                execBidAmt = it->sz * ctVal;
            }
        }
        else
        {
            if (it->side == OKExEnums::side::BUY)//this means the market order is BUY
            {
                ++execAskCnt;
                execAskVol = it->sz * ctVal;
                execAskAmt = it->sz * ctVal * it->px;
            }
            else if (it->side == OKExEnums::side::SELL)//this means the market order is SELL
            {
                ++execBidCnt;
                execBidVol = it->sz * ctVal;
                execBidAmt = it->sz * ctVal * it->px;
            }
        }
    }
}

void OKExInstrument::initializeBooks(OKExMktMsg* msg, int depth)
{
    books.clear();
    bookDepth = depth;

    std::list<dataBooks>::iterator it;
    std::list<dataBooks>::iterator itend = msg->books.end();
    std::list<msgbook>::iterator msgbookit;
    std::list<msgbook>::iterator msgbookitend;

    std::map<int, book> bids;
    std::map<int, book> asks;
    std::map<int, book>::iterator bidsit;
    std::map<int, book>::iterator asksit;
    std::map<int, book>::iterator bidsitend;
    std::map<int, book>::iterator asksitend;
    int tempbestask = 0;
    int tempbestbid = 0;

    for (it = msg->books.begin(); it != itend; ++it)
    {
        if (it->ts > ts)
        {
            ts = it->ts;
        }
        msgbookitend = it->bids.end();
        for (msgbookit = it->bids.begin(); msgbookit != msgbookitend; ++msgbookit)
        {
            if (ctType == OKExEnums::ctType::INVERSE)
            {
                msgbookit->sz *= (double)ctVal / msgbookit->px;
            }
            else
            {
                msgbookit->sz *= (double)ctVal;
            }
            book bk;
            bk.updateBook(OKExEnums::side::BUY, *msgbookit);
            bids[(int)(msgbookit->px * priceUnit)] = bk;
            if (tempbestbid == 0 || tempbestbid < (int)(msgbookit->px * priceUnit))
            {
                tempbestbid = (int)(msgbookit->px * priceUnit);
            }
        }
        msgbookitend = it->asks.end();
        for (msgbookit = it->asks.begin(); msgbookit != msgbookitend; ++msgbookit)
        {
            if (ctType == OKExEnums::ctType::INVERSE)
            {
                msgbookit->sz *= (double)ctVal / msgbookit->px;
            }
            else
            {
                msgbookit->sz *= (double)ctVal;
            }
            book bk;
            bk.updateBook(OKExEnums::side::SELL, *msgbookit);
            asks[(int)(msgbookit->px * priceUnit)] = bk;
            if (tempbestask == 0 || tempbestask > (int)(msgbookit->px * priceUnit))
            {
                tempbestask = (int)(msgbookit->px * priceUnit);
            }
        }
    }
    int mid;
    if (tempbestask > 0 && tempbestbid > 0)
    {
        mid = (int)(round((tempbestask + tempbestbid) / 2 / tickSz / priceUnit) * tickSz * priceUnit);
    }
    else if (tempbestask > 0)
    {
        mid = tempbestask;
    }
    else if (tempbestbid > 0)
    {
        mid = tempbestbid;
    }
    else
    {
        mid = 0;
    }

    int i = 0;
    int tick = (int)(tickSz * priceUnit);
    int askpr = mid;
    int bidpr = mid;
    book bk;
    bidsitend = bids.end();
    asksitend = asks.end();
    bidsit = bids.find(mid);
    asksit = asks.find(mid);
    if (bidsit != bidsitend)
    {
        books.emplace(mid, bidsit->second);
    }
    else if (asksit != asksitend)
    {
        books.emplace(mid, asksit->second);
    }
    else
    {
        bk.px = mid;
        books.emplace(mid, bk);
    }


    while (i < bookDepth)
    {
        bidpr -= tick;
        askpr += tick;
        bidsit = bids.find(bidpr);
        asksit = asks.find(askpr);
        if (bidsit != bidsitend)
        {
            books.emplace(bidpr, bidsit->second);
        }
        else
        {
            bk.px = bidpr;
            books.emplace(bidpr, bk);
        }
        if (asksit != asksitend)
        {
            books.emplace(askpr, asksit->second);
        }
        else
        {
            bk.px = askpr;
            books.emplace(askpr, bk);
        }
        ++i;
    }
}

std::map<int, book>::iterator OKExInstrument::findBest(int pr, OKExEnums::side side)
{
    std::map<int, book>::iterator it = books.find(pr);
    std::map<int, book>::iterator itend = books.end();
    std::map<int, book>::iterator itbegin = books.begin();
    if (it == itend)
    {
        return itend;
    }
    else
    {
        switch (side)
        {
        case OKExEnums::BUY:
            while (it != itbegin)
            {
                --it;
                if (it->second.sz > 0 && it->second.side == side)
                {
                    return it;
                }
            }
            break;
        case OKExEnums::SELL:
            while (it != itend)
            {
                ++it;
                if (it == itend)
                {
                    return itend;
                }
                else if (it->second.sz > 0 && it->second.side == side)
                {
                    return it;
                }
            }
            break;
        case OKExEnums::NONE:
        default:
            break;
        }
    }
    return itend;
}

bool OKExInstrument::updateBooks(OKExMktMsg* msg)
{
    std::list<dataBooks>::iterator it;
    std::list<dataBooks>::iterator itend = msg->books.end();
    std::list<msgbook>::iterator bookit;
    std::list<msgbook>::iterator bookitend;
    std::map<int, book>::iterator thisbookend = books.end();
    std::map<int, book>::iterator bk;
    OKExEnums::side orgSide;
    std::map<int, book>::iterator temp_bid;
    std::map<int, book>::iterator temp_ask;

    for (it = msg->books.begin(); it != itend; ++it)
    {
        if (it->ts > ts)
        {
            ts = it->ts;
        }
        bookitend = it->bids.end();
        for (bookit = it->bids.begin(); bookit != bookitend; ++bookit)
        {
            if (ctType == OKExEnums::ctType::INVERSE)
            {
                bookit->sz *= (double)ctVal / bookit->px;
            }
            else
            {
                bookit->sz *= (double)ctVal;
            }
            bk = books.find((int)(bookit->px * priceUnit));
            if (bk != thisbookend)
            {
                orgSide = bk->second.side;
                bk->second.updateBook(OKExEnums::side::BUY,*bookit);
                if (bk->second.side == OKExEnums::side::BUY)
                {
                    if (bk->second.sz > 0 && bk->first > bestBid->first)
                    {
                        bestBid = bk;
                    }
                    else if (bk->first == bestBid->first && bk->second.sz == 0)
                    {
                        temp_bid = findBest(bk->first, OKExEnums::side::BUY);
                        if (temp_bid != books.end())
                        {
                            bestBid = temp_bid;
                        }
                        else
                        {

                        }
                    }
                }
                else
                {
                    temp_bid = findBest(bestBid->first, OKExEnums::side::BUY);
                    if (temp_bid != books.end())
                    {
                        bestBid = temp_bid;
                    }
                    else
                    {
                        //?
                    }
                }
                if (orgSide != bk->second.side && bk->first >= bestBid->first)
                {
                    temp_ask = findBest(bestAsk->first, OKExEnums::side::SELL);
                    if (temp_ask != books.end())
                    {
                        bestAsk = temp_ask;
                    }
                }
                //Reshape books
            }
        }
        bookitend = it->asks.end();
        for (bookit = it->asks.begin(); bookit != bookitend; ++bookit)
        {
            if (ctType == OKExEnums::ctType::INVERSE)
            {
                bookit->sz *= (double)ctVal / bookit->px;
            }
            else
            {
                bookit->sz *= (double)ctVal;
            }
            bk = books.find((int)(bookit->px * priceUnit));
            if (bk != thisbookend)
            {
                orgSide = bk->second.side;
                bk->second.updateBook(OKExEnums::side::SELL, *bookit);
                if (bk->second.side == OKExEnums::side::SELL)
                {
                    if (bk->second.sz > 0 && bk->first < bestAsk->first)
                    {
                        bestAsk = bk;
                    }
                    else if (bk->first == bestAsk->first && bk->second.sz == 0)
                    {
                        temp_ask = findBest(bk->first, OKExEnums::side::SELL);
                        if (temp_ask != books.end())
                        {
                            bestAsk = temp_ask;
                        }
                        else
                        {

                        }
                    }
                }
                else
                {
                    temp_ask = findBest(bestAsk->first, OKExEnums::side::SELL);
                    if (temp_ask != books.end())
                    {
                        bestAsk = temp_ask;
                    }
                    else
                    {
                        //?
                    }
                }
                if (orgSide != bk->second.side && bk->first <= bestAsk->first)
                {
                    temp_bid = findBest(bestAsk->first, OKExEnums::side::BUY);
                    if (temp_bid != books.end())
                    {
                        bestBid = temp_bid;
                    }
                }
                //Reshape books
            }
        }
    }
}

bool OKExInstrument::reflectMsg(OKExMktMsg* msg)
{
    bool blOptimize = false;
    if (msg->args["channel"] == "books")
    {
        if (msg->args["action"] == "snapshot")
        {
            initializeBooks(msg,bookDepth);
        }
        else
        {
            blOptimize = updateBooks(msg);
        }
    }
    else if (msg->args["channel"] == "trades")
    {
        updateTrade(msg);
        blOptimize = true;
    }
    //calcMid
    return blOptimize;
}
void OKExInstrument::updateOrders(dataOrder* dtord)
{
    //Find the order and update it.
    //Call book->updateOrder if it's an ack of the order
    //Call book->executeOrder if it's an execution.
    //Update Variables on Instrument class and Position Class.
    std::map<std::string, OKExOrder*>::iterator ordit = liveOrdList->find(dtord->clOrdId);
    book bk;
    book newbk;
    OKExOrder* neword;
    if (ordit != liveOrdList->end())
    {
        bk = books.at(ordit->second->px);
        if (dtord->fillSz > 0)//Execution
        {
            bk.executeOrder(dtord);
            if (dtord->side == OKExEnums::side::BUY)
            {
                ++tradedCntBuy;
                tradedQtyBuy += dtord->fillSz;
                tradedAmtBuy += dtord->fillSz * dtord->fillPx;
            }
            else if (dtord->side == OKExEnums::side::SELL)
            {
                ++tradedCntSell;
                tradedQtySell += dtord->fillSz;
                tradedAmtSell += dtord->fillSz * dtord->fillPx;
            }
        }
        else
        {
            neword = bk.updateOrder(dtord);
            if (neword)
            {
                newbk = books.at(neword->px);
                newbk.addOrder(neword);
                
            }
        }
        if (ordit->second->status == OKExEnums::orderState::CANCELED || ordit->second->status == OKExEnums::orderState::FILLED)
        {
            liveOrdList->erase(ordit->first);
        }
    }
}