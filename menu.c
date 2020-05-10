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

//�˵�Ŀ¼
void outputMenu() {
	printf("--------�˵�--------\n");
	printf("1.��ӿ�\n");
	printf("2.��ѯ��\n");
	printf("3.�ϻ�\n");
	printf("4.�»�\n");
	printf("5.��ֵ\n");
	printf("6.�˷�\n");
	printf("7.ע����\n");
	printf("8.����Աģʽ\n");
	printf("0.�˳�\n");
	printf("��ѡ��˵����ţ�0~8����");
}

//��ӿ�
void add() {
	Card card;
	char name[100];//����
	char pwd[100];//����
	float money = 0;//Ԥ�����
	time_t timeStart = 0;
	time(&timeStart);//����ʱ��

	//��¼����
	printf("�����뿨��<����Ϊ1~18>��");
	scanf("%s", &name);
	//�жϳ����Ƿ�Ϸ�
	if (strlen(name) > 18) {
		printf("���ų��Ȳ��Ϸ���\n");
		return;
	}
	//��¼����
	printf("����������<����Ϊ1~8>��");
	scanf("%s", &pwd);
	//�ж����볤���Ƿ�Ϸ�
	if (strlen(pwd) > 8) {
		printf("���볤�Ȳ��Ϸ���\n");
		return;
	}
	//��¼�������
	printf("�����뿪�����(RMB)��");
	scanf("%f", &money);
	//�жϿ�������Ƿ�Ϸ�
	if (money < 0) {
		printf("���Ϸ���\n");
		return;
	}
	//��֤��
	if (doCode() != 1) {
		printf("��֤�����\n");
		return;
	}
	//�����Ƿ��Ѵ���
	getCardByB();
	Card* tempCard = checkCard(cardLinkList, name);
	if (tempCard != NULL) {
		printf("�����Ѵ��ڣ�\n");
		cardListClean(&cardLinkList);//�����������ظ�
		return;
	}
	cardListClean(&cardLinkList);//�����������ظ�
	strcpy(card.aName, name);//����
	strcpy(card.aPwd, pwd);//����
	card.fTotalUse = 0;//�ۼƽ��
	card.fBalance = money;//���

	//���뿨��������Ϣ
	card.nStatus = 0;//��״̬
	card.tStart = timeStart;//����ʱ��
	card.tEnd = timeStart;//��ֹ����
	card.tLast = timeStart;//���ʹ��ʱ��
	card.nUseConst = 0;//ʹ�ô���
	card.nDel = 0;//ɾ�����
	//�����ļ�
	if (addCard(card) != 1) {
		printf("���ʧ�ܣ�\n");
		return;
	}
	//��ӡ
	printf("����      ����      ״̬      �������\n");
	printf("%-10s%-10s%-10d%-10.1f\n", card.aName, card.aPwd, card.nStatus, card.fBalance);
}

//��ѯ��
void query() {
	char name[100];//���ղ�ѯ����
	Card* cards = NULL;//�����������������
	int index = 0;//���շ���������
	printf("�������ѯ����<����Ϊ1~18>��");
	scanf("%s", &name);
	//����ѯ���ų����Ƿ�Ϸ�
	if (strlen(name) > 18) {
		printf("��ѯ���ų��Ȳ��Ϸ���\n");
		return;
	}
	//��֤��
	if (doCode() != 1) {
		printf("��֤�����\n");
		return;
	}
	//���ļ��е����ݶ�������
	cards = queryCard(name, &index);
	if (index == 0) {
		printf("��ƥ�䣡\n");
		return;
	}
	//��ӡ
	printf("����      ״̬      ���      �ۼ�ʹ��      ʹ�ô���      �ϴ�ʹ��ʱ��\n");
	for (int i = 0; i < index; i++) {
		char* strTLast = timeToString(cards[i].tLast);
		printf("%-10s%-10d%-10.1f%-14.1f%-14d%-10s\n",
			cards[i].aName, cards[i].nStatus, cards[i].fBalance, cards[i].fTotalUse, cards[i].nUseConst, strTLast);
		free(strTLast);
		strTLast = NULL;
	}
	//�ͷ�cards
	free(cards);
	cards = NULL;
}

void logon() {
	char pName[32];//����Ҫ��ѯ�Ŀ���
	char pPwd[32];//����Ҫ��ѯ������
	//��ȡ�ϻ���Ϣ
	printf("�������ϻ�����<����Ϊ1~18>:");
	scanf("%s", &pName);
	if (strlen(pName) > 18 || strlen(pName) <= 0){
		printf("���ų��Ȳ��Ϸ���\n");
		return;
	}
	printf("�������ϻ�����<����Ϊ1~8>:");
	scanf("%s", &pPwd);
	if (strlen(pPwd) > 8 || strlen(pPwd) <= 0) {
		printf("���볤�Ȳ��Ϸ���\n");
		return;
	}
	//��֤��
	if (doCode() != 1) {
		printf("��֤�����\n");
		return;
	}
	LogonInfo* logonInfo = (LogonInfo*)malloc(sizeof(LogonInfo));//��¼�ϻ���¼
	int flag = doLogon(pName, pPwd, logonInfo);//��ʼ�ϻ�
	switch (flag) {
	case FALSE:
		printf("�ϻ�ʧ�ܣ�\n");
		return;
	case TRUE:
		printf("�ϻ��ɹ���\n");
		break;
	case UNUSE:
		printf("������ʹ�ã�\n");
		return;
	case ENOUGHMONEY:
		printf("���㣡\n");
		return;
	default:
		printf("ERROR");
		return;
	}
	//��ӡ
	char* strTLogon = timeToString(logonInfo->tLogen);
	printf("--------�ϻ���Ϣ����--------\n");
	printf("����      ���      �ϻ�ʱ��\n");
	printf("%-10s%-10.1f%-10s\n", logonInfo->aCardName, logonInfo->fBalance, strTLogon);
	free(strTLogon);
	strTLogon = NULL;
	free(logonInfo);
	logonInfo = NULL;

}

//�»�
void settle() {
	char pName[32];//����Ҫ��ѯ�Ŀ���
	char pPwd[32];//����Ҫ��ѯ������
	//��ȡ�»���Ϣ
	printf("�������»�����<����Ϊ1~18>:");
	scanf("%s", &pName);
	if (strlen(pName) > 18 || strlen(pName) <= 0) {
		printf("���ų��Ȳ��Ϸ���\n");
		return;
	}
	printf("�������»�����<����Ϊ1~8>:");
	scanf("%s", &pPwd);
	if (strlen(pPwd) > 8 || strlen(pPwd) <= 0) {
		printf("���볤�Ȳ��Ϸ���\n");
		return;
	}
	//��֤��
	if (doCode() != 1) {
		printf("��֤�����\n");
		return;
	}
	//��ȡ�»���Ϣ
	SettleInfo* settleInfo = (SettleInfo*)malloc(sizeof(SettleInfo));
	int flag = doSettle(pName, pPwd, settleInfo);
	switch (flag) {
	case FALSE:
		printf("�»�ʧ�ܣ�\n");
		return;
	case TRUE:
		printf("�»��ɹ���\n");
		break;
	case UNUSE:
		printf("������ʹ�ã�\n");
		return;
	case ENOUGHMONEY:
		printf("���㣡\n");
		return;
	default:
		printf("ERROR");
		return;
	}

	char* tSettleStart = timeToString(settleInfo->tStart);
	char* tSettleEnd = timeToString(settleInfo->tEnd);
	printf("--------�»���Ϣ����--------\n");
	printf("����      ����      ���      �ϻ�ʱ��              �»�ʱ��\n");
	printf("%-10s%-10.1f%-10.1f%-22s%-22s\n", settleInfo->aCardName, settleInfo->fAmount, settleInfo->fBalance, tSettleStart, tSettleEnd);

	//�ͷ�
	free(tSettleStart);
	free(tSettleEnd);
	free(settleInfo);
}

//��ֵ
void addMoney() {
	char pName[32];//����
	char pPwd[32];//����
	MoneyInfo* moneyInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));
	//��ȡ���������ֵ�����Ϣ
	printf("�����뿨��<����Ϊ1~18>:");
	scanf("%s", &pName);
	if (strlen(pName) > 18 || strlen(pName) <= 0) {
		printf("���ų��Ȳ��Ϸ���\n");
		return;
	}
	printf("����������<����Ϊ1~8>:");
	scanf("%s", &pPwd);
	if (strlen(pPwd) > 8 || strlen(pPwd) <= 0) {
		printf("���볤�Ȳ��Ϸ���\n");
		return;
	}
	printf("�������ֵ��RMB����");
	scanf("%f", &(moneyInfo->fMoney));
	//��֤��
	if (doCode() != 1) {
		printf("��֤�����\n");
		return;
	}
	//��ֵ
	if (doAddMoney(pName, pPwd, moneyInfo) == FALSE) {
		printf("��ֵʧ�ܣ�\n");
		return;
	}
	printf("--------��ֵ��Ϣ����--------\n");
	printf("����      ��ֵ���      ���\n");
	printf("%-10s%-14.1f%-10.1f\n", moneyInfo->aCardName, moneyInfo->fMoney, moneyInfo->fBalance);
	//�ͷ�
	free(moneyInfo);
}

//�˷�
void refundMoney() {
	char pName[32];//����
	char pPwd[32];//����
	MoneyInfo* moneyInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));
	//��ȡ����������Ϣ
	printf("�����뿨��<����Ϊ1~18>:");
	scanf("%s", &pName);
	if (strlen(pName) > 18 || strlen(pName) <= 0) {
		printf("���ų��Ȳ��Ϸ���\n");
		return;
	}
	printf("����������<����Ϊ1~8>:");
	scanf("%s", &pPwd);
	if (strlen(pPwd) > 8 || strlen(pPwd) <= 0) {
		printf("���볤�Ȳ��Ϸ���\n");
		return;
	}
	//��֤��
	if (doCode() != 1) {
		printf("��֤�����\n");
		return;
	}
	//�˷�
	if ((doRefundMoney(pName, pPwd, moneyInfo)) == FALSE) {
		printf("�˷�ʧ�ܣ�\n");
		return;
	}
	printf("--------�˷���Ϣ����--------\n");
	printf("����      �˷ѽ��      ���\n");
	printf("%-10s%-14.1f%-10.1f\n", moneyInfo->aCardName, moneyInfo->fMoney, moneyInfo->fBalance);
	//�ͷ�
	free(moneyInfo);
}

//ע����
void annul() {
	char pName[32];//����
	char pPwd[32];//����
	float* fBal = (float*)malloc(sizeof(float));//���
	MoneyInfo* moneyInfo = (MoneyInfo*)malloc(sizeof(MoneyInfo));
	//��ȡ����������Ϣ
	printf("�����뿨��<����Ϊ1~18>:");
	scanf("%s", &pName);
	if (strlen(pName) > 18 || strlen(pName) <= 0) {
		printf("���ų��Ȳ��Ϸ���\n");
		return;
	}
	printf("����������<����Ϊ1~8>:");
	scanf("%s", &pPwd);
	if (strlen(pPwd) > 8 || strlen(pPwd) <= 0) {
		printf("���볤�Ȳ��Ϸ���\n");
		return;
	}
	//��֤��
	if (doCode() != 1) {
		printf("��֤�����\n");
		return;
	}
	//ע��
	if (annulCard(pName, pPwd, fBal) == FALSE) {
		printf("ע����ʧ�ܣ�\n");
	}
	printf("--------ע����Ϣ����--------\n");
	printf("����      �˿���\n");
	printf("%-10s%-14.1f\n", pName, (*fBal));
	//�ͷ�
	free(fBal);
}

void admMode() {
	char pwdAdm[18];//��¼����
	printf("���������Ա���룺");
	scanf("%s", &pwdAdm);
	//��֤��
	if (doCode() != 1) {
		printf("��֤�����\n");
		return;
	}
	if (strcmp(pwdAdm, ADMINISTRATOR) != 0) {
		printf("�������\n");
		return;
	}
	printf("��ӭʹ�ù���Աģʽ��\n");
	int nChoise = -1;
	do {
		printf("--------����Աģʽ--------\n");
		printf("1.ɾ��������Ϣ\n");
		printf("2.�鿴���п���Ϣ\n");
		printf("3.�鿴�������Ѽ�¼\n");
		printf("0.�˳�\n");
		printf("��ѡ���ܣ�");
		scanf("%d", &nChoise);
		switch (nChoise) {
		//ɾ��
		case 1: {
			remove(CARDPATH);
			remove(BILLINGPATH);
			printf("ɾ���ɹ���\n");
			break;
		}
		//�鿴����Ϣ
		case 2: {
			printf("���п���Ϣ��\n");
			getCardByB();
			CardNode* p = cardLinkList->next;
			if (p == NULL) {
				printf("�����ݣ�\n");
				break;
			}
			printf("����      ״̬      ���      �ۼ�ʹ��      ʹ�ô���      �ϴ�ʹ��ʱ��\n");
			//�����������
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
		//�鿴������Ϣ
		case 3: {
			printf("����������Ϣ��\n");
			getBilling();
			BillingNode* p = billingLinkList->next;
			if (p == NULL) {
				printf("�����ݣ�\n");
				break;
			}
			printf("����      �ϻ�ʱ��              �»�ʱ��              ���ѽ��      ����״̬\n");
			//�����������
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
			printf("��ӭ�ٴ�ʹ�ã�\n");
			break;
		}
		default:
			printf("����ѡ�����\n");
			break;
		}
	} while (nChoise);

}

//ɾ���ļ�
void exitAMS() {
	//�ͷ�cardLinkList
	cardListClean(&cardLinkList);
	if (cardLinkList != NULL) {
		free(cardLinkList);
		cardLinkList = NULL;
	}
	//�ͷ�billingLinkList
	billingListClean(&billingLinkList);
	if (billingLinkList != NULL) {
		free(billingLinkList);
		billingLinkList = NULL;
	}
}
