#pragma once
#include"model.h"

int saveCard(const Card* pCard, const char* pPath);
int saveCardByB(const Card* pCard, const char* pPath);
int readCard(Card* pCard, const char* pPath);
int readCardByB(Card* pCard, const char* pPath);
void prase(char** p, char* str);
Card praseCard(char* pBuf);
int getCardCount(const char* pPath);
int getCardCountByB(const char* pPath);
int updateCard(const Card* pCard, const char* pPath, int nIndex);
int updateCardByB(const Card* pCard, const char* pPath, int nIndex);
