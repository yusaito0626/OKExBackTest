#pragma once


namespace OKExEnums
{
    enum struct msgType
    {
        _NONE = 0,
        _PUSHDATA = 1
    };

    enum struct subsribeType
    {
        _NONE = 0,
        _SUBSCRIBE = 1,
        _UNSUBSCRIBE = 2,
        _ERROR = 3
    };

    enum struct instType
    {
        _NONE = 0,
        _SPOT = 1,
        _MARGIN = 2,
        _SWAP = 3,
        _FUTURES = 4,
        _OPTION = 5
    };

    enum struct side
    {
        _NONE = 0,
        _BUY = 1,
        _SELL = 2
    };

    enum struct tradeMode
    {
        _NONE = 0,
        _ISOLATED = 2,
        _CROSS = 3
    };

    enum struct posSide
    {
        _NONE = 0,
        _NET = 1,
        _LONG = 2,
        _SHORT = 3
    };

    enum struct ordType
    {
        _NONE = 0,
        _MARKET = 1,
        _LIMIT = 2,
        _POST_ONLY = 3,
        _FOK = 4,
        _IOC = 5,
        _OPTIMAL_LIMIT_IOC = 6
    };

    enum struct quantityType
    {
        _NONE = 0,
        _BASE_CCY = 1,
        _QUOTE_CCY = 2
    };

    enum struct eventType
    {
        _NONE = 0,
        _SNAPSHOT = 1,
        _DELIVERED = 2,
        _EXERCISED = 3,
        _TRANSFERRED = 4,
        _FILLED = 5,
        _LIQUIDATION = 6,
        _CLAW_BACK = 7,
        _ADL = 8,
        _FUNDING_FEE = 9,
        _ADJUST_MARGIN = 10,
        _SET_LEVERAGE = 11,
        _INTEREST_DEDUCTION = 12
    };

    enum struct execType
    {
        _NONE = 0,
        _TAKER = 1,
        _MAKER = 2
    };

    enum struct orderState
    {
        _NONE = 0,
        _LIVE = 1,
        _CANCELED = 2,
        _PARTIALLY_FILLED = 3,
        _FILLED = 4,
        _WAIT_NEW = 11,
        _WAIT_AMD = 12,
        _WAIT_CAN = 13
    };

    enum struct priceType
    {
        _NONE = 0,
        _LAST = 1,
        _INDEX = 2,
        _MARK = 3
    };

    enum struct category
    {
        _NONE = 0,
        _NORMAL = 1,
        _TWAP = 2,
        _ADL = 3,
        _FULL_LIQUIDATION = 4,
        _PARTIAL_LIQUIDATION = 5,
        _DELIVERY = 6,
        _DDH = 7
    };

    enum struct amendResult
    {
        _NONE = -99,
        _FAILURE = -1,
        _SUCCESS = 0,
        _AUTO_CANCEL = 1
    };

    enum struct rfqState
    {
        _NONE = 0,
        _ACTIVE = 1,
        _QUOTED = 2,
        _CANCELED = 3,
        _FILLED = 4,
        _EXPIRED = 5,
        _TRADED_AWAY = 6,
        _FAILED = 7
    };

    enum struct algoState
    {
        _NONE = 0,
        _STARTING = 1,
        _RUNNING = 2,
        _STOPPING = 3,
        _NO_CLOSE_POSITION = 4
    };

    enum struct gridType
    {
        _NONE = 0,
        _ARITHMETIC = 1,
        _GEOMETRIC = 2
    };

    enum struct cancelType
    {
        _NONE = 0,
        _MANUALSTOP = 1,
        _TAKEPROFIT = 2,
        _STOPLOSS = 3,
        _RISKCONTROL = 4,
        _DELIVERY = 5
    };

    enum struct stopType
    {
        _NONE = 0,
        _SELLBASECCY = 1,
        _KEEPBASECCY = 2,
        _MKTCLOSEALLPOS = 3,
        _KEEPPOS = 4
    };

    enum struct contractGridType
    {
        _NONE = 0,
        _LONG = 1,
        _SHORT = 2,
        _NUETRAL = 3
    };

    enum struct subOrderState
    {
        _NONE = 0,
        _CANCELED = 1,
        _LIVE = 2,
        _PARTIAL_FILLED = 3,
        _FILLED = 4,
        _CANCELLING = 5
    };

    enum struct optType
    {
        _NONE = 0,
        _CALL = 1,
        _PUT = 2
    };

    enum struct ctType
    {
        _NONE = 0,
        _LINEAR = 1,
        _INVERSE = 2
    };

    enum struct insState
    {
        _NONE = 0,
        _LIVE = 1,
        _SUSPEND = 2,
        _EXPIRED = 3,
        _PREOPEN = 4,
        _SETTLEMENT = 5
    };

    enum struct sysStatus
    {
        _NONE = 0,
        _SCHEDULED = 1,
        _ONGOING = 2,
        _COMPLETED = 3,
        _CANCELED = 4
    };
    enum struct serviceType
    {
        _NONE = -1,
        _WEBSOCKET = 0,
        _SPOTMERGIN = 1,
        _FUTURES = 2,
        _PERPETUAL = 3,
        _OPTIONS = 4,
        _TRADING = 5
    };

}
