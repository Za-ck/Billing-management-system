#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include"model.h"
#include"globle.h"
#include"billing_file.h"

//将消费记录存入文件
int saveBilling(const Billing* billing, const char* pPath) {
	FILE* fp;
	if ((fp = fopen(pPath, "ab")) == NULL) {
		if ((fp = fopen(pPath, "wb")) == NULL) {
			return 0;
		}
	}
	fwrite(billing, sizeof(Billing), 1, fp);
	fclose(fp);
	return 1;
}

int readBilling(Billing* pBilling, const char* pPath) {
	int n = getBillingCount(BILLINGPATH);//获取消费信息文件中信息数量
	FILE* fp;
	if ((fp = fopen(BILLINGPATH, "rb")) == NULL) {
		return 0;
	}
	//逐个读入数组
	for (int i = 0; i < n; i++) {
		if (fread(&pBilling[i], sizeof(Billing), 1, fp) == 0) {
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

//读取消费信息文件中信息数量
int getBillingCount(const char* pPath) {
	FILE* fp;
	if ((fp = fopen(pPath, "rb")) == NULL) {
		return 0;
	}
	int n = 0;//记录数量
	Billing* billing = (Billing*)malloc(sizeof(Billing));
	//逐个遍历文件，记录数量
	while (!feof(fp)) {
		if (fread(billing, sizeof(Billing), 1, fp) != 0) {
			n++;
		}
	}
	fclose(fp);//关闭文件
	//释放billing
	if (billing != NULL) {
		free(billing);
		billing = NULL;
	}
	return n;
}

//更新消费信息信息
int updateBilling(const Billing* pBilling, const char* pPath, int nIndex) {
	FILE* fp = NULL;
	Billing billing;
	int nLine = 1;
	long lPosition = 0;
	if ((fp = fopen(pPath, "r+")) == NULL) {
		return 0;
	}
	//遍历卡信息进行更新
	while (!feof(fp) && nLine < nIndex) {
		if (fread(&billing, sizeof(Billing), 1, fp) != 0) {
			lPosition = ftell(fp);
			nLine++;
		}
	}
	fseek(fp, lPosition, SEEK_SET);
	fwrite(pBilling, sizeof(Billing), 1, fp);
	
	fclose(fp);//关闭
	return 1;
}