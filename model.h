#pragma once
#include<time.h>

//卡信息
typedef struct Card {
	char aName[18];//卡号
	char aPwd[8];//密码
	int nStatus;//卡状态 0-未上机；1-正在上机；2-已注销；3-失效
	time_t tStart;//开卡时间
	time_t tEnd;//截止时间
	float fTotalUse;//累计金额
	time_t tLast;//最后使用时间
	int nUseConst;//使用次数
	float fBalance;//余额
	int nDel;//删除标记：0-未删除；1-已删除
} Card;

//卡信息结点
typedef struct IpCardNode {
	Card data;
	struct IpCardNode* next;
}CardNode;

//消费信息
typedef struct Billing {
	char aCardName[18];//卡号
	time_t tStart;//上机时间
	time_t tEnd;//下机时间
	float fAmount;//消费金额
	int nStatus;//消费状态，0-未结算，1-已经结算
	int nDel;//0-未删除，1-已删除
}Billing;

//消费信息节点
typedef struct BillingNode {
	Billing data;
	struct BillingNode* next;
}BillingNode;

//上机信息
typedef struct LogonInfo {
	char aCardName[18];//上机卡号
	time_t tLogen;//上机时间
	float fBalance;//上机时的时间
}LogonInfo;

//下机信息
typedef struct SettleInfo {
	char aCardName[18];//卡号
	time_t tStart;//上机时间
	time_t tEnd;//下机时间
	float fAmount;//消费金额
	float fBalance;//余额
}SettleInfo;

//充值退费结构体
typedef struct Money {
	char aCardName[18];//卡号
	time_t tTime;//充值退费时间
	int nStatus;//状态：0-充值；1-退费
	float fMoney;//充值退费余额
	int nDel;//删除标识：0-未删除，1-已删除
} Money;

//充值退费信息
typedef struct MoneyInfo {
	char aCardName[18];//卡号
	float fMoney;//充值退费余额
	float fBalance;//余额
} MoneyInfo;

