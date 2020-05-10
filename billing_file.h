#pragma once
#include"model.h"
int saveBilling(const Billing* billing, const char* pPath);
int readBilling(const Billing* pBilling, const char* pPath);
int getBillingCount(const char* pPath);
int updateBilling(const Billing* pBilling, const char* pPath, int nIndex);
