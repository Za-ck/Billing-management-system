#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include"globle.h"
#include"model.h"
#include"service.h"
#include"card_service.h"
#include"card_file.h"
#include"billing_service.h"
#include"billing_file.h"
#include"money_file.h"

//卡信息表头
CardNode* cardLinkList;
//计费信息表头
BillingNode* billingLinkList;

//添加卡
int addCardInfo(Card card) {
	if (addCard == 0) {
		return 0;
	}
	return 1;
}
//查找卡
Card* queryCardInfo(const char* pName, int* pIndex) {
	return queryCard(pName, pIndex);
}

//上机
int doLogon(const char* pName, const char* pPwd, LogonInfo* info) {
	int nIndex = 0;
	//获得卡信息
	if (getCardByB() == 0) {
		return;
	}
	//在链表中查找
	Card* card = checkCard_(cardLinkList, pName, &nIndex);
	//判断用户名，密码是否正确
	if (card == NULL || strcmp(card->aPwd, pPwd) != 0) {
		return FALSE;
	}
	/*if (t >= card->tEnd) {
		card->nDel = 1;
	}*/
	//判断卡状态
	if (card->nDel != 0) {
		return UNUSE;
	}
	//判断是否已经上机
	if (card->nStatus != 0) {
		return FALSE;
	}
	//判断余额是否不足
	if (card->fBalance <= 0) {
		return ENOUGHMONEY;
	}

	//记录消费信息
	Billing* billing = (Billing*)malloc(sizeof(Billing));
	strcpy(billing->aCardName, pName);
	billing->fAmount = 0;
	billing->tStart = time(NULL);
	billing->tEnd = 0;
	billing->nStatus = 0;
	billing->nDel = 0;
	addBilling(billing);//存入文件
	//更新链表中的卡信息
	card->nStatus = 1;//状态
	card->tLast = billing->tStart;//最后使用时间
	//更新文件中的卡信息
	if (0 == updateCardByB(card, CARDPATH, nIndex)) {
		return FALSE;
	}
	//将数据存入info
	strcpy(info->aCardName, card->aName);
	info->tLogen = billing->tStart;
	info->fBalance = card->fBalance;
	//释放billing
	free(billing);
	//清空卡链表
	cardListClean(&cardLinkList);
	return TRUE;//上机成功
}

//下机
int doSettle(const char* pName, const char* pPwd, SettleInfo* info) {
	int nIndex = 0;
	//获取卡信息
	if (getCardByB() == 0) {
		return;
	}
	//获取消费信息
	if (getBilling() == 0) {
		return;
	}
	Card* card = checkCard_(cardLinkList, pName, &nIndex);//寻找
	//判断用户名，密码是否正确
	if (card == NULL || strcmp(card->aPwd, pPwd) != 0) {
		return FALSE;
	}
	/*if (t >= card->tEnd) {
		card->nDel = 1;
	}*/
	//判断卡状态
	if (card->nDel != 0) {
		return UNUSE;
	}
	//判断是否已经上机
	if (card->nStatus != 1) {
		return FALSE;
	}
	//判断余额是否不足
	if (card->fBalance <= 0) {
		return ENOUGHMONEY;
	}

	int nIndexB = 0;
	Billing* billing = queryBilling_(billingLinkList, pName, &nIndexB);
	//判断卡号是否存在消费信息
	if (billing == NULL) {
		return FALSE;
	}
	//判断消费信息状态
	if (billing->nStatus != 0) {
		return FALSE;
	}
	//判断消费信息是否被删除
	if (billing->nDel != 0) {
		return UNUSE;
	}

	//计算消费金额
	double amount = getAmount(billing->tStart);
	if (amount < 0) {
		return FALSE;
	}
	//判断余额是否充足
	if (card->fBalance < amount) {
		return ENOUGHMONEY;
	}

	//获取当前时间
	time_t t = time(NULL);

	//更新卡信息
	card->nStatus = 0;//更新卡状态
	card->fBalance -= amount;//更新卡余额
	card->fTotalUse += amount;//更新卡累计使用
	card->nUseConst++;
	card->tLast = t;
	if (0 == updateCardByB(card, CARDPATH, nIndex)) {
		return FALSE;
	}

	//更新消费信息
	billing->tEnd = t;
	billing->fAmount = amount;
	billing->nStatus = 1;
	if (0 == updateBilling(billing, BILLINGPATH, nIndexB)) {
		return FALSE;
	}

	//组装下机信息
	strcpy(info->aCardName, billing->aCardName);
	info->tStart = billing->tStart;
	info->tEnd = t;
	info->fAmount = amount;
	info->fBalance = card->fBalance;
	//清空卡链表
	cardListClean(&cardLinkList);
	//清空消费信息链表
	billingListClean(&billingLinkList);
	return TRUE;
}

//充值
int doAddMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo) {
	Money* money = (Money*)malloc(sizeof(Money));//储存信息
	int nIndex = 0;//记录更新卡位置
	//获得卡信息
	if (getCardByB() == 0) {
		return;
	}
	Card* card = checkCard_(cardLinkList, pName, &nIndex);//寻找
	//判断用户名，密码是否正确
	if (card == NULL || strcmp(card->aPwd, pPwd) != 0) {
		return FALSE;
	}
	//判断卡状态
	if (card->nStatus != 0 && card->nStatus != 1) {
		return FALSE;
	}
	//判断删除标记
	if (card->nDel == 1) {
		return FALSE;
	}
	//更新链表中的卡信息
	card->fBalance += pMoneyInfo->fMoney;
	//更新文件中的卡信息
	if (0 == updateCardByB(card, CARDPATH, nIndex)) {
		return FALSE;
	}
	//组装退费结构体，将退费信息存入文件
	strcpy(money->aCardName, pName);
	money->tTime = time(NULL);
	money->nStatus = 0;
	money->fMoney = pMoneyInfo->fMoney;
	money->nDel = 0;
	if (saveMoney(money, MONEYPATH) == 0) {
		return 0;
	}
	free(money);
	//组装充值信息
	strcpy(pMoneyInfo->aCardName, pName);
	pMoneyInfo->fBalance = card->fBalance;
	//清空卡链表
	cardListClean(&cardLinkList);
	return TRUE;
}

//退费
int doRefundMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo) {
	Money* money = (Money*)malloc(sizeof(Money));//储存信息
	int nIndex = 0;//记录卡在文件中的位置
	//获得卡信息
	if (getCardByB() == 0) {
		return;
	}
	Card* card = checkCard_(cardLinkList, pName, &nIndex);//寻找
	//判断用户名，密码是否正确
	if (card == NULL || strcmp(card->aPwd, pPwd) != 0) {
		return FALSE;
	}
	//判断卡余额
	if (card->fBalance <= 0) {
		return FALSE;
	}
	//判断卡状态
	if (card->nStatus != 0) {
		return FALSE;
	}
	//判断删除标记
	if (card->nDel == 1) {
		return FALSE;
	}
	//记录退费金额
	int nBalance = card->fBalance;
	//更新链表中的卡信息
	card->fBalance = 0;
	//更新文件中的卡信息
	if (0 == updateCardByB(card, CARDPATH, nIndex)) {
		return FALSE;
	}
	//组装退费结构体，将退费信息存入文件
	strcpy(money->aCardName, pName);
	money->tTime = time(NULL);
	money->nStatus = 1;
	money->fMoney = nBalance;
	money->nDel = 0;
	if (saveMoney(money, MONEYPATH) == 0) {
		return 0;
	}
	free(money);
	//组装退费信息
	strcpy(pMoneyInfo->aCardName, pName);
	pMoneyInfo->fBalance = card->fBalance;
	pMoneyInfo->fMoney = nBalance;
	//清空卡链表
	cardListClean(&cardLinkList);
	return TRUE;
}

//注销
int annulCard(char* pName, char* pPwd, float* fBal) {
	int nIndex = 0;//记录卡在文件中的位置
	//获得卡信息
	if (getCardByB() == 0) {
		return;
	}
	Card* card = checkCard_(cardLinkList, pName, &nIndex);//寻找
	//判断用户名，密码是否正确
	if (card == NULL || strcmp(card->aPwd, pPwd) != 0) {
		return FALSE;
	}
	//判断卡状态
	if (card->nStatus != 0) {
		return FALSE;
	}
	//记录余额
	(*fBal) = card->fBalance;
	//更新链表中的卡信息
	card->nStatus = 2;
	card->fBalance = 0;
	//更新文件中的卡信息
	if (0 == updateCardByB(card, CARDPATH, nIndex)) {
		return FALSE;
	}
	//
	//清空卡链表
	cardListClean(&cardLinkList);
	return TRUE;
}

//获取消费金额
double getAmount(time_t tStart) {
	//获取当前时间
	time_t t;
	time(&t);
	//计算时间间隔
	int prd = t - tStart;
	int prd_ = prd / 60;
	if (prd % 60 != 0) {
		prd_++;
	}
	double amount = prd_ * UNIT;//消费金额
	return amount;
}

//验证码
int doCode() {
	//生成验证码
	char letters[63] = "abcdefjhijklmnopqrstuvwxyzABCDEFJHIJKLMNOPQRSTUVWXYZ1234567890";
	char upLetters[27] = "ABCDEFJHIJKLMNOPQRSTUVWXYZ";
	char lowLetters[27] = "abcdefjhijklmnopqrstuvwxyz";
	char code[CODENUM + 1];
	srand((unsigned)time(NULL));
	for (int i = 0; i < CODENUM; i++) {
		int j = rand() % 62;
		code[i] = letters[j];
	}
	code[CODENUM] = '\0';
	printf("请输入验证码（不区分大小写）：%s\n", code);
	char getCode[CODENUM + 1];//接收的验证码
	scanf("%s", &getCode);
	code[CODENUM] = '\0';
	//将生成的验证码和输入的验证码转换为小写
	for (int i = 0; i < CODENUM; i++) {
		for (int j = 0; j < 26; j++) {
			if (code[i] == upLetters[j]) {
				code[i] = lowLetters[j];
			}
			if (getCode[i] == upLetters[j]){
				getCode[i] = lowLetters[j];
			}
		}
	}
	if (strcmp(code, getCode) != 0) {
		return 0;
	}
	return 1;
}

//将时间戳转化为字符串
char* timeToString(const time_t t) {
	char* str = (char*)malloc(sizeof(char) * 30);
	struct tm* tmT = localtime(&t);
	strftime(str, 128, "%Y-%m-%d %H:%M:%S", tmT);
	return str;
}

//将字符串转化为时间戳
time_t stringToTime_t(const char* strT) {
	struct tm tmT;
	tmT.tm_year = atoi(strT) - 1900;
	tmT.tm_mon = atoi(strT + 5) - 1;
	tmT.tm_mday = atoi(strT + 8);
	tmT.tm_hour = atoi(strT + 11);
	tmT.tm_min = atoi(strT + 14);
	tmT.tm_sec = atoi(strT + 17);
	return mktime(&tmT);
}
