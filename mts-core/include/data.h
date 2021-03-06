#pragma once

/**
 * 数据结构
 *
 */

#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <tuple>
#include "enums.h"
#include "lockFreeQueue.hpp"
#include "xpack/json.h"
#include "message.h"
#include<fstream>
#include <iostream>
#include <any>


using std::string;
using std::map;
using std::vector;

struct Event {
public:
    EvType type;
    long tsc;
    void *data;
    //std::any data;
};


struct Position;
struct Order;

struct Contract {
    string name;// 合约中文名
    string type; // 合约类型
    string symbol; // 合约代码
    string exchange; // 交易所代码
    //string posDateType;//持仓日期类型（可用于判断是否区分今昨仓）

    //期货相关
    double priceTick; // 最小变动价位
    double longMarginRatio; // 多头保证金率
    double shortMarginRatio; // 空头保证金率
    bool maxMarginSideAlgorithm; // 最大单边保证金算法
    int multiple;//合约乘数

    // 期权相关
    double strikePrice; // 期权行权价
    string underlyingSymbol; // 标的物合约代码
    string optionType; /// 期权类型
    string expiryDate; // 到期日
XPACK(M(name, type, symbol, exchange), O(priceTick, longMarginRatio, multiple))
};


struct Position {
    string id;//持仓编号（合约代码-方向）
    string symbol; // 代码
    POS_DIRECTION direction; // 持仓方向
    string exchange; // 交易所代码


    // 持仓相关
    int pos; // 持仓量
    int onway; // 在途数量(>0在途开仓，<0在途平仓)
    int ydPos; // 昨仓（=pos-tdPos）
    int tdPos; // 今仓
    int ydPosition;//上日持仓
    double useMargin; // 占用的保证金

    //交易核心中不再计算盈亏
//    double lastPrice; // 计算盈亏使用的行情最后价格
//    double avgPrice; // 持仓均价
//    double openPrice; // 开仓均价
//    double holdProfit; // 持仓盈亏
//    int multiple;//合约乘数

    Position(string symbol, POS_DIRECTION direction) {
        this->symbol = symbol;
        this->direction = direction;
        this->id = this->symbol + "-" + string(enum_string(direction));
    }

XPACK(M(id, symbol, exchange, direction, pos, ydPos, tdPos, ydPosition), O(useMargin));
};


struct Order {
    // 代码编号相关
    string symbol; // 代码
    string exchange; // 交易所代码
    int orderRef; // 订单编号
    string orderSysId;//交易所报单编号
    // 报单相关
    TRADE_DIRECTION direction;//报单方向
    string directionStr;
    string positionId;//持仓代码（合约-方向）
    TRADE_TYPE offset; // 报单开平仓
    string offsetStr;
    ORDER_TYPE orderType;//报单方式
    string orderTypeStr;
    double price; // 报单价格
    int totalVolume; // 报单总数量
    int tradedVolume; // 报单成交数量
    ORDER_STATUS status; // 报单状态
    string statusStr;
    string statusMsg;
    string tradingDay;//交易日期
    //string actionDate; // 自然日期
    //string orderTime; // 发单时间
    string updateTime; // 最后修改时间
    long updateTsc;
    bool canceling = false;//测单中
    bool finished = false;

    // CTP/LTS相关
    int frontID; // 前置机编号
    int sessionID; // 连接编号

    int realTradedVolume;//实际成交手数(来自trade)

    POS_DIRECTION getPosDirection() {
        if (offset == TRADE_TYPE::OPEN)
            return direction == TRADE_DIRECTION::BUY ? POS_DIRECTION::LONG : POS_DIRECTION::SHORT;
        else
            return direction == TRADE_DIRECTION::SELL ? POS_DIRECTION::LONG : POS_DIRECTION::SHORT;
    }

XPACK(M(tradingDay, orderRef, symbol, exchange, offsetStr, directionStr, price, totalVolume, tradedVolume, statusStr,
        statusMsg, updateTime));

};


struct Action {
    int orderRef;
    int frontId;
    long long sessionId;
};

struct Trade {
    // 代码编号相关
    string symbol; // 代码
    string exchange; // 交易所代码
    string tradeId; // 成交编号
    int orderRef;

    // 成交相关
    TRADE_DIRECTION direction; //成交方向
    TRADE_TYPE tradeType; // 成交开平仓
    double tradedPrice; // 成交价格
    int tradedVolume; // 成交数量

    string tradingDay; // 交易日
    string tradeDate; // 业务发生日
    string tradeTime; // 时间(HHMMSSmmm)
    string sysId; //报单系统编号

    long updateTsc;

    POS_DIRECTION getPosDirection() {
        if (tradeType == TRADE_TYPE::OPEN)
            return direction == TRADE_DIRECTION::BUY ? POS_DIRECTION::LONG : POS_DIRECTION::SHORT;
        else
            return direction == TRADE_DIRECTION::SELL ? POS_DIRECTION::LONG : POS_DIRECTION::SHORT;
    }

XPACK(M(tradingDay, tradeId, orderRef, symbol, exchange, tradeType, direction, tradedPrice, tradedVolume, tradeTime));

};

/**
 * TICK行情
 */
struct Tick {
    string symbol; // 合约代码
    string exchange; // 交易所代码
    string tradingDay; //交易日
    string actionDay;  //自然日
    float updateTime; //时间 格式:091230.500

    string source;//来源

    // 成交数据
    double lastPrice = 0; // 最新成交价
    int lastVolume = 0; // 最新成交量
    int volume = 0; // 今天总成交量
    double openInterest = 0; // 持仓量

    long preOpenInterest = 0L;// 昨持仓
    double preClosePrice = 0; // 前收盘价
    double preSettlePrice = 0; // 昨结算

    // 常规行情
    double openPrice = 0; // 今日开盘价
    double highPrice = 0; // 今日最高价
    double lowPrice = 0; // 今日最低价
    double upperLimit = 0; // 涨停价
    double lowerLimit = 0; // 跌停价

    double bidPrice1 = 0;
    double bidPrice2 = 0;
    double bidPrice3 = 0;
    double bidPrice4 = 0;
    double bidPrice5 = 0;
    double askPrice1 = 0;
    double askPrice2 = 0;
    double askPrice3 = 0;
    double askPrice4 = 0;
    double askPrice5 = 0;
    int bidVolume1 = 0;
    int bidVolume2 = 0;
    int bidVolume3 = 0;
    int bidVolume4 = 0;
    int bidVolume5 = 0;
    int askVolume1 = 0;
    int askVolume2 = 0;
    int askVolume3 = 0;
    int askVolume4 = 0;
    int askVolume5 = 0;

    //用于性能统计的相关字段
    long recvTsc;  //接收时间  tsc
    long eventTsc;//事件触发时间 tsc
XPACK(M(symbol, exchange, tradingDay, actionDay, updateTime, lastPrice, bidPrice1, askPrice1));

};

struct Bar {
    BAR_LEVEL level;//级别M1,M5
    string level_s;
    string symbol; // 代码
    string exchange;
    string tradingDay; // 交易日
    string actionDay; // 业务发生日
    int barTime; // 时间(HHmmss)
    float updateTime;//最新时间(HHmmss.SSS)
    // LocalDateTime dateTime;
    double open = 0;
    double high = 0;
    double low = 0;
    double close = 0;

    int volume = 0; // 成交量
    double openInterest = 0; // 持仓量

    int tickCount = 0;

    bool saved = false;//是否持久化

XPACK(M(level, symbol, exchange, tradingDay, actionDay, barTime, updateTime, open, high, low, close, volume,
        openInterest, tickCount));


};

struct StrategySetting {
    string strategyId;
    string strategyType;
    BAR_LEVEL barLevel;
    int barOffset;
    string refSymbol;
    string trgSymbol;
    map<string, string> paramMap;
};

struct QuotaFile {
    std::ofstream ofs;
    string date;
    string fname;
};





