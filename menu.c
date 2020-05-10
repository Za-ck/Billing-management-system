#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include"globle.h"
#include"model.h"
#include"service.h"
#include"card_service.h"
#include"card_file.h"
#include"billing_service.h"
#include"billing_file.h"

extern CardNode* cardLinkList;
extern BillingNode* billingLinkList;

//菜单目录
void outputMenu() {
	printf("--------菜单--------\n");
	printf("1.添加卡\n");
	printf("2.查询卡\n");
	printf("3.上机\n");
	printf("4.下机\n");
	printf("5.充值\n");
	printf("6.退费\n");
	printf("7.注销卡\n");
	printf("8.管理员模式\n");
	printf("0.退出\n");
	printf("请选择菜单项编号（0~8）：");
}

//添加卡
void add() {
	Card card;
	char name[100];//卡号
	char pwd[100];//密码
	float money = 0;//预付金额
	time_t timeStart = 0;
	time(&timeStart);//开卡时间

	//记录卡号
	printf("请输入卡号<长度为1~18>：");
	scanf("%s", &name);
	//判断长度是否合法
	if (strlen(name) > 18) {
		printf("卡号长度不合法！\n");
		return;
	}
	//记录密码
	printf("请输入密码<长度为1~8>：");
	scanf("%s", &pwd);
	//判断密码长度是否合法
	if (strlen(pwd) > 8) {
		printf("密码长度不合法！\n");
		return;
	}
	//记录开卡金额
	printf("请输入开卡金额(RMB)：");
	scanf("%f", &money);
	//判断开卡金额是否合法
	if (money < 0) {
		printf("余额不合法！\n");
		return;
	}
	//验证码
	if (doCode() != 1) {
		printf("验证码错误！\n");
		return;
	}
	//卡号是否已存在
	getCardByB();
	Card* tempCard = checkCard(cardLinkList, name);
	if (tempCard != NULL) {
		printf("卡号已存在！\n");
		cardListClean(&cardLinkList);//清空链表避免重复
		return;
	}
	cardListClean(&cardLinkList);//清空链表避免重复
	strcpy(card.aName, name);//卡号
	strcpy(card.aPwd, pwd);//密码
	card.fTotalUse = 0;//累计金额
	card.fBalance = money;//余额

	//放入卡的其他信息
	card.nStatus = 0;//卡状态
	card.tStart = timeStart;//开卡时间
	card.tEnd = timeStart;//截止日期
	card.tLast = timeStart;//最后使用时间
	card.nUseConst = 0;//使用次数
	card.nDel = 0;//删除标记
	//插入文件
	if (addCard(card) != 1) {
		printf("添加失败！\n");
		return;
	}
	//打印
	printf("卡号      密码      状态      开卡金额\n");
	printf("%-10s%-10s%-10d%-10.1f\n", card.aName, card.aPwd, card.nStatus, card.fBalance);
}

//查询卡
void query() {
	char name[100];//接收查询卡号
	Card* cards = NULL;//储存符合条件的数据
	int index = 0;//接收符合数据数
	printf("请输入查询卡号<长度为1~18>：");
	scanf("%s", &name);
	//检查查询卡号长度是否合法
	if (strlen(name) > 18) {
		printf("查询卡号长度不合法！\n");
		return;
	}
	//验证码
	if (doCode() != 1) {
		printf("验证码错误！\n");
		return;
	}
	//将文件中的数据读入链表
	cards = queryCard(name, &index);
	if (index == 0) {
		printf("无匹配！\n");
		return;
	}
	//打印
	printf("卡号      状态      余额      累计使用      使用次数      上次使用时间\n");
	for (int i = 0; i < index; i++) {
		char* strTLast = timeToString(cards[i].tLast);
		printf("%-10s%-10d%-10.1f%-14.1f%-14d%-10s\n",
			cards[i].aName, cards[i].nStatus, cards[i].fBalance, cards[i].fTotalUse, cards[i].nUseConst, strTLast);
		free(strTLast);
		strTLast = NULL;
	}
	//释放cards
	free(cards);
	cards = NULL;
}

void logon() {
	char pName[32];//储存要查询的卡号
	char pPwd[32];//储存要查询的密码
	//获取上机信息
	printf("请输入上机卡号<长度为1~18>:");
	scanf("%s", &pName);
	if (strlen(pName) > 18 || strlen(pName) <= 0){
		printf("卡号长度不合法！\n");
		return;
	}
	printf("请输入上机密码<长度为1~8>:");
	scanf("%s", &pPwd);
	if (strlen(pPwd) > 8 || strlen(pPwd) <= 0) {
		printf("密码长度不合法！\n");
		return;
	}
	//验证码
	if (doCode() != 1) {
		printf("验证码错误！\n");
		return;
	}
	LogonInfo* logonInfo = (LogonInfo*)malloc(sizeof(LogonInfo));//记录上机记录
	int flag = doLogon(pName, pPwd, logonInfo);//开始上机
	switch (flag) {
	case FALSE:
		printf("上机失败！\n");
		return;
	case TRUE:
		printf("上机成功！\n");
		break;
	case UNUSE:
		printf("卡不能使用！\n");
		return;
	case ENOUGHMONEY:
		printf("余额不足！\n");
		return;
	default:
		printf("ERROR");
		return;
	}
	//打印
	char* strTLogon = timeToString(logonInfo->tLogen);
	printf("--------上机信息如下--------\n");
	printf("卡号      余额      上机时间\n");
	printf("%-10s%-10.1f%-10s\n", logonInfo->aCardName, logonInfo->fBalance, strTLogon);
	free(strTLogon);
	strTLogon = NULL;
	free(logonInfo);
	logonInfo = NULL;

}

//下机
void settle() {
	char pName[32];//储存要查询的卡号
	char pPwd[32];//储存要查询的密码
	//获取下机信息
	printf("请输入下机卡号<长度为1~18>:");
	scanf("%s", &pName);
	if (strlen(pName) > 18 || strlen(pName) <= 0) {
		printf("卡号长度不合法！\n");
		return;
	}
	printf("请输入下机密码<长度为1~8>:");
	scanf("%s", &pPwd);
	if (strlen(pPwd) > 8 || strlen(pPwd) <= 0) {
		printf("密码长度不合法！\n");
		return;
	}
	//验证码
	if (doCode() != 1) {
		printf("验证码错误！\n");
		return;
	}
	//获取下机信息
	SettleInfo* settleInfo = (SettleInfo*)malloc(sizeof(SettleInfo));
	int flag = doSettle(pName, pPwd, settleInfo);
	switch (flag) {
	case FALSE:
		printf("下机失败！\n");
		return;
	case TRUE:
		printf("下机成功！\n");
		break;
	case UNUSE:
		printf("卡不能使用！\n");
		return;
	case ENOUGHMONEY:
		printf("余额不足！\n");
		return;
	default:
		printf("ERROR");
		return;
	}

	char* tSettleStart = timeToString(settleInfo->tStart);
	char* tSettleEnd = timeToString(settleInfo->tEnd);
	printf("--------下机信息如下--------\n");
	printf("卡号      消费      余额      上机时间              下机时间\n");
	printf("%-10s%-10.1f%-10.1f%-22s%-22s\n", settleInfo->aCardName, settleInfo->fAmount, settleInfo->fBalance, tSettleStart, tSettleEnd);

	//释放
	free(tSettleStart);
	free(tSettleEnd);
	free(settleInfo);
}

//充值
void addMoney() {
	char pName[32];//卡号
	char pPwd[32];//密码
	MoneyInfo* moneyInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));
	//获取卡号密码充值金额信息
	printf("请输入卡号<长度为1~18>:");
	scanf("%s", &pName);
	if (strlen(pName) > 18 || strlen(pName) <= 0) {
		printf("卡号长度不合法！\n");
		return;
	}
	printf("请输入密码<长度为1~8>:");
	scanf("%s", &pPwd);
	if (strlen(pPwd) > 8 || strlen(pPwd) <= 0) {
		printf("密码长度不合法！\n");
		return;
	}
	printf("请输入充值金额（RMB）：");
	scanf("%f", &(moneyInfo->fMoney));
	//验证码
	if (doCode() != 1) {
		printf("验证码错误！\n");
		return;
	}
	//充值
	if (doAddMoney(pName, pPwd, moneyInfo) == FALSE) {
		printf("充值失败！\n");
		return;
	}
	printf("--------充值信息如下--------\n");
	printf("卡号      充值金额      余额\n");
	printf("%-10s%-14.1f%-10.1f\n", moneyInfo->aCardName, moneyInfo->fMoney, moneyInfo->fBalance);
	//释放
	free(moneyInfo);
}

//退费
void refundMoney() {
	char pName[32];//卡号
	char pPwd[32];//密码
	MoneyInfo* moneyInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));
	//获取卡号密码信息
	printf("请输入卡号<长度为1~18>:");
	scanf("%s", &pName);
	if (strlen(pName) > 18 || strlen(pName) <= 0) {
		printf("卡号长度不合法！\n");
		return;
	}
	printf("请输入密码<长度为1~8>:");
	scanf("%s", &pPwd);
	if (strlen(pPwd) > 8 || strlen(pPwd) <= 0) {
		printf("密码长度不合法！\n");
		return;
	}
	//验证码
	if (doCode() != 1) {
		printf("验证码错误！\n");
		return;
	}
	//退费
	if ((doRefundMoney(pName, pPwd, moneyInfo)) == FALSE) {
		printf("退费失败！\n");
		return;
	}
	printf("--------退费信息如下--------\n");
	printf("卡号      退费金额      余额\n");
	printf("%-10s%-14.1f%-10.1f\n", moneyInfo->aCardName, moneyInfo->fMoney, moneyInfo->fBalance);
	//释放
	free(moneyInfo);
}

//注销卡
void annul() {
	char pName[32];//卡号
	char pPwd[32];//密码
	float* fBal = (float*)malloc(sizeof(float));//余额
	MoneyInfo* moneyInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));
	//获取卡号密码信息
	printf("请输入卡号<长度为1~18>:");
	scanf("%s", &pName);
	if (strlen(pName) > 18 || strlen(pName) <= 0) {
		printf("卡号长度不合法！\n");
		return;
	}
	printf("请输入密码<长度为1~8>:");
	scanf("%s", &pPwd);
	if (strlen(pPwd) > 8 || strlen(pPwd) <= 0) {
		printf("密码长度不合法！\n");
		return;
	}
	//验证码
	if (doCode() != 1) {
		printf("验证码错误！\n");
		return;
	}
	//注销
	if (annulCard(pName, pPwd, fBal) == FALSE) {
		printf("注销卡失败！\n");
	}
	printf("--------注销信息如下--------\n");
	printf("卡号      退款金额\n");
	printf("%-10s%-14.1f\n", pName, (*fBal));
	//释放
	free(fBal);
}

void admMode() {
	char pwdAdm[18];//记录密码
	printf("请输入管理员密码：");
	scanf("%s", &pwdAdm);
	//验证码
	if (doCode() != 1) {
		printf("验证码错误！\n");
		return;
	}
	if (strcmp(pwdAdm, ADMINISTRATOR) != 0) {
		printf("密码错误！\n");
		return;
	}
	printf("欢迎使用管理员模式！\n");
	int nChoise = -1;
	do {
		printf("--------管理员模式--------\n");
		printf("1.删除所有信息\n");
		printf("2.查看所有卡信息\n");
		printf("3.查看所有消费记录\n");
		printf("0.退出\n");
		printf("请选择功能：");
		scanf("%d", &nChoise);
		switch (nChoise) {
		//删除
		case 1: {
			remove(CARDPATH);
			remove(BILLINGPATH);
			printf("删除成功！\n");
			break;
		}
		//查看卡信息
		case 2: {
			printf("所有卡信息：\n");
			getCardByB();
			CardNode* p = cardLinkList->next;
			if (p == NULL) {
				printf("无数据！\n");
				break;
			}
			printf("卡号      状态      余额      累计使用      使用次数      上次使用时间\n");
			//遍历链表输出
			while (p != NULL) {
				char* strTLast = timeToString(p->data.tLast);
				printf("%-10s%-10d%-10.1f%-14.1f%-14d%-10s\n",
					p->data.aName, p->data.nStatus, p->data.fBalance, p->data.fTotalUse, p->data.nUseConst, strTLast);
				free(strTLast);
				p = p->next;
			}
			cardListClean(&cardLinkList);
			break;
		}
		//查看消费信息
		case 3: {
			printf("所有消费信息：\n");
			getBilling();
			BillingNode* p = billingLinkList->next;
			if (p == NULL) {
				printf("无数据！\n");
				break;
			}
			printf("卡号      上机时间              下机时间              消费金额      消费状态\n");
			//遍历链表输出
			while (p != NULL) {
				char* tSettleStart = timeToString(p->data.tStart);
				char* tSettleEnd = timeToString(p->data.tEnd);
				printf("%-10s%-22s%-22s%-14f%-10d\n", p->data.aCardName, tSettleStart, tSettleEnd, p->data.fAmount, p->data.nStatus);
				free(tSettleStart);
				free(tSettleEnd);
				p = p->next;
			}
			billingListClean(&billingLinkList);
			break;
		}
		case 0: {
			printf("欢迎再次使用！\n");
			break;
		}
		default:
			printf("功能选择错误！\n");
			break;
		}
	} while (nChoise);

}

//删除文件
void exitAMS() {
	//释放cardLinkList
	cardListClean(&cardLinkList);
	if (cardLinkList != NULL) {
		free(cardLinkList);
		cardLinkList = NULL;
	}
	//释放billingLinkList
	billingListClean(&billingLinkList);
	if (billingLinkList != NULL) {
		free(billingLinkList);
		billingLinkList = NULL;
	}
}
