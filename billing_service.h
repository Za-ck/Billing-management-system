#pragma once
#include"model.h"

int addBilling(Billing* billing);
Billing* queryBilling(const BillingNode* pLinkList, const char* pName);
Billing* queryBilling_(const BillingNode* pLinkList, const char* pName, int* nIndex);
int getBilling();
void initBillingNode(BillingNode** L);
void billingListInsert(BillingNode** L, Billing billing);
void billingListClean(BillingNode** L);

