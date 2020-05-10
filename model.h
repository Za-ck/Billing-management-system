#pragma once
#include<time.h>

//����Ϣ
typedef struct Card {
	char aName[18];//����
	char aPwd[8];//����
	int nStatus;//��״̬ 0-δ�ϻ���1-�����ϻ���2-��ע����3-ʧЧ
	time_t tStart;//����ʱ��
	time_t tEnd;//��ֹʱ��
	float fTotalUse;//�ۼƽ��
	time_t tLast;//���ʹ��ʱ��
	int nUseConst;//ʹ�ô���
	float fBalance;//���
	int nDel;//ɾ����ǣ�0-δɾ����1-��ɾ��
} Card;

//����Ϣ���
typedef struct IpCardNode {
	Card data;
	struct IpCardNode* next;
}CardNode;

//������Ϣ
typedef struct Billing {
	char aCardName[18];//����
	time_t tStart;//�ϻ�ʱ��
	time_t tEnd;//�»�ʱ��
	float fAmount;//���ѽ��
	int nStatus;//����״̬��0-δ���㣬1-�Ѿ�����
	int nDel;//0-δɾ����1-��ɾ��
}Billing;

//������Ϣ�ڵ�
typedef struct BillingNode {
	Billing data;
	struct BillingNode* next;
}BillingNode;

//�ϻ���Ϣ
typedef struct LogonInfo {
	char aCardName[18];//�ϻ�����
	time_t tLogen;//�ϻ�ʱ��
	float fBalance;//�ϻ�ʱ��ʱ��
}LogonInfo;

//�»���Ϣ
typedef struct SettleInfo {
	char aCardName[18];//����
	time_t tStart;//�ϻ�ʱ��
	time_t tEnd;//�»�ʱ��
	float fAmount;//���ѽ��
	float fBalance;//���
}SettleInfo;

//��ֵ�˷ѽṹ��
typedef struct Money {
	char aCardName[18];//����
	time_t tTime;//��ֵ�˷�ʱ��
	int nStatus;//״̬��0-��ֵ��1-�˷�
	float fMoney;//��ֵ�˷����
	int nDel;//ɾ����ʶ��0-δɾ����1-��ɾ��
} Money;

//��ֵ�˷���Ϣ
typedef struct MoneyInfo {
	char aCardName[18];//����
	float fMoney;//��ֵ�˷����
	float fBalance;//���
} MoneyInfo;

