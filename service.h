#pragma once
#include"model.h"

int addCardInfo(Card card);
Card* queryCardInfo(const char* pName, int* pIndex);
int doLogon(const char* pName, const char* pPwd, LogonInfo* info);
int doSettle(const char* pName, const char* pPwd, SettleInfo* info);
int doAddMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo);
int doRefundMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo);
int annulCard(char* pName, char* pPwd,  float* fBal);
double getAmount(time_t tStart);
int doCode();
char* timeToString(const time_t t);
time_t stringToTime_t(const char* strT);
