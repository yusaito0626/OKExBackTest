#pragma once

#include "includes.h"
#include "OKExEnums.h"
struct msgbook
{
    double px;
    double sz;
    double liqOrd;
    int numOfOrd;
    msgbook();
    ~msgbook();
    void init(void);
};
struct dataBooks
{
    std::list<msgbook*>* bids;
    std::list<msgbook*>* asks;
    long long ts;
    int checkSum;

    dataBooks();
    ~dataBooks();
    void init(void);
};
struct dataTrades
{
    std::string instId;
    std::string tradeId;
    int px;
    double sz;
    OKExEnums::side side;
    long long ts;

    dataTrades();
    ~dataTrades();
    void init(void);
};

struct dataOrder
{
    OKExEnums::ticketType tktType;

    OKExEnums::instType instType;
    std::string instId;
    OKExEnums::quantityType tagCcy;
    std::string ccy;
    std::string ordId;
    std::string clOrdId;
    std::string tag;
    double px;
    double sz;
    double notionalUsd;
    OKExEnums::ordType ordType;
    OKExEnums::side side;
    OKExEnums::posSide posSide;
    OKExEnums::tradeMode tdMode;
    std::string tradeId;
    double fillPx;
    double fillSz;
    long long fillTime;
    double fillFee;
    std::string fillFeeCcy;
    OKExEnums::execType execType;
    double accFillSz;
    double fillNotionalUsd;
    double avgPx;
    OKExEnums::orderState state;
    double lever;

    double tpTriggerPx;
    OKExEnums::priceType tpTriggerPxType;
    double tpOrdPx;
    double slTriggerPx;
    OKExEnums::priceType slTriggerPxType;
    double slOrdPx;
    std::string feeCcy;
    double fee;
    std::string rebateCcy;
    double rebate;
    double pnl;
    std::string source;
    OKExEnums::category category;
    long long uTime;
    long long cTime;
    std::string reqId;
    OKExEnums::amendResult amendResult;
    int code;
    std::string msg;
    //algo order channel
    std::string algoId;
    double actualSz;
    double actualPx;
    std::string actualSide;
    long long triggerTime;
    //Advanced algo order channel
    double pxVar;
    double pxSpread;
    double szLimit;
    double pxLimit;
    double timeInterval;
    int count;
    double callbackRatio;
    double callbackSpread;
    double activePx;
    double moveTriggerPx;
    long long pTime;
};

struct OKExMktMsg
{
    bool blHasData;
    std::map<std::string, std::string>* args;
	std::list<dataBooks*>* books;
	std::list<dataTrades*>* trades;
    OKExMktMsg();
    ~OKExMktMsg();
    void init(void);
};

struct ackTikcet
{
    std::string ordId;
    std::string clOrdId;
    std::string tag;
    int sCode;
    std::string sMsg;
    std::string reqId;
};

struct ordTicket
{
    int idate;
    long long ts;

    OKExEnums::ticketType tktType;
    std::string instId;
    OKExEnums::tradeMode tdMode;
    std::string ccy;
    std::string ordId;
    std::string clOrdId;
    std::string tag;
    OKExEnums::side side;
    OKExEnums::posSide posSide;
    OKExEnums::ordType ordType;
    double sz;
    double px;
    bool reduceOnly;
    //Only Amend Orders
    bool cxlOnFail;
    std::string reqId;
};

struct msgOrder
{
    std::string dataType;//Always order
    std::string uniId;
    std::string op;
    int errCode;
};
