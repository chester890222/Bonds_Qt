#设置是否使用base库
#MYBASE += BaseQuant BaseView BaseMarket BaseTrade BaseSQL

#设置是否编译交易接口useXtTraderApi useCTP useKCBP
#DEFINES += useWindTrade useXtTraderApi useCTP useKCBP

#设置是否编译行情接口
DEFINES += useWindMarket useTDF


contains(MYBASE,BaseQuant):INCLUDEPATH += ../BaseQuant
contains(MYBASE,BaseView):INCLUDEPATH += ../BaseView
contains(MYBASE,BaseMarket):INCLUDEPATH += ../BaseMarket
contains(MYBASE,BaseTrade):INCLUDEPATH += ../BaseTrade
contains(MYBASE,BaseSQL):INCLUDEPATH += ../BaseSQL
contains(MYBASE,BaseBond):INCLUDEPATH += ../BaseBond

win32{
        #    LIBS += ../bin/BaseQuant.dll
        #    LIBS += ../bin/BaseView.dll
        #    LIBS += ../bin/BaseTrade.dll
        #libBaseQuant和libBaseView的顺序很重要，如果颠倒，会编译报错，哎，坑了半天
    !win32-g++{
        contains(MYBASE,BaseQuant):LIBS += ../bin/lib/BaseQuant.lib
        contains(MYBASE,BaseView):LIBS += ../bin/lib/BaseView.lib
        contains(MYBASE,BaseMarket):LIBS += ../bin/lib/BaseMarket.lib
        contains(MYBASE,BaseTrade):LIBS += ../bin/lib/BaseTrade.lib
        contains(MYBASE,BaseSQL):LIBS += ../bin/lib/BaseSQL.lib
        contains(MYBASE,BaseBond):LIBS += ../bin/lib/BaseBond.lib
        contains(MYBASE,BaseTrade) {
            contains(DEFINES,useXtTraderApi){
                LIBS+=$$PWD/BaseTrade/MyXtTrader/XtTraderApi/XtTraderApi.lib
            }
            contains(DEFINES,useCTP) {
                LIBS += $$PWD/BaseTrade/MyCTP/ThostTraderApi/Market/thostmduserapi.lib
                LIBS += $$PWD/BaseTrade/MyCTP/ThostTraderApi/Trader/thosttraderapi.lib
            }
            contains(DEFINES,useKCBP) {
                #现在Encode是使用dll动态加载，可以平台无关
                #    LIBS += $$PWD/KCBPCli/KDEncodeCli.lib
                LIBS += $$PWD/BaseTrade/MyKCBP/KCBPCli/KCBPCli.lib
            }

            #默认使用wind交易
            contains(DEFINES,useWindTrade) {
                LIBS += $$PWD/BaseTrade/MyWind/Wind/Quant/lib/Win32/WindQuantData.lib
                LIBS += $$PWD/BaseTrade/MyWind/Wind/Trade/lib/win32/WindTradeAPI.lib
            }
        }

        contains(MYBASE,BaseMarket) {
            contains(DEFINES,useTDF) {
                LIBS += $$PWD/BaseMarket/TDF/lib/Win32Release/tdfapi_jni.lib
                LIBS += $$PWD/BaseMarket/TDF/lib/Win32Release/TDFAPI25.lib
            }

            #默认使用wind行情
            contains(DEFINES,useWindMarket) {
                LIBS += $$PWD/BaseMarket/Wind/Quant/lib/Win32/WindQuantData.lib
            }
        }
    } else {
#        LIBS += ../bin/lib/libBaseQuant.a
#        LIBS += ../bin/lib/libBaseView.a
#        LIBS += ../bin/lib/libBaseTrade.a
    }
}



        #引入quantlib金融工具定价计算引擎
        #LIBS += -L$$PWD/../BaseQuant/QuantLib/lib/ -lQuantLib-mgw
