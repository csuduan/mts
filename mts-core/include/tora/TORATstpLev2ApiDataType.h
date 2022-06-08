/////////////////////////////////////////////////////////////////////////
///@company 上海泰琰信息科技有限公司
///@file TORATstpLev2ApiStruct.h
///@brief 定义了客户端接口使用的业务数据结构
///@history 
/////////////////////////////////////////////////////////////////////////

#ifndef _TORA_TSTPLEV2DATATYPE_H
#define _TORA_TSTPLEV2DATATYPE_H

#include <limits.h>
#include <float.h>

namespace TORALEV2API
{
	///定义空值
	const int INT_NULL_VAL = INT_MAX;
	const double FLOAT_NULL_VAL = DBL_MAX;
	const char CHAR_NULL_VAL = 0;
	const short WORD_NULL_VAL = SHRT_MAX;
#ifdef WINDOWS
	const __int64 LONG_NULL_VAL = _I64_MAX;
#else
	const long long LONG_NULL_VAL = LLONG_MAX;
#endif
	
	///整型数据置空和判空
	inline void set_null(int &v)
	{
		v = INT_NULL_VAL;
	}
	inline bool is_null(const int &v)
	{
		return v == INT_NULL_VAL;
	}
	
	///浮点型数据置空和判空
	inline void set_null(double &v)
	{
		v = FLOAT_NULL_VAL;
	}
	inline bool is_null(const double &v)
	{
		return v == FLOAT_NULL_VAL;
	}
	
	///字符型数据置空和判空
	inline void set_null(char &v)
	{
		v = CHAR_NULL_VAL;
	}
	inline bool is_null(const char &v)
	{
		return v == CHAR_NULL_VAL;
	}
	
	///短整型数据置空和判空
	inline void set_null(short &v)
	{
		v = WORD_NULL_VAL;
	}
	inline bool is_null(const short &v)
	{
		return v == WORD_NULL_VAL;
	}
	
	///长整型数据置空和判空
	#ifdef WINDOWS
	inline void set_null(__int64 &v)
	#else
	inline void set_null(long long &v)
	#endif
	{
		v = LONG_NULL_VAL;
	}
	#ifdef WINDOWS
	inline bool is_null(const __int64 &v)
	#else
	inline bool is_null(const long long &v)
	#endif
	{
		return v == LONG_NULL_VAL;
	}
	
	///字符串型数据置空和判空
	inline void set_null(char *v)
	{
		v[0] = '\0';
	}
	
	inline bool is_null(const char *v)
	{
		const char *p=v;
		while (*p)
		{
			if (*p!=' ')
			{
				return false;
			}
			p++;
		}
		return true;
	}


	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpDateType是一个日期类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpDateType[9];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTimeType是一个时间类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpTimeType[9];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPriceType是一个价格类型
	/////////////////////////////////////////////////////////////////////////
	typedef double TTORATstpPriceType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpMoneyType是一个资金类型
	/////////////////////////////////////////////////////////////////////////
	typedef double TTORATstpMoneyType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpVolumeType是一个数量类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpVolumeType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpBoolType是一个布尔型类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpBoolType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpSequenceNoType是一个流水号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpSequenceNoType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCommFluxType是一个通讯流量类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpCommFluxType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpDeviceIDType是一个设备标识类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpDeviceIDType[129];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCertSerialType是一个认证序列类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpCertSerialType[129];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpExchangeIDType是一个交易所代码类型
	/////////////////////////////////////////////////////////////////////////
	///通用(内部使用)
	const char TORA_TSTP_EXD_COMM = '0';
	///上海交易所
	const char TORA_TSTP_EXD_SSE = '1';
	///深圳交易所
	const char TORA_TSTP_EXD_SZSE = '2';
	///香港交易所
	const char TORA_TSTP_EXD_HK = '3';
	typedef char TTORATstpExchangeIDType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpAuthModeType是一个认证方式类型
	/////////////////////////////////////////////////////////////////////////
	///密码
	const char TORA_TSTP_AM_Password = '0';
	///指纹
	const char TORA_TSTP_AM_FingerPrint = '1';
	///钥匙串
	const char TORA_TSTP_AM_CertInfo = '2';
	typedef char TTORATstpAuthModeType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpMDSubModeType是一个行情订阅模式类型
	/////////////////////////////////////////////////////////////////////////
	///TCP连接模式
	const char TORA_TSTP_MST_TCP = '0';
	///UDP单播模式
	const char TORA_TSTP_MST_UDP = '1';
	///UDP组播模式
	const char TORA_TSTP_MST_MCAST = '2';
	typedef char TTORATstpMDSubModeType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpUserIDType是一个交易用户代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpUserIDType[16];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPasswordType是一个密码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpPasswordType[41];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpFrontIDType是一个前置编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpFrontIDType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpSessionIDType是一个会话编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpSessionIDType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpProductInfoType是一个产品信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpProductInfoType[11];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpProtocolInfoType是一个协议信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpProtocolInfoType[11];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCountType是一个计数类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpCountType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpErrorIDType是一个错误代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpErrorIDType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpErrorMsgType是一个错误信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpErrorMsgType[81];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpSecurityIDType是一个证券代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpSecurityIDType[31];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpDepartmentIDType是一个部门代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpDepartmentIDType[11];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTerminalInfoType是一个终端信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpTerminalInfoType[256];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpIPAddressType是一个IP地址类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpIPAddressType[16];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpMacAddressType是一个Mac地址类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpMacAddressType[21];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPortType是一个端口号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpPortType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLangType是一个语言类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpLangType[17];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTimeStampType是一个时间戳类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpTimeStampType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOrderRefType是一个报单引用类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpOrderRefType[13];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpUserNameType是一个用户名称类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpUserNameType[81];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpMobileType是一个手机类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpMobileType[41];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLongVolumeType是一个LongVolume类型
	/////////////////////////////////////////////////////////////////////////
	#ifdef WINDOWS
	typedef __int64 TTORATstpLongVolumeType;
	#else
	typedef long long int TTORATstpLongVolumeType;
	#endif

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLongSequenceType是一个序号类型
	/////////////////////////////////////////////////////////////////////////
	#ifdef WINDOWS
	typedef __int64 TTORATstpLongSequenceType;
	#else
	typedef long long int TTORATstpLongSequenceType;
	#endif

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpUserTypeType是一个用户类型类型
	/////////////////////////////////////////////////////////////////////////
	///经纪公司用户
	const char TORA_TSTP_UTYPE_BrokerUser = '0';
	///超级用户
	const char TORA_TSTP_UTYPE_SuperUser = '1';
	///投资者用户
	const char TORA_TSTP_UTYPE_Investor = '2';
	typedef char TTORATstpUserTypeType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpSystemNameType是一个系统名称类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpSystemNameType[41];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLogInAccountType是一个登录账户类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpLogInAccountType[21];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLogInAccountTypeType是一个登录账户类型类型
	/////////////////////////////////////////////////////////////////////////
	///用户代码
	const char TORA_TSTP_LACT_UserID = '0';
	///资金账号
	const char TORA_TSTP_LACT_AccountID = '1';
	///上海A股
	const char TORA_TSTP_LACT_SHAStock = '2';
	///深圳A股
	const char TORA_TSTP_LACT_SZAStock = '3';
	///上海B股
	const char TORA_TSTP_LACT_SHBStock = '4';
	///深圳B股
	const char TORA_TSTP_LACT_SZBStock = '5';
	///三板A
	const char TORA_TSTP_LACT_ThreeNewBoardA = '6';
	///三板B
	const char TORA_TSTP_LACT_ThreeNewBoardB = '7';
	///港股
	const char TORA_TSTP_LACT_HKStock = '8';
	///统一用户代码
	const char TORA_TSTP_LACT_UnifiedUserID = '9';
	typedef char TTORATstpLogInAccountTypeType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpIntInfoType是一个整形附加信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpIntInfoType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLOrderTypeType是一个订单类别类型
	/////////////////////////////////////////////////////////////////////////
	///市价
	const char TORA_TSTP_LOT_Market = '1';
	///限价
	const char TORA_TSTP_LOT_Limit = '2';
	///本方最优
	const char TORA_TSTP_LOT_HomeBest = '3';
	typedef char TTORATstpLOrderTypeType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLSideType是一个委托方向类型
	/////////////////////////////////////////////////////////////////////////
	///买
	const char TORA_TSTP_LSD_Buy = '1';
	///卖
	const char TORA_TSTP_LSD_Sell = '2';
	///借入
	const char TORA_TSTP_LSD_Borrow = '3';
	///借出
	const char TORA_TSTP_LSD_Lend = '4';
	typedef char TTORATstpLSideType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpExecTypeType是一个成交类别类型
	/////////////////////////////////////////////////////////////////////////
	///成交
	const char TORA_TSTP_ECT_Fill = '1';
	///撤销
	const char TORA_TSTP_ECT_Cancel = '2';
	///未知
	const char TORA_TSTP_ECT_Unknown = 'N';
	typedef char TTORATstpExecTypeType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLOrderStatusType是一个订单状态类型
	/////////////////////////////////////////////////////////////////////////
	///新增
	const char TORA_TSTP_LOS_Add = 'A';
	///删除
	const char TORA_TSTP_LOS_Delete = 'D';
	typedef char TTORATstpLOrderStatusType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpHDSerialType是一个硬盘序列号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpHDSerialType[33];

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpMDSecurityStatType是一个行情证券状态类型
	/////////////////////////////////////////////////////////////////////////
	///开盘前
	const char TORA_TSTP_MSST_PreOpen = '0';
	///集合竞价
	const char TORA_TSTP_MSST_CallAuction = '1';
	///连续交易
	const char TORA_TSTP_MSST_Continous = '2';
	///休市
	const char TORA_TSTP_MSST_Pause = '3';
	///停牌
	const char TORA_TSTP_MSST_Suspend = '4';
	///长期停牌
	const char TORA_TSTP_MSST_LongSuspend = '5';
	///波动性中断
	const char TORA_TSTP_MSST_UndulationInt = '6';
	///熔断可恢复
	const char TORA_TSTP_MSST_CircuitBreak = '7';
	///熔断不可恢复
	const char TORA_TSTP_MSST_CircuitBreakU = '8';
	///闭市
	const char TORA_TSTP_MSST_Close = '9';
	///其它
	const char TORA_TSTP_MSST_Other = 'a';
	///收盘集合竞价
	const char TORA_TSTP_MSST_CloseCallAuction = 'b';
	///集中撮合(盘后定价)
	const char TORA_TSTP_MSST_CallMatch = 'c';
	///连续交易(盘后定价)
	const char TORA_TSTP_MSST_PostContinous = 'd';
	///闭市(盘后定价)
	const char TORA_TSTP_MSST_PostClose = 'e';
	///开盘前(盘后定价)
	const char TORA_TSTP_MSST_PrePostOpen = 'f';
	///未上市
	const char TORA_TSTP_MSST_Unlisted = 'g';
	typedef char TTORATstpMDSecurityStatType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTradeBSFlagType是一个内外盘标志类型
	/////////////////////////////////////////////////////////////////////////
	///外盘,主动买
	const char TORA_TSTP_TBSF_Buy = 'B';
	///内盘,主动卖
	const char TORA_TSTP_TBSF_Sell = 'S';
	///未知
	const char TORA_TSTP_TBSF_Unknown = 'N';
	typedef char TTORATstpTradeBSFlagType;

	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLTickTypeType是一个逐笔类型类型
	/////////////////////////////////////////////////////////////////////////
	///新增委托订单
	const char TORA_TSTP_LTT_Add = 'A';
	///删除委托订单
	const char TORA_TSTP_LTT_Delete = 'D';
	///产品状态订单
	const char TORA_TSTP_LTT_Status = 'S';
	///成交
	const char TORA_TSTP_LTT_Trade = 'T';
	typedef char TTORATstpLTickTypeType;
}
#endif