#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>
#include"globle.h"
#include"model.h"
#include"service.h"
#include"card_file.h"

//将卡信息存入文件中
int saveCard(const Card* pCard, const char* pPath) {
	FILE* fp;
	//打开文件
	if ((fp = fopen(pPath, "a")) == NULL) {
		fopen(pPath, "w");
	}
	//将数据存入文件
	char* strTStart = timeToString(pCard->tStart);
	char* strTEnd = timeToString(pCard->tEnd);
	char* strTLast = timeToString(pCard->tLast);
	fprintf(fp, "%s##%s##%d##%s##%s##%f##%s##%d##%f##%d\n",pCard->aName, pCard->aPwd, 
		pCard->nStatus, strTStart, strTEnd, pCard->fTotalUse, strTLast, 
		pCard->nUseConst, pCard->fBalance, pCard->nDel);
	free(strTStart);
	free(strTEnd);
	free(strTLast);
	//关闭文件
	fclose(fp);
	return 1;
}

int saveCardByB(const Card* pCard, const char* pPath) {
	FILE* fp;
	//打开文件
	if ((fp = fopen(pPath, "ab")) == NULL) {
		fopen(pPath, "wb");
	}
	//将数据存入文件
	fwrite(pCard, sizeof(Card), 1, fp);
	fclose(fp);
	return 1;
}

//将卡信息读入数组中
int readCard(Card* pCard, const char* pPath) {
	int n = getCardCount(pPath);//获取卡信息中信息数量
	FILE* fp;
	if ((fp = fopen(pPath, "r")) == NULL) {
		return 0;
	}
	char ch[256];
	//逐行将读取的信息存入数组
	for (int i = 0; i < n; i++) {
		fgets(ch, 256, fp);
		//将字符串转化为数组
		pCard[i] = praseCard(ch);
	}
	fclose(fp);
	return 1;
}

int readCardByB(Card* pCard, const char* pPath) {
	int n = getCardCountByB(pPath);//获取卡信息中信息数量
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

//读取字符串中的一个信息
void prase(char** p, char* str) {
	memset(str, 0, sizeof(str));//初始化str
	int i = 0;
	while ((**p) != '#' && (**p) != NULL) {
		str[i] = **p;
		i++;
		(*p)++;
	}
	(*p)++;
	(*p)++;
}

//读取卡中的信息
Card praseCard(char* pBuf) {
	Card card;
	char* p = &pBuf[0];//遍历字符串
	char str[32];
	//读取卡号
	prase(&p, str);
	strcpy(card.aName, str);
	//读取密码
	prase(&p, str);
	strcpy(card.aPwd, str);
	//读取卡状态
	prase(&p, str);
	card.nStatus = atoi(str);
	//开卡时间
	prase(&p, str);
	card.tStart = stringToTime_t(str);
	//截止时间
	prase(&p, str);
	card.tEnd = stringToTime_t(str);
	//累计金额
	prase(&p, str);
	card.fTotalUse = (float)atof(str);
	//最后使用时间
	prase(&p, str);
	card.tLast = stringToTime_t(str);
	//使用次数
	prase(&p, str);
	card.nUseConst = atoi(str);
	//当前余额
	prase(&p, str);
	card.fBalance = (float)atof(str);
	//删除标识
	prase(&p, str);
	card.nDel = atoi(str);
	return card;
} 

//获取卡文件中信息数量
int getCardCount(const char* pPath) {
	FILE* fp;
	if ((fp = fopen(pPath, "r"))== NULL) {
		return 0;
	}
	int n = 0;
	char ch;
	//移动文件指针直到结束
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

//更新卡信息
int updateCard(const Card* pCard, const char* pPath, int nIndex) {
	FILE* fp = NULL;
	char aBuffer[CARDCHARNUM];
	int nLine = 1;
	long lPosition = 0;
	if ((fp = fopen(pPath, "r+")) == NULL) {
		return 0;
	}
	//遍历卡信息进行更新
	while (!feof(fp) && nLine < nIndex) {
		if (fgets(aBuffer, CARDCHARNUM, fp) != NULL) {
			lPosition = ftell(fp);//得到目标行距离文件开头距离
			nLine++;
		}
	}
	fseek(fp, lPosition, SEEK_SET);//移动文件指针至目标位置
	char* strTStart = timeToString(pCard->tStart);
	char* strTEnd = timeToString(pCard->tEnd);
	char* strTLast = timeToString(pCard->tLast);
	fprintf(fp, "%s##%s##%d##%s##%s##%f##%s##%d##%f##%d\n", pCard->aName, pCard->aPwd,
		pCard->nStatus, strTStart, strTEnd, pCard->fTotalUse, strTLast,
		pCard->nUseConst, pCard->fBalance, pCard->nDel);//更新
	free(strTStart);
	free(strTEnd);
	free(strTLast);
	fclose(fp);//关闭 
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
	//遍历卡信息
	while (!feof(fp) && nLine < nIndex) {
		if (fread(card, sizeof(Card), 1, fp) == 1) {
			lPosition = ftell(fp);//得到目标行距离文件开头距离
			nLine++;
		}
	}
	fseek(fp, lPosition, SEEK_SET);//移动文件指针至目标位置
	fwrite(pCard, sizeof(Card), 1, fp);//更新
	fclose(fp);//关闭文件
	return 1;
}
