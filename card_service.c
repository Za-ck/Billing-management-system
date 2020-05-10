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

//���������ļ�
int addCard(Card card) {
	if (saveCardByB(&card, CARDPATH) == 0) {
		return 0;
	}
	return 1;
}

//��ѯ�����з���������
Card* queryCard(const char* pName, int* pIndex) {
	//��ÿ���Ϣ
	getCardByB();
	Card* cards = (Card*)malloc(sizeof(Card)*MAX_QUERY);
	int index = 0;//��¼����Ԫ�ظ���
	int keyLen = strlen(pName);//���鿨�ų���
	//��������
	CardNode* q = cardLinkList->next;
	typedef struct {
		char son[18];
	} myArray1;
	while (q != NULL) {
		int qLen = strlen(q->data.aName);//�����п��ų���
		if (qLen < keyLen) {//���ų������鿨�Ųſɲ�
			q = q->next;
			continue;
		}
		int strNum = qLen - keyLen + 1;//�Ӵ�����
		//�Ӵ�
		myArray1* str = (myArray1*)malloc(sizeof(myArray1)*strNum);
		//���Ӵ���ֵ	
		int kIdx = 0;//�������Ӵ��Ǳ�
		int mIdx = 0;//���Ƹ�����ʼ��
		int nIdx = 0;//���Ƹ����Ǳ�
		int jIdx = 0;//����С�Ӵ��Ǳ�
		for (mIdx = 0; mIdx < (qLen - keyLen + 1); mIdx++) {//�ƶ���ʼͷ
			nIdx = mIdx;
			for (jIdx = 0; jIdx < keyLen; jIdx++) {//��С�Ӵ�����
				str[kIdx].son[jIdx] = q->data.aName[nIdx];
				nIdx++;
			}
			str[kIdx].son[jIdx] = '\0';
			kIdx++;
		}
		//�����Ƚ�
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
	//�������,��ֹ�ٴβ�ѯʱ�ظ�
	cardListClean(&cardLinkList);
	return cards;
}

//�����������Ƿ�����ͬ����
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

//�����������Ƿ�����ͬ����
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


//������Ϣ���ļ���������
int getCard() {
	//�������
	cardListClean(&cardLinkList);
	int n = getCardCount(CARDPATH);
	Card* pCard = (Card*)malloc(sizeof(Card)*n);
	//��ȡ��
	if (readCard(pCard, CARDPATH) == 0) {
		return 0;
	}
	//��������
	for (int i = 0; i < n; i++) {
		cardListInsert(&cardLinkList, pCard[i]);
	}
	//�ͷ�fCard
	if (pCard != NULL) {
		free(pCard);
		pCard = NULL;
	}
	return 1;
}

//������Ϣ���ļ���������
int getCardByB() {
	//�������
	cardListClean(&cardLinkList);
	int n = getCardCountByB(CARDPATH);
	Card* pCard = (Card*)malloc(sizeof(Card)*n);
	//��ȡ��
	if (readCardByB(pCard, CARDPATH) == 0) {
		return 0;
	}
	//��������
	for (int i = 0; i < n; i++) {
		cardListInsert(&cardLinkList, pCard[i]);
	}
	//�ͷ�fCard
	if (pCard != NULL) {
		free(pCard);
		pCard = NULL;
	}
	return 1;
}

//��ʼ������Ϣ�ڵ�
void initCardNode(CardNode** L) {
	*L = (CardNode*)malloc(sizeof(CardNode));
	(*L)->next = NULL;
}

//���뿨��Ϣ�ڵ�
void cardListInsert(CardNode** L, Card card) {
	CardNode* p = NULL;
	initCardNode(&p);
	p->data = card;
	p->next = (*L)->next;
	(*L)->next = p;
}

//��տ���Ϣ����
void cardListClean(CardNode** LinkList) {
	while ((*LinkList)->next != NULL) {
		CardNode* p = (*LinkList)->next;
		(*LinkList)->next = p->next;
		free(p);
	}
}
