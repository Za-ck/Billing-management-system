#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"model.h"
#include"globle.h"

int saveMoney(const Money* pMoney, const char* pPath) {
	FILE* fp = NULL;
	if ((fp = fopen(MONEYPATH, "ab")) == NULL) {
		if ((fp = fopen(MONEYPATH, "wb")) == NULL) {
			return 0;
		}
	}
	if (fwrite(pMoney, sizeof(Money), 1, fp) != 1) {
		return 0;
	}
	fclose(fp);
	return 1;
}