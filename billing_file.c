#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include"model.h"
#include"globle.h"
#include"billing_file.h"

//�����Ѽ�¼�����ļ�
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
	int n = getBillingCount(BILLINGPATH);//��ȡ������Ϣ�ļ�����Ϣ����
	FILE* fp;
	if ((fp = fopen(BILLINGPATH, "rb")) == NULL) {
		return 0;
	}
	//�����������
	for (int i = 0; i < n; i++) {
		if (fread(&pBilling[i], sizeof(Billing), 1, fp) == 0) {
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

//��ȡ������Ϣ�ļ�����Ϣ����
int getBillingCount(const char* pPath) {
	FILE* fp;
	if ((fp = fopen(pPath, "rb")) == NULL) {
		return 0;
	}
	int n = 0;//��¼����
	Billing* billing = (Billing*)malloc(sizeof(Billing));
	//��������ļ�����¼����
	while (!feof(fp)) {
		if (fread(billing, sizeof(Billing), 1, fp) != 0) {
			n++;
		}
	}
	fclose(fp);//�ر��ļ�
	//�ͷ�billing
	if (billing != NULL) {
		free(billing);
		billing = NULL;
	}
	return n;
}

//����������Ϣ��Ϣ
int updateBilling(const Billing* pBilling, const char* pPath, int nIndex) {
	FILE* fp = NULL;
	Billing billing;
	int nLine = 1;
	long lPosition = 0;
	if ((fp = fopen(pPath, "r+")) == NULL) {
		return 0;
	}
	//��������Ϣ���и���
	while (!feof(fp) && nLine < nIndex) {
		if (fread(&billing, sizeof(Billing), 1, fp) != 0) {
			lPosition = ftell(fp);
			nLine++;
		}
	}
	fseek(fp, lPosition, SEEK_SET);
	fwrite(pBilling, sizeof(Billing), 1, fp);
	
	fclose(fp);//�ر�
	return 1;
}