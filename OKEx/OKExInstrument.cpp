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
    recentExec = 0.0;
    liveOrders = new std::map<std::string, OKExOrder*>();
}

book::book(const book& obj)
{
    side = obj.side;
    px = obj.px;
    sz = obj.sz;
    szSell = obj.szSell;
    szBuy = obj.szBuy;
    liqOrd = obj.liqOrd;
    numOfOrdBuy = obj.numOfOrdBuy;
    numOfOrdSell = obj.numOfOrdSell;
    szMyOrd = obj.szMyOrd;
    numOfMyOrd = obj.numOfMyOrd;
    ts = obj.ts;
    recentExec = obj.recentExec;
    liveOrders = obj.liveOrders;
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
    recentExec = 0.0;
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
    liveOrders->clear();
    szMyOrd = 0.0;
    numOfMyOrd = 0;
    ts = 0;
    recentExec = 0;
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

OKExOrder* book::getTopOrder(OKExEnums::side sd)
{
    std::map<std::string, OKExOrder*>::iterator it;
    std::map<std::string, OKExOrder*>::iterator itend = liveOrders->end();
    long long ts = 0;
    OKExOrder* ord = nullptr;
    for (it = liveOrders->begin(); it != itend; ++it)
    {
        if (it->second->side == sd)
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
    }
    return ord;
}

//OKExOrder* book::updateOrder(dataOrder* tkt)//Return order object if the price has been changed
//{
//    std::map<std::string, OKExOrder*>::iterator ordit = liveOrders->find(tkt->clOrdId);
//    OKExOrder* output = nullptr;
//    if (ordit != liveOrders->end())
//    {
//        switch (ordit->second->status)
//        {
//        case OKExEnums::orderState::_WAIT_NEW:
//            ordit->second->ordId = tkt->ordId;
//            ordit->second->status = OKExEnums::orderState::_LIVE;
//            ordit->second->live = true;
//            break;
//        case OKExEnums::orderState::_WAIT_AMD:
//            ordit->second->ordId = tkt->ordId;
//            ordit->second->sz = ordit->second->newSz;
//            ordit->second->openSz = ordit->second->sz - ordit->second->execSz;
//            if (ordit->second->openSz <= 0)
//            {
//                ordit->second->openSz = 0;
//                ordit->second->live = false;
//                if (ordit->second->execSz > 0)
//                {
//                    ordit->second->status = OKExEnums::orderState::_FILLED;
//                }
//                else
//                {
//                    ordit->second->status = OKExEnums::orderState::_CANCELED;
//                }
//                liveOrders->erase(ordit->first);
//            }
//            else
//            {
//                if (ordit->second->execSz > 0)
//                {
//                    ordit->second->status = OKExEnums::orderState::_PARTIALLY_FILLED;
//                }
//                else
//                {
//                    ordit->second->status = OKExEnums::orderState::_LIVE;
//                }
//                if (ordit->second->px != ordit->second->newPx)
//                {
//                    ordit->second->px = ordit->second->newPx;
//                    output = ordit->second;
//                    liveOrders->erase(ordit->first);
//                }
//            }
//            break;
//        case OKExEnums::orderState::_WAIT_CAN:
//            ordit->second->ordId = tkt->ordId;
//            ordit->second->sz = ordit->second->execSz;
//            ordit->second->openSz = 0;
//            ordit->second->live = false;
//            if (ordit->second->execSz > 0)
//            {
//                ordit->second->status = OKExEnums::orderState::_FILLED;
//            }
//            else
//            {
//                ordit->second->status = OKExEnums::orderState::_CANCELED;
//            }
//            liveOrders->erase(ordit->first);
//            break;
//        default:
//            break;
//        }
//    }
//    return output;
//}
//bool book::executeOrder(dataOrder* trd)
//{
//    std::map<std::string, OKExOrder*>::iterator ordit = liveOrders->find(trd->clOrdId);
//    if (ordit != liveOrders->end())
//    {
//        ordit->second->lastSz = trd->fillSz;
//        ordit->second->execSz += trd->fillSz;
//        ordit->second->openSz -= trd->fillSz;
//        ordit->second->lastPx = trd->px;
//        ordit->second->avgPx = trd->avgPx;
//        switch (ordit->second->status)
//        {
//        case OKExEnums::orderState::_WAIT_NEW:
//        case OKExEnums::orderState::_WAIT_AMD:
//        case OKExEnums::orderState::_WAIT_CAN:
//            if (ordit->second->openSz < 0)
//            {
//                ordit->second->status = OKExEnums::orderState::_FILLED;
//            }
//            
//            break;
//        default:
//            if (ordit->second->openSz > 0)
//            {
//                ordit->second->status = OKExEnums::orderState::_PARTIALLY_FILLED;
//            }
//            else
//            {
//                ordit->second->status = OKExEnums::orderState::_FILLED;
//            }
//            break;
//        }
//        
//        //if (ordit->second->status == OKExEnums::orderState::_CANCELED || ordit->second->status == OKExEnums::orderState::_FILLED)
//        //{
//        //    liveOrders->erase(ordit->first);
//        //    return true;
//        //}
//    }
//    return false;
//}

void OKExInstrument::setInstrumentData(std::map<std::string, std::string> mp)
{
	instId = mp["instId"];
    alias = mp["alias"];

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
        displayDigits = (int)floor(log10(priceUnit));
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
    //test
    bidOrd = nullptr;
    askOrd = nullptr;

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
    prevBestAsk = books->end();
    prevBestBid = books->end();
    lowestBook = 0;
    highestBook = 0;
    netPosition = 0.0;
    baseMid = 0.0;
    prevNetPos = 0.0;

    tradePL = 0.0;
    posPL = 0.0;
    totalPL = 0.0;

    ordList = new std::map<std::string, OKExOrder*>();
    liveOrdList = new std::map<std::string, OKExOrder*>();
    lckLiveOrdList = false;
    waitingExeQueue = new LockFreeQueue::SISOQueue<dataOrder*>();

    intradayOrdBuy = 0;
    intradayOrdSell = 0;
    intradayExeBuy = 0;
    intradayExeSell = 0;
    intradayExeAmtBuy = 0;
    intradayExeAmtSell = 0;

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
    execImbalance = 0.0;
    hret = 0.0;
    currentTradedQtyBuy = 0;
    currentTradedQtySell = 0;

    ringIdx = -1;
    ringDataCount = 0;
    lastRingUpdatedTime = 0;
    for (int i = 0; i < 60; ++i)
    {
        RVRing[i] = new ring<double>(RINGSIZE);
        netPosRing[i] = new ring<double>(RINGSIZE);
        bookImbalanceRing[i] = new ring<double>(RINGSIZE);
        execImbalanceRing[i] = new ring<double>(RINGSIZE);
        bestAskRing[i] = new ring<int>(RINGSIZE);
        bestBidRing[i] = new ring<int>(RINGSIZE);
        execBidCntRing[i] = new ring<int>(RINGSIZE);
        execAskCntRing[i] = new ring<int>(RINGSIZE);
        execBidQtyRing[i] = new ring<double>(RINGSIZE);
        execAskQtyRing[i] = new ring<double>(RINGSIZE);
        SkewWgtExecBidQtyRing[i] = new ring<double>(RINGSIZE);
        SkewWgtExecAskQtyRing[i] = new ring<double>(RINGSIZE);
        tradeCntBuyRing[i] = new ring<int>(RINGSIZE);
        tradeCntSellRing[i] = new ring<int>(RINGSIZE);
        tradeQtyBuyRing[i] = new ring<double>(RINGSIZE);
        tradeQtySellRing[i] = new ring<double>(RINGSIZE);
        SkewWgtTradeQtyBuyRing[i] = new ring<double>(RINGSIZE);
        SkewWgtTradeQtySellRing[i] = new ring<double>(RINGSIZE);
        midRing[i] = new ring<double>(RINGSIZE);
        midMARing[i] = new ring<double>(RINGSIZE);
        for (int j = 0; j < RINGSIZE; ++j)
        {
            RVRing[i]->add(0.0);
            netPosRing[i]->add(0.0);
            bookImbalanceRing[i]->add(0.0);
            execImbalanceRing[i]->add(0.0);
            bestAskRing[i]->add(0);
            bestBidRing[i]->add(0);
            execBidCntRing[i]->add(0);
            execAskCntRing[i]->add(0);
            execBidQtyRing[i]->add(0.0);
            execAskQtyRing[i]->add(0.0);
            SkewWgtExecBidQtyRing[i]->add(0.0);
            SkewWgtExecAskQtyRing[i]->add(0.0);
            tradeCntBuyRing[i]->add(0);
            tradeCntSellRing[i]->add(0);
            tradeQtyBuyRing[i]->add(0.0);
            tradeQtySellRing[i]->add(0.0);
            SkewWgtTradeQtyBuyRing[i]->add(0.0);
            SkewWgtTradeQtySellRing[i]->add(0.0);
            midRing[i]->add(0.0);
            midMARing[i]->add(0.0);
        }
    }

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

void OKExInstrument::setParams(std::map<std::string, std::string> params)
{
    std::map<std::string, std::string>::iterator itend = params.end();
    if (params.find("BITicks") != itend)
    {
        BITicks = stoi(params["BITicks"]);
    }
    if (params.find("BIDecayingParam") != itend)
    {
        BIDecayingParam = stod(params["BIDecayingParam"]);
    }
    if (params.find("EIPeriod") != itend)
    {
        EIPeriod = stoi(params["EIPeriod"]);
    }
    if (params.find("EIDecayingParam") != itend)
    {
        EIDecayingParam = stod(params["EIDecayingParam"]);
    }
    if (params.find("TQIPeriod") != itend)
    {
        TQIPeriod = stoi(params["TQIPeriod"]);
    }
    if (params.find("TQIDecayingParam") != itend)
    {
        TQIDecayingParam = stod(params["TQIDecayingParam"]);
    }
    if (params.find("SkewWgtEIPeriod") != itend)
    {
        SkewWgtEIPeriod = stoi(params["SkewWgtEIPeriod"]);
    }
    if (params.find("SkewWgtEIDecayingParam") != itend)
    {
        SkewWgtEIDecayingParam = stod(params["SkewWgtEIDecayingParam"]);
    }
    if (params.find("MAPeriod") != itend)
    {
        MAPeriod = stoi(params["MAPeriod"]);
    }
    if (params.find("RVPeriod") != itend)
    {
        RVPeriod = stoi(params["RVPeriod"]);
    }
    if (params.find("histReturnPeriod") != itend)
    {
        histReturnPeriod = stoi(params["histReturnPeriod"]);
    }
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
            if (open == 0)
            {
                open = last;
            }
            if (low == 0 || low > last)
            {
                low = last;
            }
            if (high == 0 || high < last)
            {
                high = last;
            }
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

bool OKExInstrument::initializeBooks(OKExMktMsg* msg, int depth)
{
    PoolingStack::PoolingStack<book*> bookPool;
    bookDepth = depth;
    int tick = (int)(tickSz * priceUnit);

    if (books->size() > 0)
    {
        std::map<int, book*>::iterator it;
        std::map<int, book*>::iterator itend = books->end();
        for (it = books->begin(); it != itend; ++it)
        {
            it->second->init();
            bookPool.push(it->second);
        }
        books->clear();
    }

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
                    book* bk;
                    if (bookPool.Count() > 0)
                    {
                        bk = bookPool.pop();
                    }
                    else
                    {
                        bk = new book();
                    }
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
                    book* bk;
                    if (bookPool.Count() > 0)
                    {
                        bk = bookPool.pop();
                    }
                    else
                    {
                        bk = new book();
                    }
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
        tempbestbid = tempbestask - tick;
    }
    else if (tempbestbid > 0)
    {
        mid = tempbestbid;
        tempbestask = tempbestbid + tick;
    }
    else
    {
        mid = 0;
        return false;
    }

    int i = 0;
    int askpr = tempbestask;
    int bidpr = tempbestbid;
    bidsitend = bids.end();
    asksitend = asks.end();

    int temppr = tempbestbid + tick;
    while (temppr < tempbestask)
    {
        book* bk = new book();
        bk->init();
        bk->px = (double)temppr / priceUnit;
        books->emplace(temppr, bk);
        temppr += tick;
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
            book* bk;
            if (bookPool.Count() > 0)
            {
                bk = bookPool.pop();
            }
            else
            {
                bk = new book();
            }
            bk->px = (double)bidpr / priceUnit;
            books->emplace(bidpr, bk);
        }
        if (asksit != asksitend)
        {
            books->emplace(askpr, asksit->second);
        }
        else
        {
            book* bk;
            if (bookPool.Count() > 0)
            {
                bk = bookPool.pop();
            }
            else
            {
                bk = new book();
            }
            bk->px = (double)askpr / priceUnit;
            books->emplace(askpr, bk);
        }
        highestBook = askpr;
        lowestBook = bidpr;
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
    book* temp;
    if (bookPool.Count() > 0)
    {
        temp = bookPool.pop();
        delete temp;
    }
    if (liveOrdList->size() > 0)
    {
        std::map<std::string, OKExOrder*>::iterator it;
        std::map<std::string, OKExOrder*>::iterator itend = liveOrdList->end();
        std::map<int, book*>::iterator bkit;
        std::map<int, book*>::iterator bkitend = books->end();
        for (it = liveOrdList->begin(); it != itend; ++it)
        {
            bkit = books->find((int)(it->second->px * priceUnit));
            if (bkit != bkitend)
            {
                bkit->second->addOrder(it->second);
//                bkit->second->liveOrders->emplace(it->first, it->second);
            }
        }
    }
    return true;
}

bool OKExInstrument::reshapeBooks(void)
{
    if (bestBid == books->end() && bestAsk == books->end())
    {
        bestBid = findBest((--books->end())->first,OKExEnums::side::_BUY);
        bestAsk = findBest(books->begin()->first, OKExEnums::side::_SELL);
        if (bestBid == books->end() && bestAsk == books->end())
        {
            return false;
        }
    }
    if (bestBid == books->end())
    {
        bestBid = books->begin();
    }
    if (bestAsk == books->end())
    {
        bestAsk = --books->end();
    }
    int bidDepth = bestBid->first - lowestBook;
    int askDepth = highestBook - bestAsk->first;

    int orgPr;
    int destPr;

    if (bidDepth > askDepth)//lowest -> highest
    {
        while (bidDepth > askDepth)
        {
            orgPr = lowestBook;
            destPr = highestBook + tickSz * priceUnit;
            book* temp = books->at(orgPr);
            books->erase(orgPr);
            temp->init();
            temp->px = (double)destPr / priceUnit;
            books->emplace(destPr, temp);
            lowestBook += tickSz * priceUnit;
            highestBook = destPr;
            bidDepth = bestBid->first - lowestBook;
            askDepth = highestBook - bestAsk->first;
        }
    }
    else if (bidDepth < askDepth)
    {
        while (bidDepth < askDepth)
        {
            orgPr = highestBook;
            destPr = lowestBook - tickSz * priceUnit;
            if (destPr <= 0)
            {
                break;
            }
            book* temp = books->at(orgPr);
            books->erase(orgPr);
            temp->init();
            temp->px = (double)destPr / priceUnit;
            books->emplace(destPr, temp);
            lowestBook = destPr;
            highestBook -= tickSz * priceUnit;
            bidDepth = bestBid->first - lowestBook;
            askDepth = highestBook - bestAsk->first;
        }
    }
    return true;
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
                if (it->second->szBuy > 0 && it->second->side == side)
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
                else if (it->second->szSell > 0 && it->second->side == side)
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
    std::map<int, book*>::iterator thisbookbegin = books->begin();
    std::map<int, book*>::iterator bk;
    OKExEnums::side orgSide;
    double orgSz;
    std::map<int, book*>::iterator temp_bestbid = bestBid;
    std::map<int, book*>::iterator temp_bestask = bestAsk;
    std::map<int, book*>::iterator tempbook;

    bool searchBest = false;

    prevBestAsk = bestAsk;
    prevBestBid = bestBid;
    if (prevBestAsk == thisbookend)
    {
        --prevBestAsk;
    }
    if (prevBestBid == thisbookend)
    {
        prevBestBid = thisbookbegin;
    }

    for (it = msg->books->begin(); it != itend; ++it)
    {
        if ((*it)->ts > 0)
        {
            if ((*it)->ts > ts)
            {
                ts = (*it)->ts;
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
                    orgSz = bk->second->szSell;
                    bk->second->updateBook(OKExEnums::side::_SELL, *bookit);
                    checkExecution(bk, orgSide, orgSz, bk->first);

                    if (bk->second->side == OKExEnums::side::_SELL)
                    {
                        if (bk->second->sz > 0 && bk->first < temp_bestask->first)
                        {
                            temp_bestask = bk;
                        }
                        else if (bk->first == temp_bestask->first && bk->second->sz == 0)
                        {
                            tempbook = findBest(bk->first, OKExEnums::side::_SELL);
                            if (tempbook != books->end() && tempbook->first < temp_bestask->first)
                            {
                                temp_bestask = tempbook;
                            }
                            else
                            {
                                searchBest = true;
                            }
                        }
                    }
                    else
                    {
                        tempbook = findBest(prevBestAsk->first, OKExEnums::side::_SELL);
                        if (tempbook != books->end() && tempbook->first < temp_bestask->first)
                        {
                            temp_bestask = tempbook;
                        }
                        else
                        {
                            searchBest = true;
                        }
                    }
                    if (orgSide != bk->second->side && bk->first <= bestAsk->first)
                    {
                        tempbook = findBest(prevBestAsk->first, OKExEnums::side::_BUY);
                        if (tempbook != books->end() && tempbook->first > temp_bestbid->first)
                        {
                            temp_bestbid = tempbook;
                        }
                        else
                        {
                            searchBest = true;
                        }
                    }
                }
                else
                {
                    break;
                }
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
                    orgSz = bk->second->szBuy;
                    bk->second->updateBook(OKExEnums::side::_BUY, *bookit);
                    checkExecution(bk, orgSide, orgSz, bk->first);
                    if (bk->second->side == OKExEnums::side::_BUY)
                    {
                        if (bk->second->sz > 0 && bk->first > temp_bestbid->first)
                        {
                            temp_bestbid = bk;
                        }
                        else if (bk->first == temp_bestbid->first && bk->second->sz == 0)
                        {
                            tempbook = findBest(bk->first, OKExEnums::side::_BUY);
                            if (tempbook != books->end() && tempbook->first > temp_bestbid->first)
                            {
                                temp_bestbid = tempbook;
                            }
                            else
                            {
                                searchBest = true;
                            }
                        }
                    }
                    else
                    {
                        tempbook = findBest(prevBestBid->first, OKExEnums::side::_BUY);
                        if (tempbook != books->end() && tempbook->first > temp_bestbid->first)
                        {
                            temp_bestbid = tempbook;
                        }
                        else
                        {
                            searchBest = true;
                        }
                    }
                    if (orgSide != bk->second->side && bk->first >= bestBid->first)
                    {
                        tempbook = findBest(prevBestBid->first, OKExEnums::side::_SELL);
                        if (tempbook != books->end() && tempbook->first < temp_bestask->first)
                        {
                            temp_bestask = tempbook;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
            if (bestBid->first - lowestBook < bookDepth / 4 || highestBook - bestAsk->first < bookDepth / 4)
            {
                isTrading = reshapeBooks();
                if (!isTrading)
                {
                    logWriter->addLog(Enums::logType::_WARNING, "Failed to reshape books. instId:" + instId);
                    logWriter->addLog(Enums::logType::_WARNING, "Trade Stopped");
                }
            }
            else if (searchBest)
            {
                bestAsk = findBest(books->begin()->first, OKExEnums::side::_SELL);
                bestBid = findBest((--books->end())->first, OKExEnums::side::_BUY);
            }
            else
            {
                while (temp_bestask != thisbookend)
                {
                    if (temp_bestask->second->sz > 0 && temp_bestask->second->side == OKExEnums::side::_SELL)
                    {
                        bestAsk = temp_bestask;
                        break;
                    }
                    else
                    {
                        ++temp_bestask;
                    }
                }
                while (temp_bestbid != thisbookend)
                {
                    if (temp_bestbid->second->sz > 0 && temp_bestbid->second->side == OKExEnums::side::_BUY)
                    {
                        bestBid = temp_bestbid;
                        break;
                    }
                    else if(temp_bestbid == thisbookbegin)
                    {
                        temp_bestbid = thisbookend;
                    }
                    else
                    {
                        ++temp_bestbid;
                    }
                }
                if (temp_bestask == thisbookend)
                {
                    bestAsk = thisbookend;
                }
                if (temp_bestbid == thisbookend)
                {
                    bestBid = thisbookend;
                }
            }
            (*it)->init();
        }
        else
        {
            break;
        }
    }
    
    return false;
}


void OKExInstrument::checkExecution(std::map<int, book*>::iterator currentbk, OKExEnums::side orgSide, double orgSz, int pr)
{
    //Execute my orders
    //if sz increased, all of sz of buy orders higher than sell pr, sell orders less than buy pr
    //if sz increased, same price, the other side, execute sz - orgSz
    //if sz decreased, same price, same side, deduct orgSz - sz if there is no book behind the order at the same price.
    std::map<int, book*>::iterator tempbk;
    std::map<std::string, OKExOrder*>::iterator ordit;
    std::map<std::string, OKExOrder*>::iterator orditend;
    std::string msg;
    OKExOrder* ord = nullptr;
    OKExEnums::side ordSide = OKExEnums::side::_NONE;
    dataOrder* exec;
    double currentSz;
    switch (currentbk->second->side)
    {
    case OKExEnums::side::_BUY:
        ordSide = OKExEnums::side::_SELL;
        currentSz = currentbk->second->szBuy;
        //Check sell Orders from org best buy to the current bk
        if (currentbk->first > prevBestBid->first && currentSz > orgSz)
        {
            for (tempbk = prevBestBid; tempbk != currentbk; ++tempbk)
            {
                orditend = tempbk->second->liveOrders->end();
                for (ordit = tempbk->second->liveOrders->begin(); ordit != orditend; ++ordit)
                {
                    if (ordit->second->side != currentbk->second->side)
                    {
                        msg = "OKExInstrument 1294";
                        exec = execute(ts, instId, ordit->second, ordit->second->openSz, ordit->second->px, msg);
                        //updateOrders(exec);
                    }
                }
            }
        }
        break;
    case OKExEnums::side::_SELL:
        ordSide = OKExEnums::side::_BUY;
        currentSz = currentbk->second->szSell;
        //Check buy orders from org best sell to the current bk
        if (currentbk->first < prevBestAsk->first && currentSz > orgSz)
        {
            for (tempbk = prevBestAsk; tempbk != currentbk; --tempbk)
            {
                orditend = tempbk->second->liveOrders->end();
                for (ordit = tempbk->second->liveOrders->begin(); ordit != orditend; ++ordit)
                {
                    if (ordit->second->side != currentbk->second->side)
                    {
                        msg = "OKExInstrument 1313";
                        exec = execute(ts, instId, ordit->second, ordit->second->openSz, ordit->second->px, msg);
                        //updateOrders(exec);
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    if (orgSide == currentbk->second->side && orgSide != OKExEnums::side::_NONE)
    {
        if (currentSz - orgSz < 0)
        {
            double deductingSz = orgSz - currentSz;
            if (currentbk->second->recentExec >= deductingSz)
            {
                currentbk->second->recentExec -= deductingSz;
                deductingSz = 0;
            }
            else
            {
                if (currentbk->second->recentExec > 0)
                {
                    deductingSz -= currentbk->second->recentExec;
                    currentbk->second->recentExec = 0;
                }
                std::map<std::string, OKExOrder*>::iterator it;
                std::map<std::string, OKExOrder*>::iterator itend = currentbk->second->liveOrders->end();
                for (it = currentbk->second->liveOrders->begin(); it != itend; ++it)
                {
                    if (currentbk->second->side == it->second->side)
                    {
                        double qtyBehind = orgSz - it->second->priorQuantity;
                        if (qtyBehind < deductingSz)
                        {
                            it->second->priorQuantity -= deductingSz - qtyBehind;
                            if (it->second->priorQuantity < 0)
                            {
                                it->second->priorQuantity = 0;
                            }
                        }
                    }
                }
            }
        }
        else if (currentSz - orgSz > 0)
        {
            orditend = currentbk->second->liveOrders->end();
            for (ordit = currentbk->second->liveOrders->begin(); ordit != orditend; ++ordit)
            {
                if (currentbk->second->side != ordit->second->side)
                {
                    ordit->second->priorQuantity = 0;
                }
            }
            double execSzAll = currentSz - orgSz;
            while (execSzAll > 0)
            {
                ord = currentbk->second->getTopOrder(ordSide);
                if (ord)
                {
                    double execSz = execSzAll;
                    if (execSz > ord->openSz)
                    {
                        execSz = ord->openSz;
                    }
                    dataOrder* exec;
                    ord->priorQuantity = 0;
                    msg = "OKExInstrument 1382";
                    exec = execute(ts, instId, ord, execSz, currentbk->second->px, msg);
                    //updateOrders(exec);
                    execSzAll -= execSz;
                }
                else
                {
                    break;
                }
            }
        }
    }
    else
    {
        orditend = currentbk->second->liveOrders->end();
        for (ordit = currentbk->second->liveOrders->begin(); ordit != orditend; ++ordit)
        {
            if (currentbk->second->side != ordit->second->side)
            {
                ordit->second->priorQuantity = 0;
            }
        }
        double execSzAll = currentbk->second->sz - orgSz;
        while (execSzAll > 0)
        {
            ord = currentbk->second->getTopOrder(ordSide);
            if (ord)
            {
                double execSz = execSzAll;
                if (execSz > ord->openSz)
                {
                    execSz = ord->openSz;
                }
                dataOrder* exec;
                ord->priorQuantity = 0;
                exec = execute(ts, instId, ord, execSz, currentbk->second->px, msg);
                //updateOrders(exec);
                execSzAll -= execSz;
            }
            else
            {
                break;
            }
        }
    }
    checkWaitingExeQueue();
}

bool OKExInstrument::reflectMsg(OKExMktMsg* msg)
{
    bool blOptimize = false;
    if (msg->args->at("channel") == "books")
    {
        if (msg->args->at("action") == "snapshot")
        {
            initializeBooks(msg,1000);
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
    std::map<int, book*>::iterator bk;
    std::map<int,book*>::iterator newbk;
    OKExOrder* neword;
    double orgSz;
    if (ordit != liveOrdList->end())
    {
        bk = books->find((int)(ordit->second->px * priceUnit));
        if (bk != books->end())
        {
            if (dtord->fillSz > 0)//Execution
            {
                ordit->second->ordId = dtord->ordId;
                ordit->second->lastSz = dtord->fillSz;
                ordit->second->execSz += dtord->fillSz;
                ordit->second->openSz -= dtord->fillSz;
                ordit->second->lastPx = dtord->px;
                ordit->second->avgPx = dtord->avgPx;
                switch (ordit->second->status)
                {
                case OKExEnums::orderState::_WAIT_NEW:
                case OKExEnums::orderState::_WAIT_AMD:
                case OKExEnums::orderState::_WAIT_CAN:
                    if (ordit->second->openSz < lotSz)
                    {
                        ordit->second->openSz = 0;
                        ordit->second->execSz = ordit->second->sz;
                        ordit->second->status = OKExEnums::orderState::_FILLED;
                        ordit->second->live = false;
                    }

                    break;
                default:
                    if (ordit->second->openSz < lotSz)
                    {
                        ordit->second->openSz = 0;
                        ordit->second->execSz = ordit->second->sz;
                        ordit->second->status = OKExEnums::orderState::_FILLED;
                    }
                    else
                    {
                        ordit->second->status = OKExEnums::orderState::_PARTIALLY_FILLED;
                    }
                    break;
                }
                if (dtord->side == OKExEnums::side::_BUY)
                {
                    ++tradedCntBuy;
                    tradedQtyBuy += dtord->fillSz;
                    tradedAmtBuy += dtord->fillSz * dtord->fillPx;
                    intradayExeAmtBuy += dtord->fillSz * dtord->fillPx;
                }
                else if (dtord->side == OKExEnums::side::_SELL)
                {
                    ++tradedCntSell;
                    tradedQtySell += dtord->fillSz;
                    tradedAmtSell += dtord->fillSz * dtord->fillPx;
                    intradayExeAmtSell += dtord->fillSz * dtord->fillPx;
                }
            }
            else
            {
                OKExOrder* output = nullptr;
                if (ordit != liveOrdList->end())
                {
                    switch (ordit->second->status)
                    {
                    case OKExEnums::orderState::_WAIT_NEW:
                        ordit->second->ordId = dtord->ordId;
                        ordit->second->status = OKExEnums::orderState::_LIVE;
                        ordit->second->live = true;
                        ordit->second->priorQuantity = bk->second->sz;
                        switch (ordit->second->side)
                        {
                        case OKExEnums::side::_BUY:
                            ++intradayOrdBuy;
                            break;
                        case OKExEnums::side::_SELL:
                            ++intradayOrdSell;
                            break;
                        default:
                            break;
                        }
                        break;
                    case OKExEnums::orderState::_WAIT_AMD:
                        ordit->second->ordId = dtord->ordId;
                        ordit->second->openSz = ordit->second->newSz - ordit->second->execSz;
                        if (ordit->second->openSz < lotSz)
                        {
                            ordit->second->openSz = 0;
                            ordit->second->sz = ordit->second->execSz;
                            if (ordit->second->execSz > 0)
                            {
                                ordit->second->status = OKExEnums::orderState::_FILLED;
                            }
                            else
                            {
                                ordit->second->status = OKExEnums::orderState::_CANCELED;
                            }
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
                                ordit->second->sz = ordit->second->newSz;
                                ordit->second->px = ordit->second->newPx;
                                output = ordit->second;
                                bk->second->removeOrder(ordit->first);
                                newbk = books->find((int)(ordit->second->px * priceUnit));
                                if (newbk != books->end())
                                {
                                    newbk->second->addOrder(ordit->second);
                                    ordit->second->priorQuantity = newbk->second->sz;
                                }
                            }
                            else if (ordit->second->newSz > ordit->second->sz)
                            {
                                ordit->second->sz = ordit->second->newSz;
                                ordit->second->priorQuantity = bk->second->sz;
                            }
                        }
                        break;
                    case OKExEnums::orderState::_WAIT_CAN:
                        ordit->second->ordId = dtord->ordId;
                        ordit->second->sz = ordit->second->execSz;
                        ordit->second->openSz = 0;
                        if (ordit->second->execSz > 0)
                        {
                            ordit->second->status = OKExEnums::orderState::_FILLED;
                        }
                        else
                        {
                            ordit->second->status = OKExEnums::orderState::_CANCELED;
                        }
                        break;
                    default:
                        break;
                    }
                }
                //neword = bk->updateOrder(dtord);
                //if (neword)
                //{
                //    newbk = books->at((int)(neword->px * priceUnit));
                //    newbk->addOrder(neword);
                //    
                //}
            }
            if ((ordit->second->status == OKExEnums::orderState::_CANCELED || ordit->second->status == OKExEnums::orderState::_FILLED))
            {
                if (ordit->second->status == OKExEnums::orderState::_FILLED && ordit->second->live)
                {
                    switch (ordit->second->side)
                    {
                    case OKExEnums::side::_BUY:
                        ++intradayExeBuy;
                        break;
                    case OKExEnums::side::_SELL:
                        ++intradayExeSell;
                        break;
                    default:
                        break;
                    }
                }

                ordit->second->live = false;
                bk->second->removeOrder(ordit->first);
                bool desired = true;
                bool expected = false;
                while (true)
                {
                    if (lckLiveOrdList.compare_exchange_weak(expected, desired))
                    {
                        liveOrdList->erase(ordit->first);
                        lckLiveOrdList = false;
                        break;
                    }
                    else
                    {
                        expected = false;
                    }
                }
            }
        }
    }
}

void OKExInstrument::addNewOrder(OKExOrder* ord)
{
    ordList->emplace(ord->baseOrdId, ord);
    std::map<int, book*>::iterator bk = books->find((int)(ord->px * priceUnit));
    if (bk != books->end())
    {
        bk->second->addOrder(ord);

    }
    bool desired = true;
    bool expected = false;
    while (true)
    {
        if (lckLiveOrdList.compare_exchange_weak(expected, desired))
        {
            liveOrdList->emplace(ord->baseOrdId, ord);
            lckLiveOrdList = false;
            break;
        }
        else
        {
            expected = false;
        }
    }
}

double OKExInstrument::getPriorQuantity(OKExEnums::side side, double px)
{
    std::map<int, book*>::iterator it = books->find((int)(px * priceUnit));
    if (it == books->end())
    {
        return 0;
    }
    else
    {
        switch (side)
        {
        case OKExEnums::side::_BUY:
            if (it->second->side == OKExEnums::side::_SELL)
            {
                return 0;
            }
            else
            {
                return it->second->sz;
            }
            break;
        case OKExEnums::side::_SELL:
            if (it->second->side == OKExEnums::side::_BUY)
            {
                return 0;
            }
            else
            {
                return it->second->sz;
            }
            break;
        case OKExEnums::side::_NONE:
        default:
            break;
        }
    }
    return 0;
}

void OKExInstrument::calcMid(void)
{
    std::map<int, book*>::iterator itend = books->end();
    std::map<int, book*>::iterator itbegin = books->begin();
    if (books->size() == 0)
    {
        return;
    }
    if (bestAsk != itend && bestBid != itend)
    {
        mid = (bestAsk->second->px + bestBid->second->px) / 2;
    }
    else if (bestBid == itbegin)
    {
        if (bestBid->second->sz > 0 && bestBid->first > 0)
        {
            mid = (bestAsk->second->px + bestBid->second->px) / 2;
        }
        else if (bestAsk != itend)
        {
            mid = bestAsk->second->px;
        }
    }
    else if (bestAsk != itend)
    {
        mid = bestAsk->second->px;
    }
}

std::string OKExInstrument::outputDailyResult(void)
{
    double avgSellPr;
    double avgBuyPr;

    if (tradedQtySell > 0)
    {
        avgSellPr = tradedAmtSell / tradedQtySell;
    }
    else
    {
        avgSellPr = 0;
    }
    if (tradedQtyBuy > 0)
    {
        avgBuyPr = tradedAmtBuy / tradedQtyBuy;
    }
    else
    {
        avgBuyPr = 0;
    }
    calcMid();

    posPL = prevNetPos * (mid - baseMid);
    tradePL = (tradedAmtSell - tradedQtySell * mid) + (tradedQtyBuy * mid - tradedAmtBuy);
    totalPL = posPL + tradePL;

    return instId + "," + std::to_string(open) + "," + std::to_string(high) + "," + std::to_string(low) + "," + std::to_string(last) + ","
        + std::to_string(tradedQtySell) + "," + std::to_string(avgSellPr) + "," + std::to_string(tradedQtyBuy) + "," + std::to_string(avgBuyPr) + ","
        + std::to_string(posPL) + "," + std::to_string(tradePL) + "," + std::to_string(totalPL);
}

void OKExInstrument::endOfDayReset(void)
{
    expTime = 0;
    listTime = 0;

    baseMid = mid;
    prevNetPos = netPosition;

    tradePL = 0.0;
    posPL = 0.0;
    totalPL = 0.0;

    //The order objects are handled in OMS
    ordList->clear();
    bool desired = true;
    bool expected = false;
    while (true)
    {
        if (lckLiveOrdList.compare_exchange_weak(expected, desired))
        {
            liveOrdList->clear();
            lckLiveOrdList = false;
            break;
        }
        else
        {
            expected = false;
        }
    }

    open = 0.0;
    high = 0.0;
    low = 0.0;

    ts = 0;
    lastOptTs = 0;

    intradayOrdBuy = 0;
    intradayOrdSell = 0;
    intradayExeBuy = 0;
    intradayExeSell = 0;
    intradayExeAmtBuy = 0;
    intradayExeAmtSell = 0;

    //Don't initialize factors
}

void OKExInstrument::checkWaitingExeQueue(void)
{
    dataOrder* exec;
    while (waitingExeQueue->Count() > 0)
    {
        exec = waitingExeQueue->Dequeue();
        updateOrders(exec);
    }
}