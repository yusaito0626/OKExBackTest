#include "OKExInstrument.h"

book::book()
{
    side = OKExEnums::side::_NONE;
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
    side = OKExEnums::side::_NONE;
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

void book::updateBook(OKExEnums::side sd, msgbook* msg)
{
    px = msg->px;
    switch (sd)
    {
    case OKExEnums::side::_BUY:
        szBuy = msg->sz;
        numOfOrdBuy = msg->numOfOrd;
        break;
    case OKExEnums::side::_SELL:
        szSell = msg->sz;
        numOfOrdBuy = msg->numOfOrd;
        break;
    case OKExEnums::side::_NONE:
    default:
        break;
    }
    if (szBuy - szSell > 0)
    {
        sz = szBuy - szSell;
        side = OKExEnums::side::_BUY;
    }
    else if (szBuy - szSell < 0)
    {
        sz = szSell - szBuy;
        side = OKExEnums::side::_SELL;
    }
    else
    {
        sz = 0;
        side = OKExEnums::side::_NONE;
    }
    liqOrd = msg->liqOrd;
}

void book::init(void)
{
    side = OKExEnums::side::_NONE;
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
    liveOrders->emplace(ord->baseOrdId, ord);
}

OKExOrder* book::removeOrder(std::string baseOrdId)
{
    std::map<std::string, OKExOrder*>::iterator ordit;
    ordit = liveOrders->find(baseOrdId);
    if (ordit == liveOrders->end())
    {
        return nullptr;
    }
    else
    {
        OKExOrder* ord = ordit->second;
        --numOfMyOrd;
        szMyOrd -= ord->sz;
        liveOrders->erase(baseOrdId);
        return ord;
    }
}

OKExOrder* book::getTopOrder(void)
{
    std::map<std::string, OKExOrder*>::iterator it;
    std::map<std::string, OKExOrder*>::iterator itend = liveOrders->end();
    long long ts = 0;
    OKExOrder* ord = nullptr;
    for (it = liveOrders->begin(); it != itend; ++it)
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
    std::map<std::string, OKExOrder*>::iterator ordit = liveOrders->find(tkt->clOrdId);
    OKExOrder* output = nullptr;
    if (ordit != liveOrders->end())
    {
        switch (ordit->second->status)
        {
        case OKExEnums::orderState::_WAIT_NEW:
            ordit->second->status = OKExEnums::orderState::_LIVE;
            ordit->second->live = true;
            break;
        case OKExEnums::orderState::_WAIT_AMD:
            ordit->second->sz = ordit->second->newSz;
            ordit->second->openSz = ordit->second->sz - ordit->second->execSz;
            if (ordit->second->openSz <= 0)
            {
                ordit->second->openSz = 0;
                ordit->second->live = false;
                if (ordit->second->execSz > 0)
                {
                    ordit->second->status = OKExEnums::orderState::_FILLED;
                }
                else
                {
                    ordit->second->status = OKExEnums::orderState::_CANCELED;
                }
                liveOrders->erase(ordit->first);
            }
            else
            {
                if (ordit->second->execSz > 0)
                {
                    ordit->second->status = OKExEnums::orderState::_PARTIALLY_FILLED;
                }
                else
                {
                    ordit->second->status = OKExEnums::orderState::_LIVE;
                }
                if (ordit->second->px != ordit->second->newPx)
                {
                    ordit->second->px = ordit->second->newPx;
                    output = ordit->second;
                    liveOrders->erase(ordit->first);
                }
            }
            break;
        case OKExEnums::orderState::_WAIT_CAN:
            ordit->second->sz = ordit->second->execSz;
            ordit->second->openSz = 0;
            ordit->second->live = false;
            if (ordit->second->execSz > 0)
            {
                ordit->second->status = OKExEnums::orderState::_FILLED;
            }
            else
            {
                ordit->second->status = OKExEnums::orderState::_CANCELED;
            }
            liveOrders->erase(ordit->first);
            break;
        default:
            break;
        }
    }
    return output;
}
bool book::executeOrder(dataOrder* trd)
{
    std::map<std::string, OKExOrder*>::iterator ordit = liveOrders->find(trd->clOrdId);
    if (ordit != liveOrders->end())
    {
        ordit->second->lastSz = trd->fillSz;
        ordit->second->execSz += trd->fillSz;
        ordit->second->openSz -= trd->fillSz;
        ordit->second->lastPx = trd->px;
        ordit->second->avgPx = trd->avgPx;
        switch (ordit->second->status)
        {
        case OKExEnums::orderState::_WAIT_NEW:
        case OKExEnums::orderState::_WAIT_AMD:
        case OKExEnums::orderState::_WAIT_CAN:
            if (trd->state == OKExEnums::orderState::_FILLED)
            {
                ordit->second->status = trd->state;
            }
            break;
        default:
            ordit->second->status = trd->state;
            break;
        }
        
        if (ordit->second->status == OKExEnums::orderState::_CANCELED || ordit->second->status == OKExEnums::orderState::_FILLED)
        {
            liveOrders->erase(ordit->first);
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
        instType = OKExEnums::instType::_SPOT;
    }
    else if (mp["instType"] == "SWAP")
    {
        instType = OKExEnums::instType::_SWAP;
    }
    else if (mp["instType"] == "FUTURES")
    {
        instType = OKExEnums::instType::_FUTURES;
    }
    else if (mp["instType"] == "MARGIN")
    {
        instType = OKExEnums::instType::_MARGIN;
    }
    else
    {
        instType = OKExEnums::instType::_NONE;
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
        ctType = OKExEnums::ctType::_INVERSE;
    }
    else
    {
        ctType = OKExEnums::ctType::_LINEAR;
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
        state = OKExEnums::insState::_LIVE;
    }
    else if (mp["state"] == "suspend")
    {
        state = OKExEnums::insState::_SUSPEND;
    }
    else if (mp["state"] == "expired")
    {
        state = OKExEnums::insState::_EXPIRED;
    }
    else if (mp["state"] == "preopen")
    {
        state = OKExEnums::insState::_PREOPEN;
    }
    else if (mp["state"] == "settlement")
    {
        state = OKExEnums::insState::_SETTLEMENT;
    }
    else
    {
        state = OKExEnums::insState::_NONE;
    }
    if (mp["tickSz"] != "")
    {
        tickSz = stod(mp["tickSz"]);
        priceUnit = (int)(1 / tickSz);
    }
}

position::position()
{
    instType = OKExEnums::instType::_NONE;
    trdMode = OKExEnums::tradeMode::_NONE;
    posSide = OKExEnums::posSide::_NONE;
    pos = 0.0;
    baseBal = 0.0;
    quoteBal = 0.0;
    posCcy = 0.0;
    posId = 0.0;
    availPos = 0.0;
    avgPx = 0.0;
    upl = 0.0;
    uplRatio = 0.0;

    instId = "";
    tradeId = "";
    lever = 0.0;
    liqPx = 0.0;
    markPx = 0.0;
    imr = 0.0;
    margin = 0.0;
    mgnRatio = 0.0;
    mmr = 0.0;
    liab = 0.0;
    liabCcy = 0.0;
    interest = 0.0;
    notionalUsd = 0.0;
    optVal = 0.0;
    adl = 0.0;
    ccy = "";
    last = 0.0;
    usdPx = 0.0;
    deltaBS = 0.0;
    deltaPA = 0.0;
    gammaBS = 0.0;
    gammaPA = 0.0;
    thetaBS = 0.0;
    thetaPA = 0.0;
    vegaBS = 0.0;
    vegaPA = 0.0;
    cTime = 0;
    uTime = 0;
    pTime = 0;
}
position::~position()
{

}
void position::init(void)
{
    instType = OKExEnums::instType::_NONE;
    trdMode = OKExEnums::tradeMode::_NONE;
    posSide = OKExEnums::posSide::_NONE;
    pos = 0.0;
    baseBal = 0.0;
    quoteBal = 0.0;
    posCcy = 0.0;
    posId = 0.0;
    availPos = 0.0;
    avgPx = 0.0;
    upl = 0.0;
    uplRatio = 0.0;

    instId = "";
    tradeId = "";
    lever = 0.0;
    liqPx = 0.0;
    markPx = 0.0;
    imr = 0.0;
    margin = 0.0;
    mgnRatio = 0.0;
    mmr = 0.0;
    liab = 0.0;
    liabCcy = 0.0;
    interest = 0.0;
    notionalUsd = 0.0;
    optVal = 0.0;
    adl = 0.0;
    ccy = "";
    last = 0.0;
    usdPx = 0.0;
    deltaBS = 0.0;
    deltaPA = 0.0;
    gammaBS = 0.0;
    gammaPA = 0.0;
    thetaBS = 0.0;
    thetaPA = 0.0;
    vegaBS = 0.0;
    vegaPA = 0.0;
    cTime = 0;
    uTime = 0;
    pTime = 0;
}



OKExInstrument::OKExInstrument()
{
    instId = "";
    instType = OKExEnums::instType::_NONE;
    std::string baseCcy = "";
    std::string quoteCcy = "";
    std::string settleCcy = "";
    std::string uly = "";
    category = 0;
    ctMulti = 0;
    ctType = OKExEnums::ctType::_NONE;
    ctVal = 0;
    ctValCcy = "";
    expTime = 0;
    lever = 0;
    listTime = 0;
    lotSz = 0.0;
    maxIcebergSz = 0;
    maxLmtSz = 0;
    maxMktSz = 0;
    maxStopSz = 0;
    maxTriggerSz = 0;
    maxTwapSz = 0;
    minSz = 0.0;
    state = OKExEnums::insState::_NONE;
    tickSz = 0.0;
    priceUnit = 0;

    isTrading = false;

    bookDepth = 1000;
    books = new std::map<int, book*>();
    bestAsk = books->end();
    bestBid = books->end();
    netPosition = 0.0;

    ordList = new std::map<std::string, OKExOrder*>();
    liveOrdList = new std::map<std::string, OKExOrder*>();

    tradedCntBuy = 0;
    tradedCntSell = 0;
    tradedQtyBuy = 0;
    tradedQtySell = 0;
    tradedAmtBuy = 0;
    tradedAmtSell = 0;

    last = 0.0;
    mid = 0.0;
    open = 0.0;
    high = 0.0;
    low = 0.0;

    ts = 0;
    lastOptTs = 0;

    //Factors
    bookImbalance = 0.0;
    currentRV = 0.0;
    exeImbalance = 0.0;
    sret = 0.0;
    currentTradedQtyBuy = 0;
    currentTradedQtySell = 0;

    realizedVolatility = 0.0;
    execAskCnt = 0;
    execBidCnt = 0;
    execAskVol = 0.0;
    execBidVol = 0.0;
    execAskAmt = 0.0;
    execBidAmt = 0.0;
}
OKExInstrument::~OKExInstrument()
{

}

void OKExInstrument::setParams(std::list<std::string> params)
{

}
void OKExInstrument::updateTrade(OKExMktMsg* msg)
{
    std::list<dataTrades*>::iterator it;
    std::list<dataTrades*>::iterator itend = msg->trades->end();
    for (it = msg->trades->begin(); it != itend; ++it)
    {
        if ((*it)->ts > 0)
        {
            if ((*it)->ts > ts)
            {
                ts = (*it)->ts;
            }
            if (last > 0 && last != (*it)->px)
            {
                realizedVolatility += pow(log(last / (*it)->px), 2);
            }
            last = (*it)->px;
            if (ctType == OKExEnums::ctType::_INVERSE)
            {
                if ((*it)->side == OKExEnums::side::_BUY)//this means the market order is BUY
                {
                    ++execAskCnt;
                    execAskVol = (*it)->sz * ctVal / (*it)->px;
                    execAskAmt = (*it)->sz * ctVal;
                }
                else if ((*it)->side == OKExEnums::side::_SELL)//this means the market order is SELL
                {
                    ++execBidCnt;
                    execBidVol = (*it)->sz * ctVal / (*it)->px;
                    execBidAmt = (*it)->sz * ctVal;
                }
            }
            else
            {
                if ((*it)->side == OKExEnums::side::_BUY)//this means the market order is BUY
                {
                    ++execAskCnt;
                    execAskVol = (*it)->sz * ctVal;
                    execAskAmt = (*it)->sz * ctVal * (*it)->px;
                }
                else if ((*it)->side == OKExEnums::side::_SELL)//this means the market order is SELL
                {
                    ++execBidCnt;
                    execBidVol = (*it)->sz * ctVal;
                    execBidAmt = (*it)->sz * ctVal * (*it)->px;
                }
            }
            (*it)->init();
        }
        else
        {
            break;
        }
        
    }
}

void OKExInstrument::initializeBooks(OKExMktMsg* msg, int depth)
{
    books->clear();
    bookDepth = depth;

    std::list<dataBooks*>::iterator it;
    std::list<dataBooks*>::iterator itend = msg->books->end();
    std::list<msgbook*>::iterator msgbookit;
    std::list<msgbook*>::iterator msgbookitend;

    std::map<int, book*> bids;
    std::map<int, book*> asks;
    std::map<int, book*>::iterator bidsit;
    std::map<int, book*>::iterator asksit;
    std::map<int, book*>::iterator bidsitend;
    std::map<int, book*>::iterator asksitend;
    int tempbestask = 0;
    int tempbestbid = 0;

    for (it = msg->books->begin(); it != itend; ++it)
    {
        if ((*it)->ts > 0)
        {
            if ((*it)->ts > ts)
            {
                ts = (*it)->ts;
            }
            msgbookitend = (*it)->bids->end();
            for (msgbookit = (*it)->bids->begin(); msgbookit != msgbookitend; ++msgbookit)
            {
                if ((*msgbookit)->px > 0)
                {
                    if (ctType == OKExEnums::ctType::_INVERSE)
                    {
                        (*msgbookit)->sz *= (double)ctVal / (*msgbookit)->px;
                    }
                    else
                    {
                        (*msgbookit)->sz *= (double)ctVal;
                    }
                    book* bk = new book();
                    bk->updateBook(OKExEnums::side::_BUY, *msgbookit);
                    bids.emplace((int)((*msgbookit)->px * priceUnit), bk);
                    if (tempbestbid == 0 || tempbestbid < (int)((*msgbookit)->px * priceUnit))
                    {
                        tempbestbid = (int)((*msgbookit)->px * priceUnit);
                    }
                    (*msgbookit)->init();
                }
                else
                {
                    break;
                }
            }
            msgbookitend = (*it)->asks->end();
            for (msgbookit = (*it)->asks->begin(); msgbookit != msgbookitend; ++msgbookit)
            {
                if ((*msgbookit)->px > 0)
                {
                    if (ctType == OKExEnums::ctType::_INVERSE)
                    {
                        (*msgbookit)->sz *= (double)ctVal / (*msgbookit)->px;
                    }
                    else
                    {
                        (*msgbookit)->sz *= (double)ctVal;
                    }
                    book* bk = new book();
                    bk->updateBook(OKExEnums::side::_SELL, *msgbookit);
                    asks.emplace((int)((*msgbookit)->px * priceUnit), bk);
                    if (tempbestask == 0 || tempbestask > (int)((*msgbookit)->px * priceUnit))
                    {
                        tempbestask = (int)((*msgbookit)->px * priceUnit);
                    }
                    (*msgbookit)->init();
                }
                else
                {
                    break;
                }
            }
            (*it)->init();
        }
        else
        {
            break;
        }
    }
    if (tempbestask > 0 && tempbestbid > 0)
    {
        mid = (tempbestask + tempbestbid) / 2;
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
    int askpr = tempbestask;
    int bidpr = tempbestbid;
    bidsitend = bids.end();
    asksitend = asks.end();

    int temppr = tempbestbid + tick;
    while (temppr < tempbestask)
    {
        book* bk = new book();
        bk->init();
        bk->px = temppr;
        books->emplace(bk->px, bk);
    }

    while (i < bookDepth)
    {
        bidsit = bids.find(bidpr);
        asksit = asks.find(askpr);
        if (bidsit != bidsitend)
        {
            books->emplace(bidpr, bidsit->second);
        }
        else
        {
            book* bk = new book();
            bk->px = bidpr;
            books->emplace(bidpr, bk);
        }
        if (asksit != asksitend)
        {
            books->emplace(askpr, asksit->second);
        }
        else
        {
            book* bk = new book();
            bk->px = askpr;
            books->emplace(askpr, bk);
        }
        bidpr -= tick;
        askpr += tick;
        ++i;
    }
    if (tempbestask > 0)
    {
        bestAsk = books->find(tempbestask);
    }
    if (tempbestbid > 0)
    {
        bestBid = books->find(tempbestbid);
    }
}

std::map<int, book*>::iterator OKExInstrument::findBest(int pr, OKExEnums::side side)
{
    std::map<int, book*>::iterator it = books->find(pr);
    std::map<int, book*>::iterator itend = books->end();
    std::map<int, book*>::iterator itbegin = books->begin();
    if (it == itend)
    {
        return itend;
    }
    else
    {
        switch (side)
        {
        case OKExEnums::side::_BUY:
            while (it != itbegin)
            {
                --it;
                if (it->second->sz > 0 && it->second->side == side)
                {
                    return it;
                }
            }
            break;
        case OKExEnums::side::_SELL:
            while (it != itend)
            {
                ++it;
                if (it == itend)
                {
                    return itend;
                }
                else if (it->second->sz > 0 && it->second->side == side)
                {
                    return it;
                }
            }
            break;
        case OKExEnums::side::_NONE:
        default:
            break;
        }
    }
    return itend;
}

bool OKExInstrument::updateBooks(OKExMktMsg* msg)
{
    std::list<dataBooks*>::iterator it;
    std::list<dataBooks*>::iterator itend = msg->books->end();
    std::list<msgbook*>::iterator bookit;
    std::list<msgbook*>::iterator bookitend;
    std::map<int, book*>::iterator thisbookend = books->end();
    std::map<int, book*>::iterator bk;
    OKExEnums::side orgSide;
    std::map<int, book*>::iterator temp_bid;
    std::map<int, book*>::iterator temp_ask;

    for (it = msg->books->begin(); it != itend; ++it)
    {
        if ((*it)->ts > 0)
        {
            if ((*it)->ts > ts)
            {
                ts = (*it)->ts;
            }

            bookitend = (*it)->bids->end();
            for (bookit = (*it)->bids->begin(); bookit != bookitend; ++bookit)
            {
                if (ctType == OKExEnums::ctType::_INVERSE)
                {
                    (*bookit)->sz *= (double)ctVal / (*bookit)->px;
                }
                else
                {
                    (*bookit)->sz *= (double)ctVal;
                }
                bk = books->find((int)((*bookit)->px * priceUnit));
                if (bk != thisbookend)
                {
                    orgSide = bk->second->side;
                    bk->second->updateBook(OKExEnums::side::_BUY, *bookit);
                    if (bk->second->side == OKExEnums::side::_BUY)
                    {
                        if (bk->second->sz > 0 && bk->first > bestBid->first)
                        {
                            bestBid = bk;
                        }
                        else if (bk->first == bestBid->first && bk->second->sz == 0)
                        {
                            temp_bid = findBest(bk->first, OKExEnums::side::_BUY);
                            if (temp_bid != books->end())
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
                        temp_bid = findBest(bestBid->first, OKExEnums::side::_BUY);
                        if (temp_bid != books->end())
                        {
                            bestBid = temp_bid;
                        }
                        else
                        {
                            //?
                        }
                    }
                    if (orgSide != bk->second->side && bk->first >= bestBid->first)
                    {
                        temp_ask = findBest(bestAsk->first, OKExEnums::side::_SELL);
                        if (temp_ask != books->end())
                        {
                            bestAsk = temp_ask;
                        }
                    }
                    //Reshape books
                }
            }
            bookitend = (*it)->asks->end();
            for (bookit = (*it)->asks->begin(); bookit != bookitend; ++bookit)
            {
                if (ctType == OKExEnums::ctType::_INVERSE)
                {
                    (*bookit)->sz *= (double)ctVal / (*bookit)->px;
                }
                else
                {
                    (*bookit)->sz *= (double)ctVal;
                }
                bk = books->find((int)((*bookit)->px * priceUnit));
                if (bk != thisbookend)
                {
                    orgSide = bk->second->side;
                    bk->second->updateBook(OKExEnums::side::_SELL, *bookit);
                    if (bk->second->side == OKExEnums::side::_SELL)
                    {
                        if (bk->second->sz > 0 && bk->first < bestAsk->first)
                        {
                            bestAsk = bk;
                        }
                        else if (bk->first == bestAsk->first && bk->second->sz == 0)
                        {
                            temp_ask = findBest(bk->first, OKExEnums::side::_SELL);
                            if (temp_ask != books->end())
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
                        temp_ask = findBest(bestAsk->first, OKExEnums::side::_SELL);
                        if (temp_ask != books->end())
                        {
                            bestAsk = temp_ask;
                        }
                        else
                        {
                            //?
                        }
                    }
                    if (orgSide != bk->second->side && bk->first <= bestAsk->first)
                    {
                        temp_bid = findBest(bestAsk->first, OKExEnums::side::_BUY);
                        if (temp_bid != books->end())
                        {
                            bestBid = temp_bid;
                        }
                    }
                    //Reshape books
                }
            }
            (*it)->init();
        }
        else
        {
            break;
        }
    }
    return true;
}

bool OKExInstrument::reflectMsg(OKExMktMsg* msg)
{
    bool blOptimize = false;
    if (msg->args->at("channel") == "books")
    {
        if (msg->args->at("action") == "snapshot")
        {
            initializeBooks(msg,bookDepth);
        }
        else if(msg->args->at("action") == "update")
        {
            blOptimize = updateBooks(msg);
        }
    }
    else if (msg->args->at("channel") == "trades")
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
    book* bk;
    book* newbk;
    OKExOrder* neword;
    if (ordit != liveOrdList->end())
    {
        bk = books->at(ordit->second->px);
        if (dtord->fillSz > 0)//Execution
        {
            bk->executeOrder(dtord);
            if (dtord->side == OKExEnums::side::_BUY)
            {
                ++tradedCntBuy;
                tradedQtyBuy += dtord->fillSz;
                tradedAmtBuy += dtord->fillSz * dtord->fillPx;
            }
            else if (dtord->side == OKExEnums::side::_SELL)
            {
                ++tradedCntSell;
                tradedQtySell += dtord->fillSz;
                tradedAmtSell += dtord->fillSz * dtord->fillPx;
            }
        }
        else
        {
            neword = bk->updateOrder(dtord);
            if (neword)
            {
                newbk = books->at(neword->px);
                newbk->addOrder(neword);
                
            }
        }
        if (ordit->second->status == OKExEnums::orderState::_CANCELED || ordit->second->status == OKExEnums::orderState::_FILLED)
        {
            liveOrdList->erase(ordit->first);
        }
    }
}