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
    px = 0;
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
    px = 0;
    sz = 0.0;
    side = OKExEnums::side::_NONE;
    ts = 0;
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