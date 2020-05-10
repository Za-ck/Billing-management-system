#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>
#include"globle.h"
#include"model.h"
#include"service.h"
#include"card_file.h"

//������Ϣ�����ļ���
int saveCard(const Card* pCard, const char* pPath) {
	FILE* fp;
	//���ļ�
	if ((fp = fopen(pPath, "a")) == NULL) {
		fopen(pPath, "w");
	}
	//�����ݴ����ļ�
	char* strTStart = timeToString(pCard->tStart);
	char* strTEnd = timeToString(pCard->tEnd);
	char* strTLast = timeToString(pCard->tLast);
	fprintf(fp, "%s##%s##%d##%s##%s##%f##%s##%d##%f##%d\n",pCard->aName, pCard->aPwd, 
		pCard->nStatus, strTStart, strTEnd, pCard->fTotalUse, strTLast, 
		pCard->nUseConst, pCard->fBalance, pCard->nDel);
	free(strTStart);
	free(strTEnd);
	free(strTLast);
	//�ر��ļ�
	fclose(fp);
	return 1;
}

int saveCardByB(const Card* pCard, const char* pPath) {
	FILE* fp;
	//���ļ�
	if ((fp = fopen(pPath, "ab")) == NULL) {
		fopen(pPath, "wb");
	}
	//�����ݴ����ļ�
	fwrite(pCard, sizeof(Card), 1, fp);
	fclose(fp);
	return 1;
}

//������Ϣ����������
int readCard(Card* pCard, const char* pPath) {
	int n = getCardCount(pPath);//��ȡ����Ϣ����Ϣ����
	FILE* fp;
	if ((fp = fopen(pPath, "r")) == NULL) {
		return 0;
	}
	char ch[256];
	//���н���ȡ����Ϣ��������
	for (int i = 0; i < n; i++) {
		fgets(ch, 256, fp);
		//���ַ���ת��Ϊ����
		pCard[i] = praseCard(ch);
	}
	fclose(fp);
	return 1;
}

int readCardByB(Card* pCard, const char* pPath) {
	int n = getCardCountByB(pPath);//��ȡ����Ϣ����Ϣ����
	FILE* fp;
	if ((fp = fopen(pPath, "r")) == NULL) {
		return 0;
	}
	for (int i = 0; i < n; i++) {
		if ((fread(&pCard[i], sizeof(Card), 1, fp)) == 0) {
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

//��ȡ�ַ����е�һ����Ϣ
void prase(char** p, char* str) {
	memset(str, 0, sizeof(str));//��ʼ��str
	int i = 0;
	while ((**p) != '#' && (**p) != NULL) {
		str[i] = **p;
		i++;
		(*p)++;
	}
	(*p)++;
	(*p)++;
}

//��ȡ���е���Ϣ
Card praseCard(char* pBuf) {
	Card card;
	char* p = &pBuf[0];//�����ַ���
	char str[32];
	//��ȡ����
	prase(&p, str);
	strcpy(card.aName, str);
	//��ȡ����
	prase(&p, str);
	strcpy(card.aPwd, str);
	//��ȡ��״̬
	prase(&p, str);
	card.nStatus = atoi(str);
	//����ʱ��
	prase(&p, str);
	card.tStart = stringToTime_t(str);
	//��ֹʱ��
	prase(&p, str);
	card.tEnd = stringToTime_t(str);
	//�ۼƽ��
	prase(&p, str);
	card.fTotalUse = (float)atof(str);
	//���ʹ��ʱ��
	prase(&p, str);
	card.tLast = stringToTime_t(str);
	//ʹ�ô���
	prase(&p, str);
	card.nUseConst = atoi(str);
	//��ǰ���
	prase(&p, str);
	card.fBalance = (float)atof(str);
	//ɾ����ʶ
	prase(&p, str);
	card.nDel = atoi(str);
	return card;
} 

//��ȡ���ļ�����Ϣ����
int getCardCount(const char* pPath) {
	FILE* fp;
	if ((fp = fopen(pPath, "r"))== NULL) {
		return 0;
	}
	int n = 0;
	char ch;
	//�ƶ��ļ�ָ��ֱ������
	while (!feof(fp)) {
		ch = fgetc(fp);
		if (ch == '\n') {
			n++;
		}
	}
	fclose(fp);
	return n;
}

int getCardCountByB(const char* pPath) {
	FILE* fp = NULL;
	if ((fp = fopen(pPath, "rb")) == NULL) {
		return 0;
	}
	int n = 0;
	Card* card = (Card*)malloc(sizeof(Card));
	while (!feof(fp)) {
		if (fread(card, sizeof(Card), 1, fp) == 1) {
			n++;
		}
	}
	free(card);
	fclose(fp);
	return n;
}

//���¿���Ϣ
int updateCard(const Card* pCard, const char* pPath, int nIndex) {
	FILE* fp = NULL;
	char aBuffer[CARDCHARNUM];
	int nLine = 1;
	long lPosition = 0;
	if ((fp = fopen(pPath, "r+")) == NULL) {
		return 0;
	}
	//��������Ϣ���и���
	while (!feof(fp) && nLine < nIndex) {
		if (fgets(aBuffer, CARDCHARNUM, fp) != NULL) {
			lPosition = ftell(fp);//�õ�Ŀ���о����ļ���ͷ����
			nLine++;
		}
	}
	fseek(fp, lPosition, SEEK_SET);//�ƶ��ļ�ָ����Ŀ��λ��
	char* strTStart = timeToString(pCard->tStart);
	char* strTEnd = timeToString(pCard->tEnd);
	char* strTLast = timeToString(pCard->tLast);
	fprintf(fp, "%s##%s##%d##%s##%s##%f##%s##%d##%f##%d\n", pCard->aName, pCard->aPwd,
		pCard->nStatus, strTStart, strTEnd, pCard->fTotalUse, strTLast,
		pCard->nUseConst, pCard->fBalance, pCard->nDel);//����
	free(strTStart);
	free(strTEnd);
	free(strTLast);
	fclose(fp);//�ر� 
	return 1;
}

int updateCardByB(const Card* pCard, const char* pPath, int nIndex) {
	FILE* fp = NULL;
	Card* card = (Card*)malloc(sizeof(Card));
	int nLine = 1;
	long lPosition = 0;
	if ((fp = fopen(pPath, "rb+")) == NULL) {
		return 0;
	}
	//��������Ϣ
	while (!feof(fp) && nLine < nIndex) {
		if (fread(card, sizeof(Card), 1, fp) == 1) {
			lPosition = ftell(fp);//�õ�Ŀ���о����ļ���ͷ����
			nLine++;
		}
	}
	fseek(fp, lPosition, SEEK_SET);//�ƶ��ļ�ָ����Ŀ��λ��
	fwrite(pCard, sizeof(Card), 1, fp);//����
	fclose(fp);//�ر��ļ�
	return 1;
}
