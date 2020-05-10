#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include"globle.h"
#include"model.h"
#include"service.h"
#include"card_service.h"
#include"card_file.h"

extern CardNode* cardLinkList;

//将卡插入文件
int addCard(Card card) {
	if (saveCardByB(&card, CARDPATH) == 0) {
		return 0;
	}
	return 1;
}

//查询链表中符合条件项
Card* queryCard(const char* pName, int* pIndex) {
	//获得卡信息
	getCardByB();
	Card* cards = (Card*)malloc(sizeof(Card)*MAX_QUERY);
	int index = 0;//记录符合元素个数
	int keyLen = strlen(pName);//所查卡号长度
	//遍历链表
	CardNode* q = cardLinkList->next;
	typedef struct {
		char son[18];
	} myArray1;
	while (q != NULL) {
		int qLen = strlen(q->data.aName);//链表中卡号长度
		if (qLen < keyLen) {//卡号长于所查卡号才可查
			q = q->next;
			continue;
		}
		int strNum = qLen - keyLen + 1;//子串数量
		//子串
		myArray1* str = (myArray1*)malloc(sizeof(myArray1)*strNum);
		//给子串赋值	
		int kIdx = 0;//控制总子串角标
		int mIdx = 0;//控制父串开始处
		int nIdx = 0;//控制父串角标
		int jIdx = 0;//控制小子串角标
		for (mIdx = 0; mIdx < (qLen - keyLen + 1); mIdx++) {//移动开始头
			nIdx = mIdx;
			for (jIdx = 0; jIdx < keyLen; jIdx++) {//给小子串复制
				str[kIdx].son[jIdx] = q->data.aName[nIdx];
				nIdx++;
			}
			str[kIdx].son[jIdx] = '\0';
			kIdx++;
		}
		//挨个比较
		for (int i = 0; i < strNum; i++) {
			if (strcmp(str[i].son, pName) == 0) {
				cards[index] = q->data;
				index++;
				break;
			}
		}
		q = q->next;
	}
	(*pIndex) = index;
	//清空链表,防止再次查询时重复
	cardListClean(&cardLinkList);
	return cards;
}

//查找链表中是否含有相同卡号
Card* checkCard(const CardNode* pLinkList, const char* pName) {
	Card* card = NULL;
 	CardNode* p = pLinkList->next;
	if (p == NULL) {
		return card;
	}
	while (p != NULL) {
		if (strcmp(p->data.aName, pName) == 0) {
			card = &(p->data);
			break;
		}
		p = p->next;
	}
	return card;
}

//查找链表中是否含有相同卡号
Card* checkCard_(const CardNode* pLinkList, const char* pName, int* nIndex) {
	Card* card = NULL;
	CardNode* p = pLinkList->next;
	(*nIndex) = getCardCountByB(CARDPATH);
	if (p == NULL) {
		return card;
	}
	while (p != NULL) {
		if (strcmp(p->data.aName, pName) == 0) {
			card = &(p->data);
			break;
		}
		p = p->next;
		(*nIndex)--;
	}
	return card;
}


//将卡信息从文件读到链表
int getCard() {
	//清空链表
	cardListClean(&cardLinkList);
	int n = getCardCount(CARDPATH);
	Card* pCard = (Card*)malloc(sizeof(Card)*n);
	//读取卡
	if (readCard(pCard, CARDPATH) == 0) {
		return 0;
	}
	//放入链表
	for (int i = 0; i < n; i++) {
		cardListInsert(&cardLinkList, pCard[i]);
	}
	//释放fCard
	if (pCard != NULL) {
		free(pCard);
		pCard = NULL;
	}
	return 1;
}

//将卡信息从文件读到链表
int getCardByB() {
	//清空链表
	cardListClean(&cardLinkList);
	int n = getCardCountByB(CARDPATH);
	Card* pCard = (Card*)malloc(sizeof(Card)*n);
	//读取卡
	if (readCardByB(pCard, CARDPATH) == 0) {
		return 0;
	}
	//放入链表
	for (int i = 0; i < n; i++) {
		cardListInsert(&cardLinkList, pCard[i]);
	}
	//释放fCard
	if (pCard != NULL) {
		free(pCard);
		pCard = NULL;
	}
	return 1;
}

//初始化卡信息节点
void initCardNode(CardNode** L) {
	*L = (CardNode*)malloc(sizeof(CardNode));
	(*L)->next = NULL;
}

//插入卡信息节点
void cardListInsert(CardNode** L, Card card) {
	CardNode* p = NULL;
	initCardNode(&p);
	p->data = card;
	p->next = (*L)->next;
	(*L)->next = p;
}

//清空卡信息链表
void cardListClean(CardNode** LinkList) {
	while ((*LinkList)->next != NULL) {
		CardNode* p = (*LinkList)->next;
		(*LinkList)->next = p->next;
		free(p);
	}
}
