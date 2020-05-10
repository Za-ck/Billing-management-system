#pragma once
#include"model.h"

int addCard(Card card);
Card* queryCard(const char* pName, int* pIndex);
Card* checkCard(const CardNode* pLinkList, const char* pName);
Card* checkCard_(const CardNode* pLinkList, const char* pName, int* nIndex);
int getCard();
int getCardByB();
void initCardNode(CardNode** L);
void cardListInsert(CardNode** L, Card card);
void cardListClean(CardNode** LinkList);
