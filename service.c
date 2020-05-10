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

//����Ϣ��ͷ
CardNode* cardLinkList;
//�Ʒ���Ϣ��ͷ
BillingNode* billingLinkList;

//��ӿ�
int addCardInfo(Card card) {
	if (addCard == 0) {
		return 0;
	}
	return 1;
}
//���ҿ�
Card* queryCardInfo(const char* pName, int* pIndex) {
	return queryCard(pName, pIndex);
}

//�ϻ�
int doLogon(const char* pName, const char* pPwd, LogonInfo* info) {
	int nIndex = 0;
	//��ÿ���Ϣ
	if (getCardByB() == 0) {
		return;
	}
	//�������в���
	Card* card = checkCard_(cardLinkList, pName, &nIndex);
	//�ж��û����������Ƿ���ȷ
	if (card == NULL || strcmp(card->aPwd, pPwd) != 0) {
		return FALSE;
	}
	/*if (t >= card->tEnd) {
		card->nDel = 1;
	}*/
	//�жϿ�״̬
	if (card->nDel != 0) {
		return UNUSE;
	}
	//�ж��Ƿ��Ѿ��ϻ�
	if (card->nStatus != 0) {
		return FALSE;
	}
	//�ж�����Ƿ���
	if (card->fBalance <= 0) {
		return ENOUGHMONEY;
	}

	//��¼������Ϣ
	Billing* billing = (Billing*)malloc(sizeof(Billing));
	strcpy(billing->aCardName, pName);
	billing->fAmount = 0;
	billing->tStart = time(NULL);
	billing->tEnd = 0;
	billing->nStatus = 0;
	billing->nDel = 0;
	addBilling(billing);//�����ļ�
	//���������еĿ���Ϣ
	card->nStatus = 1;//״̬
	card->tLast = billing->tStart;//���ʹ��ʱ��
	//�����ļ��еĿ���Ϣ
	if (0 == updateCardByB(card, CARDPATH, nIndex)) {
		return FALSE;
	}
	//�����ݴ���info
	strcpy(info->aCardName, card->aName);
	info->tLogen = billing->tStart;
	info->fBalance = card->fBalance;
	//�ͷ�billing
	free(billing);
	//��տ�����
	cardListClean(&cardLinkList);
	return TRUE;//�ϻ��ɹ�
}

//�»�
int doSettle(const char* pName, const char* pPwd, SettleInfo* info) {
	int nIndex = 0;
	//��ȡ����Ϣ
	if (getCardByB() == 0) {
		return;
	}
	//��ȡ������Ϣ
	if (getBilling() == 0) {
		return;
	}
	Card* card = checkCard_(cardLinkList, pName, &nIndex);//Ѱ��
	//�ж��û����������Ƿ���ȷ
	if (card == NULL || strcmp(card->aPwd, pPwd) != 0) {
		return FALSE;
	}
	/*if (t >= card->tEnd) {
		card->nDel = 1;
	}*/
	//�жϿ�״̬
	if (card->nDel != 0) {
		return UNUSE;
	}
	//�ж��Ƿ��Ѿ��ϻ�
	if (card->nStatus != 1) {
		return FALSE;
	}
	//�ж�����Ƿ���
	if (card->fBalance <= 0) {
		return ENOUGHMONEY;
	}

	int nIndexB = 0;
	Billing* billing = queryBilling_(billingLinkList, pName, &nIndexB);
	//�жϿ����Ƿ����������Ϣ
	if (billing == NULL) {
		return FALSE;
	}
	//�ж�������Ϣ״̬
	if (billing->nStatus != 0) {
		return FALSE;
	}
	//�ж�������Ϣ�Ƿ�ɾ��
	if (billing->nDel != 0) {
		return UNUSE;
	}

	//�������ѽ��
	double amount = getAmount(billing->tStart);
	if (amount < 0) {
		return FALSE;
	}
	//�ж�����Ƿ����
	if (card->fBalance < amount) {
		return ENOUGHMONEY;
	}

	//��ȡ��ǰʱ��
	time_t t = time(NULL);

	//���¿���Ϣ
	card->nStatus = 0;//���¿�״̬
	card->fBalance -= amount;//���¿����
	card->fTotalUse += amount;//���¿��ۼ�ʹ��
	card->nUseConst++;
	card->tLast = t;
	if (0 == updateCardByB(card, CARDPATH, nIndex)) {
		return FALSE;
	}

	//����������Ϣ
	billing->tEnd = t;
	billing->fAmount = amount;
	billing->nStatus = 1;
	if (0 == updateBilling(billing, BILLINGPATH, nIndexB)) {
		return FALSE;
	}

	//��װ�»���Ϣ
	strcpy(info->aCardName, billing->aCardName);
	info->tStart = billing->tStart;
	info->tEnd = t;
	info->fAmount = amount;
	info->fBalance = card->fBalance;
	//��տ�����
	cardListClean(&cardLinkList);
	//���������Ϣ����
	billingListClean(&billingLinkList);
	return TRUE;
}

//��ֵ
int doAddMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo) {
	Money* money = (Money*)malloc(sizeof(Money));//������Ϣ
	int nIndex = 0;//��¼���¿�λ��
	//��ÿ���Ϣ
	if (getCardByB() == 0) {
		return;
	}
	Card* card = checkCard_(cardLinkList, pName, &nIndex);//Ѱ��
	//�ж��û����������Ƿ���ȷ
	if (card == NULL || strcmp(card->aPwd, pPwd) != 0) {
		return FALSE;
	}
	//�жϿ�״̬
	if (card->nStatus != 0 && card->nStatus != 1) {
		return FALSE;
	}
	//�ж�ɾ�����
	if (card->nDel == 1) {
		return FALSE;
	}
	//���������еĿ���Ϣ
	card->fBalance += pMoneyInfo->fMoney;
	//�����ļ��еĿ���Ϣ
	if (0 == updateCardByB(card, CARDPATH, nIndex)) {
		return FALSE;
	}
	//��װ�˷ѽṹ�壬���˷���Ϣ�����ļ�
	strcpy(money->aCardName, pName);
	money->tTime = time(NULL);
	money->nStatus = 0;
	money->fMoney = pMoneyInfo->fMoney;
	money->nDel = 0;
	if (saveMoney(money, MONEYPATH) == 0) {
		return 0;
	}
	free(money);
	//��װ��ֵ��Ϣ
	strcpy(pMoneyInfo->aCardName, pName);
	pMoneyInfo->fBalance = card->fBalance;
	//��տ�����
	cardListClean(&cardLinkList);
	return TRUE;
}

//�˷�
int doRefundMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo) {
	Money* money = (Money*)malloc(sizeof(Money));//������Ϣ
	int nIndex = 0;//��¼�����ļ��е�λ��
	//��ÿ���Ϣ
	if (getCardByB() == 0) {
		return;
	}
	Card* card = checkCard_(cardLinkList, pName, &nIndex);//Ѱ��
	//�ж��û����������Ƿ���ȷ
	if (card == NULL || strcmp(card->aPwd, pPwd) != 0) {
		return FALSE;
	}
	//�жϿ����
	if (card->fBalance <= 0) {
		return FALSE;
	}
	//�жϿ�״̬
	if (card->nStatus != 0) {
		return FALSE;
	}
	//�ж�ɾ�����
	if (card->nDel == 1) {
		return FALSE;
	}
	//��¼�˷ѽ��
	int nBalance = card->fBalance;
	//���������еĿ���Ϣ
	card->fBalance = 0;
	//�����ļ��еĿ���Ϣ
	if (0 == updateCardByB(card, CARDPATH, nIndex)) {
		return FALSE;
	}
	//��װ�˷ѽṹ�壬���˷���Ϣ�����ļ�
	strcpy(money->aCardName, pName);
	money->tTime = time(NULL);
	money->nStatus = 1;
	money->fMoney = nBalance;
	money->nDel = 0;
	if (saveMoney(money, MONEYPATH) == 0) {
		return 0;
	}
	free(money);
	//��װ�˷���Ϣ
	strcpy(pMoneyInfo->aCardName, pName);
	pMoneyInfo->fBalance = card->fBalance;
	pMoneyInfo->fMoney = nBalance;
	//��տ�����
	cardListClean(&cardLinkList);
	return TRUE;
}

//ע��
int annulCard(char* pName, char* pPwd, float* fBal) {
	int nIndex = 0;//��¼�����ļ��е�λ��
	//��ÿ���Ϣ
	if (getCardByB() == 0) {
		return;
	}
	Card* card = checkCard_(cardLinkList, pName, &nIndex);//Ѱ��
	//�ж��û����������Ƿ���ȷ
	if (card == NULL || strcmp(card->aPwd, pPwd) != 0) {
		return FALSE;
	}
	//�жϿ�״̬
	if (card->nStatus != 0) {
		return FALSE;
	}
	//��¼���
	(*fBal) = card->fBalance;
	//���������еĿ���Ϣ
	card->nStatus = 2;
	card->fBalance = 0;
	//�����ļ��еĿ���Ϣ
	if (0 == updateCardByB(card, CARDPATH, nIndex)) {
		return FALSE;
	}
	//
	//��տ�����
	cardListClean(&cardLinkList);
	return TRUE;
}

//��ȡ���ѽ��
double getAmount(time_t tStart) {
	//��ȡ��ǰʱ��
	time_t t;
	time(&t);
	//����ʱ����
	int prd = t - tStart;
	int prd_ = prd / 60;
	if (prd % 60 != 0) {
		prd_++;
	}
	double amount = prd_ * UNIT;//���ѽ��
	return amount;
}

//��֤��
int doCode() {
	//������֤��
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
	printf("��������֤�루�����ִ�Сд����%s\n", code);
	char getCode[CODENUM + 1];//���յ���֤��
	scanf("%s", &getCode);
	code[CODENUM] = '\0';
	//�����ɵ���֤����������֤��ת��ΪСд
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

//��ʱ���ת��Ϊ�ַ���
char* timeToString(const time_t t) {
	char* str = (char*)malloc(sizeof(char) * 30);
	struct tm* tmT = localtime(&t);
	strftime(str, 128, "%Y-%m-%d %H:%M:%S", tmT);
	return str;
}

//���ַ���ת��Ϊʱ���
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
