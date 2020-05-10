#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>

#include"model.h"
#include"globle.h"
#include"billing_service.h"
#include"billing_file.h"


extern BillingNode* billingLinkList;

//添加消费信息
int addBilling(Billing* billing) {
	if (saveBilling(billing, BILLINGPATH) == 0) {
		return 0;
	}
	return 1;
}

Billing* queryBilling(const BillingNode* pLinkList, const char* pName) {
	Billing* billing = NULL;
	BillingNode* p = pLinkList->next;
	while (p != NULL) {
		if (strcmp(p->data.aCardName, pName) == 0 && p->data.nStatus != 1) {
			billing = &(p->data);
			break;
		}
		p = p->next;
	}
	return billing;
}

Billing* queryBilling_(const BillingNode* pLinkList, const char* pName, int* nIndex) {
	Billing* billing = NULL;
	BillingNode* p = pLinkList->next;
	(*nIndex) = getBillingCount(BILLINGPATH);
	while (p != NULL) {
		if (strcmp(p->data.aCardName, pName) == 0 && p->data.nStatus != 1) {
			billing = &(p->data);
			break;
		}
		p = p->next;
		(*nIndex)--;
	}
	return billing;
}


//将文件中的消费信息读入链表
int getBilling() {
	billingListClean(&billingLinkList);//清空链表避免重复
	int n = getBillingCount(BILLINGPATH);
	Billing* pBilling = (Billing*)malloc(sizeof(Billing)*n);
	//读取卡
	if (readBilling(pBilling, BILLINGPATH) == 0) {
		return 0;
	}
	//放入链表
	for (int i = 0; i < n; i++) {
		billingListInsert(&billingLinkList, pBilling[i]);
	}
	//释放pBilling
	if (pBilling != NULL) {
		free(pBilling);
		pBilling = NULL;
	}
	return 1;
}

//初始化消费信息节点
void initBillingNode(BillingNode** L) {
	*L = (BillingNode*)malloc(sizeof(BillingNode));
	(*L)->next = NULL;
}

//初始化消费信息节点
void billingListInsert(BillingNode** L, Billing billing) {
	BillingNode* p = NULL;
	initBillingNode(&p);
	p->data = billing;
	p->next = (*L)->next;
	(*L)->next = p;
}

//清空消费信息节点
void billingListClean(BillingNode** L) {
	while ((*L)->next != NULL) {
		BillingNode* p = (*L)->next;
		(*L)->next = p->next;
		free(p);
	}
}