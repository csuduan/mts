

//
// Created by 段晴 on 2022/1/22.
//

#ifndef MTS_CORE_TRADEEXECUTOR_H
#define MTS_CORE_TRADEEXECUTOR_H


#include <atomic>
#include "data.h"
#include "message.h"
#include "gateway/gateway.h"
#include "strategy/strategy.h"
#include "lockFreeQueue.hpp"
#include "barGenerator.hpp"
#include "common/socketServer.hpp"
#include "common/sqliteHelper.hpp"
#include "msgListener.h"
#include <limits>
#include <mutex>
#include <condition_variable>

class Strategy;
class TradeExecutor : public MsgListener {
private:
    string id;
    Acct *acct;
    //vector<Quote *> quotes;
    //SocketClient* agentClient;
    SocketServer *udsServer;//对内提供服务


    map<string, vector<BarGenerator *> *> barGeneratorMap;
    //LockFreeQueue<Event> msgQueue = {1 << 20};//系统消息队列


    //合约订阅列表
    std::map<string, std::set<Strategy *>> subsMap;

    //策略信息
    std::map<string, Strategy *> strategyMap;
    //报单-策略 映射map
    std::map<int, Strategy *> strategyOrderMap;


    std::vector<Order *> removeList;


    SqliteHelper *sqliteHelper = NULL;

    std::mutex mut;
    std::condition_variable cv;



    //Shm<MemTick>* shm=NULL;
public:
    TradeExecutor(string acctId): id(acctId){}

    void start();


    void subContract(set<string> contracts, Strategy *strategy);

    BarGenerator *getBarGenerator(string symbol, BAR_LEVEL level);

    void onTick(Tick *tick);


    void createStrategy(StrategySetting *setting);

    ///清理（非close）
    void clear();


    void fastEventHandler();

    void msgHandler();

    Message *onRequest(Message *request) override;

    ///优雅停机
    void shutdown();

    int signalHanler(int signo);
};


#endif //MTS_CORE_TRADEEXECUTOR_H
