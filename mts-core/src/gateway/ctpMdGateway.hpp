//
// Created by Administrator on 2020/7/2.
//

#ifndef TRADECORE_CTPMDAPI_H
#define TRADECORE_CTPMDAPI_H


#include "ctp/ThostFtdcMdApi.h"
#include "ctp/ThostFtdcUserApiStruct.h"
#include <functional>
#include <dlfcn.h>
#include "delegate.h"
#include "define.h"

#include <thread>
#include <netdb.h>
#include "ctpMdGateway.hpp"
#include "data.h"
#include "common/util.hpp"
#include <climits>
#include <float.h>

#include "data.h"
#include "gateway.h"
#include "lockFreeQueue.hpp"
#include "trade/acct.h"
#include "common/util.hpp"


class CtpMdGateway : public CThostFtdcMdSpi, public MdGateway {
public:
    //std::function<void(TickData *)> tickDataCallBack = nullptr;
    //std::list<std::function<void(TickData)>> tickDataCallBack;
    CtpMdGateway(QuoteInfo *quota) : MdGateway(quota) {
    }

    ~CtpMdGateway() {}

    void ReqUserLogin() {
        CThostFtdcReqUserLoginField reqUserLogin;
        int ret = m_pUserApi->ReqUserLogin(&reqUserLogin, nRequestID++);
        logi("\tlogin ret = {}\n", ret);
    }

    void OnFrontConnected() override {
        fmtlog::setThreadName("MdGateway");
        logi("{} OnFrontConnected", name);
        this->ReqUserLogin();
    }

    void OnFrontDisconnected(int nReason) override {
        logi("{} OnFrontDisconnected reson=[{}]", name, nReason);
        this->setStatus(false);
    }

    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                        bool bIsLast) override {
        logi("{} OnRspUserLogin", name);
        if (bIsLast && pRspInfo->ErrorID == 0) {
            this->tradingDay = this->m_pUserApi->GetTradingDay();
            logi("{}  行情接口连接成功,交易日 = {}", name, this->tradingDay);
            this->setStatus(true);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            //重新订阅
            this->reSubscribe();
        } else {
            loge("{} 行情接口连接失败, ErrorMsg={}", name, pRspInfo->ErrorMsg);
        }
    }

    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo,
                            int nRequestID, bool bIsLast) override {
        if (pRspInfo->ErrorID == 0) {
            logi("{} OnRspSubMarketData {}", name, pSpecificInstrument->InstrumentID);
        } else
            loge("{} OnRspSubMarketData fail {} {}", name, pSpecificInstrument->InstrumentID, pRspInfo->ErrorMsg);

    }

    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) override {
        Tick *tickData = new Tick();
        tickData->tradingDay = pDepthMarketData->TradingDay;
        tickData->symbol = pDepthMarketData->InstrumentID;
        tickData->exchange = pDepthMarketData->ExchangeID;
        tickData->lastPrice = pDepthMarketData->LastPrice;
        tickData->preSettlePrice = pDepthMarketData->PreSettlementPrice;
        tickData->openPrice = pDepthMarketData->OpenPrice;
        tickData->volume = pDepthMarketData->Volume;
        tickData->updateTime =
                Util::str2time(string(pDepthMarketData->UpdateTime)) + pDepthMarketData->UpdateMillisec / 1000.0;
        tickData->bidPrice1 = pDepthMarketData->BidPrice1 == DBL_MAX ? 0 : pDepthMarketData->BidPrice1;
        tickData->bidPrice2 = pDepthMarketData->BidPrice2 == DBL_MAX ? 0 : pDepthMarketData->BidPrice2;
        tickData->bidPrice3 = pDepthMarketData->BidPrice3 == DBL_MAX ? 0 : pDepthMarketData->BidPrice3;
        tickData->bidPrice4 = pDepthMarketData->BidPrice4 == DBL_MAX ? 0 : pDepthMarketData->BidPrice4;
        tickData->bidPrice5 = pDepthMarketData->BidPrice5 == DBL_MAX ? 0 : pDepthMarketData->BidPrice5;
        tickData->askPrice1 = pDepthMarketData->AskPrice1 == DBL_MAX ? 0 : pDepthMarketData->AskPrice1;
        tickData->askPrice2 = pDepthMarketData->AskPrice2 == DBL_MAX ? 0 : pDepthMarketData->AskPrice2;
        tickData->askPrice3 = pDepthMarketData->AskPrice3 == DBL_MAX ? 0 : pDepthMarketData->AskPrice3;
        tickData->askPrice4 = pDepthMarketData->AskPrice4 == DBL_MAX ? 0 : pDepthMarketData->AskPrice4;
        tickData->askPrice5 = pDepthMarketData->AskPrice5 == DBL_MAX ? 0 : pDepthMarketData->AskPrice5;
        tickData->bidVolume1 = pDepthMarketData->BidVolume1;
        tickData->askVolume1 = pDepthMarketData->AskVolume1;

        tickData->recvTsc = Context::get().tn.rdtsc();
        this->getQueue()->push(Event{EvType::TICK, tickData->recvTsc, tickData});
        //logi("MD OnRtnTick instrument=[{}] time=[{}] price=[{}]",
        //     pDepthMarketData->InstrumentID, pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice);
    }

    ///错误应答
    void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {
        loge("{} OnRspError {}", name, pRspInfo->ErrorMsg);
    }


    void subscribe(set<string> &subContracts) override {
        char **str = new char *[subContracts.size() + 1];
        int i = 0;
        for (auto &item: subContracts) {
            if (this->quotaInfo->subSet.count(item) > 0)
                continue;
            this->quotaInfo->subSet.insert(item);
            str[i++] = const_cast<char *>(item.c_str());
        }
        if (i > 0 && this->connected) {
            int ret = this->m_pUserApi->SubscribeMarketData(str, i);
            logi("{} subscribeContract count:{} ret = {}", name, i, ret);
        }
    }

    int connect() override {
        void *handle = dlopen("lib/ctp/thostmduserapi_se.so", RTLD_LAZY);
        if (handle == nullptr) {
            logi("{} load thosttraderapi.dll fail  [{}]", name, errno, dlerror());
            return -1;
        }

        //查看函数符号  objdump -tT ctp/thostmduserapi_se.so  |grep CreateFtdcMdApi
        //或者nm -D   （-T 的行表示导出函数）
        typedef CThostFtdcMdApi *(*CreateApiMdFunc)(const char *);
        CreateApiMdFunc pfnCreateFtdcMdApiFunc = (CreateApiMdFunc) dlsym(handle,
                                                                         "_ZN15CThostFtdcMdApi15CreateFtdcMdApiEPKcbb");
        if (pfnCreateFtdcMdApiFunc == nullptr) {
            logi("{} load thosttraderapi.so fail [{}] [{}]", name, errno, hstrerror(errno));
            return -1;
        }
        m_pUserApi = pfnCreateFtdcMdApiFunc(".");
        m_pUserApi->RegisterSpi(this);

        std::thread t(std::bind(&CtpMdGateway::Run, this));
        t.detach();
        return 0;
    }

    void disconnect() override {

    }

    void reSubscribe() {
        if (this->quotaInfo->subSet.size() > 0) {
            char **str = new char *[this->quotaInfo->subSet.size() + 1];
            int i = 0;
            for (auto &item: this->quotaInfo->subSet) {
                str[i++] = const_cast<char *>(item.c_str());
            }
            int ret = this->m_pUserApi->SubscribeMarketData(str, i);
            logi("{} reSubscribe  count={} ret = {}", name, this->quotaInfo->subSet.size(), ret);
        }
    }

    CMultiDelegate<void, Tick *> OnTickData;


private:
    string name = "MdGateway";
    static inline int nRequestID = 0;
    // 指向CThostFtdcMduserApi实例的指针
    CThostFtdcMdApi *m_pUserApi;
    string tradingDay;

    //LockFreeQueue<Event>* queue;
    void Run() {
        const char *address = this->quotaInfo->address.c_str();
        m_pUserApi->RegisterFront(const_cast<char *>(address));
        m_pUserApi->Init();
        logi("{} ctp md connecting...{}", name, address);
        m_pUserApi->Join();
    }
};


#endif //TRADECORE_CTPMDAPI_H
