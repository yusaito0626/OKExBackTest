#include "OKExMessage.h"

msgbook::msgbook()
{
    px = 0.0;
    sz = 0.0;
    liqOrd = 0.0;
    numOfOrd = 0;
}
msgbook::~msgbook()
{

}
void msgbook::init(void)
{
    px = 0.0;
    sz = 0.0;
    liqOrd = 0.0;
    numOfOrd = 0;
}

dataBooks::dataBooks()
{
    int maxBooks = 400;
    bids = new std::list<msgbook*>();
    asks = new std::list<msgbook*>();
    for (int i = 0; i < maxBooks; ++i)
    {
        bids->push_back(new msgbook());
        asks->push_back(new msgbook());
    }
    ts = 0;
    checkSum = 0;
}
dataBooks::~dataBooks()
{
    ts = 0;
    checkSum = 0;
}
void dataBooks::init(void)
{
    std::list<msgbook*>::iterator bidit;
    std::list<msgbook*>::iterator biditend = bids->end();
    std::list<msgbook*>::iterator askit;
    std::list<msgbook*>::iterator askitend = asks->end();
    for (bidit = bids->begin(); bidit != biditend; ++bidit)
    {
        if ((*bidit)->px > 0)
        {
            (*bidit)->init();
        }
        else
        {
            break;
        }
    }
    for (askit = asks->begin(); askit != askitend; ++askit)
    {
        if ((*askit)->px > 0)
        {
            (*askit)->init();
        }
        else
        {
            break;
        }
    }
    ts = 0;
    checkSum = 0;
}

dataTrades::dataTrades()
{
    instId = "";
    tradeId = "";
    px = 0.0;
    sz = 0.0;
    side = OKExEnums::side::_NONE;
    ts = 0;
}
dataTrades::~dataTrades()
{
}
void dataTrades::init(void)
{
    instId = "";
    tradeId = "";
    px = 0.0;
    sz = 0.0;
    side = OKExEnums::side::_NONE;
    ts = 0;
}

dataOrder::dataOrder()
{
    tktType = OKExEnums::ticketType::_NONE;
    instType = OKExEnums::instType::_NONE;
    instId = "";
    tagCcy = OKExEnums::quantityType::_NONE;
    ccy = "";
    ordId = "";
    clOrdId = "";
    tag = "";
    px = 0.0;
    sz = 0.0;
    notionalUsd = 0.0;
    ordType = OKExEnums::ordType::_NONE;
    side = OKExEnums::side::_NONE;
    posSide = OKExEnums::posSide::_NONE;
    tdMode = OKExEnums::tradeMode::_NONE;
    tradeId = "";
    fillPx = 0.0;
    fillSz = 0.0;
    fillTime = 0;
    fillFee = 0.0;
    fillFeeCcy = "";
    execType = OKExEnums::execType::_NONE;
    accFillSz = 0.0;
    fillNotionalUsd = 0.0;
    avgPx = 0.0;
    state = OKExEnums::orderState::_NONE;
    lever = 0.0;
    tpTriggerPx = 0.0;
    tpTriggerPxType = OKExEnums::priceType::_NONE;
    tpOrdPx = 0.0;
    slTriggerPx = 0.0;
    slTriggerPxType = OKExEnums::priceType::_NONE;
    slOrdPx = 0.0;
    feeCcy = "";
    fee = 0.0;
    rebateCcy = "";
    rebate = 0.0;
    pnl = 0.0;
    source = "";
    category = OKExEnums::category::_NONE;
    uTime = 0;
    cTime = 0;
    reqId = "";
    amendResult = OKExEnums::amendResult::_NONE;
    code = 0;
    msg = "";
    algoId = "";
    actualSz = 0.0;
    actualPx = 0.0;
    actualSide = "";
    triggerTime = 0;
    pxVar = 0.0;
    pxSpread = 0.0;
    szLimit = 0.0;
    pxLimit = 0.0;
    timeInterval = 0.0;
    count = 0;
    callbackRatio = 0.0;
    callbackSpread = 0.0;
    activePx = 0.0;
    moveTriggerPx = 0.0;
    pTime = 0;
}
dataOrder::~dataOrder()
{

}
void dataOrder::init(void)
{
    tktType = OKExEnums::ticketType::_NONE;
    instType = OKExEnums::instType::_NONE;
    instId = "";
    tagCcy = OKExEnums::quantityType::_NONE;
    ccy = "";
    ordId = "";
    clOrdId = "";
    tag = "";
    px = 0.0;
    sz = 0.0;
    notionalUsd = 0.0;
    ordType = OKExEnums::ordType::_NONE;
    side = OKExEnums::side::_NONE;
    posSide = OKExEnums::posSide::_NONE;
    tdMode = OKExEnums::tradeMode::_NONE;
    tradeId = "";
    fillPx = 0.0;
    fillSz = 0.0;
    fillTime = 0;
    fillFee = 0.0;
    fillFeeCcy = "";
    execType = OKExEnums::execType::_NONE;
    accFillSz = 0.0;
    fillNotionalUsd = 0.0;
    avgPx = 0.0;
    state = OKExEnums::orderState::_NONE;
    lever = 0.0;
    tpTriggerPx = 0.0;
    tpTriggerPxType = OKExEnums::priceType::_NONE;
    tpOrdPx = 0.0;
    slTriggerPx = 0.0;
    slTriggerPxType = OKExEnums::priceType::_NONE;
    slOrdPx = 0.0;
    feeCcy = "";
    fee = 0.0;
    rebateCcy = "";
    rebate = 0.0;
    pnl = 0.0;
    source = "";
    category = OKExEnums::category::_NONE;
    uTime = 0;
    cTime = 0;
    reqId = "";
    amendResult = OKExEnums::amendResult::_NONE;
    code = 0;
    msg = "";
    algoId = "";
    actualSz = 0.0;
    actualPx = 0.0;
    actualSide = "";
    triggerTime = 0;
    pxVar = 0.0;
    pxSpread = 0.0;
    szLimit = 0.0;
    pxLimit = 0.0;
    timeInterval = 0.0;
    count = 0;
    callbackRatio = 0.0;
    callbackSpread = 0.0;
    activePx = 0.0;
    moveTriggerPx = 0.0;
    pTime = 0;
}
std::string dataOrder::toString(void)
{
    return std::to_string(uTime) + ","
        + std::to_string((int)tktType) + ","
        + std::to_string((int)instType) + ","
        + instId + ","
        + ordId + ","
        + clOrdId + ","
        + std::to_string((int)side) + ","
        + std::to_string(px) + ","
        + std::to_string(sz) + ","
        + std::to_string(fillPx) + ","
        + std::to_string(fillSz) + ","
        + std::to_string(fillTime) + ","
        + std::to_string(avgPx) + ","
        + std::to_string((int)state) + ","
        + std::to_string(code) + ","
        + msg;
}

OKExMktMsg::OKExMktMsg()
{
    blHasData = false;
    args = new std::map<std::string, std::string>();
    books = new std::list<dataBooks*>();
    trades = new std::list<dataTrades*>();
    //Should be one object for books....
    books->push_back(new dataBooks());
    int maxTrades = 100;//for now
    for (int i = 0; i < maxTrades; ++i)
    {
        trades->push_back(new dataTrades());
    }
}
OKExMktMsg::~OKExMktMsg()
{

}
void OKExMktMsg::init(void)
{
    blHasData = false;
    args->clear();
    std::list<dataBooks*>::iterator bookit;
    std::list<dataBooks*>::iterator bookitend = books->end();
    std::list<dataTrades*>::iterator tradeit;
    std::list<dataTrades*>::iterator tradeitend = trades->end();
    for (bookit = books->begin(); bookit != bookitend; ++bookit)
    {
        (*bookit)->init();
    }
    for (tradeit = trades->begin(); tradeit != tradeitend; ++tradeit)
    {
        (*tradeit)->init();
    }
}

ackTikcet::ackTikcet()
{
    ordId = "";
    clOrdId = "";
    tag = "";
    sCode = 0;
    sMsg = "";
    reqId = "";
}
ackTikcet::~ackTikcet()
{

}
void ackTikcet::init(void)
{
    ordId = "";
    clOrdId = "";
    tag = "";
    sCode = 0;
    sMsg = "";
    reqId = "";
}

ordTicket::ordTicket()
{
    idate = 0;
    ts = 0;
    tktType = OKExEnums::ticketType::_NONE;
    instId = "";
    tdMode = OKExEnums::tradeMode::_NONE;
    ccy = "";
    ordId = "";
    clOrdId = "";
    tag = "";
    side = OKExEnums::side::_NONE;
    posSide = OKExEnums::posSide::_NONE;
    ordType = OKExEnums::ordType::_NONE;
    sz = 0.0;
    px = 0.0;
    reduceOnly = false;
    cxlOnFail = false;
    reqId = "";
}
ordTicket::~ordTicket()
{

}
void ordTicket::init(void)
{
    idate = 0;
    ts = 0;
    tktType = OKExEnums::ticketType::_NONE;
    instId = "";
    tdMode = OKExEnums::tradeMode::_NONE;
    ccy = "";
    ordId = "";
    clOrdId = "";
    tag = "";
    side = OKExEnums::side::_NONE;
    posSide = OKExEnums::posSide::_NONE;
    ordType = OKExEnums::ordType::_NONE;
    sz = 0.0;
    px = 0.0;
    reduceOnly = false;
    cxlOnFail = false;
    reqId = "";
}
std::string ordTicket::toString(void)
{
    return std::to_string(ts) + ","
        + std::to_string((int)tktType) + ","
        + instId + ","
        + ordId + ","
        + clOrdId + ","
        + std::to_string((int)side) + ","
        + std::to_string(px) + ","
        + std::to_string(sz);
}

msgOrder::msgOrder()
{
    dataType = "";
    uniId = "";
    op = "";
    errCode = 0;
}
msgOrder::~msgOrder()
{

}
void msgOrder::init()
{
    dataType = "";
    uniId = "";
    op = "";
    errCode = 0;
}