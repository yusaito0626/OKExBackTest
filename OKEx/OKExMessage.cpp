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
    bids.clear();
    asks.clear();
    ts = 0;
    checkSum = 0;
}

dataBooks::~dataBooks()
{
    bids.clear();
    asks.clear();
    ts = 0;
    checkSum = 0;
}

void dataBooks::init(void)
{
    bids.clear();
    asks.clear();
    ts = 0;
    checkSum = 0;
}

dataTrades::dataTrades()
{
    instId = "";
    tradeId = "";
    px = 0;
    sz = 0.0;
    side = OKExEnums::side::NONE;
    ts = 0;
}

dataTrades::~dataTrades()
{
}

void dataTrades::init(void)
{
    instId = "";
    tradeId = "";
    px = 0;
    sz = 0.0;
    side = OKExEnums::side::NONE;
    ts = 0;
}

OKExMktMsg::OKExMktMsg()
{
    args.clear();
    books.clear();
    trades.clear();
}
OKExMktMsg::~OKExMktMsg()
{
    args.clear();
    books.clear();
    trades.clear();
}
void OKExMktMsg::init(void)
{
    args.clear();
    books.clear();
    trades.clear();
}