#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<Windows.h>
#include"model.h"
#include"globle.h"
#include"menu.h"
#include"service.h"
#include"billing_service.h"
#include"billing_file.h"
#include"card_service.h"
#include"card_file.h"

extern CardNode* cardLinkList;
extern BillingNode* billingLinkList;

int main(void) {
	printf("��ӭ����Ʒѹ���ϵͳ\n");
	initCardNode(&cardLinkList);//��������Ϣ��ͷ
	initBillingNode(&billingLinkList);//����������Ϣ��ͷ
	int nSelection = -1;//�˵���ѡ��
	do {
		outputMenu();
		scanf("%d", &nSelection);
		switch (nSelection) {
		//��ӿ�
		case 1: {
			printf("--------��ӿ�-------\n");
			add();
			break;
		}
		//��ѯ��
		case 2: {
			printf("--------��ѯ��-------\n");
			query();
			break;
		}
		//�ϻ�
		case 3: {
			printf("--------�ϻ�--------\n");
			logon();
			break;
		}
		//�»�
		case 4: {
			printf("--------�»�--------\n");
			settle();
			break;
		}
		//��ֵ
		case 5: {
			printf("--------��ֵ--------\n");
			addMoney();
			break;
		}
		//�˷�
		case 6: {
			printf("--------�˷�--------\n");
			refundMoney();
			break;
		}
		//��ѯͳ��
		//ע����
		case 7: {
			printf("--------ע����-------\n");
			annul();
			break;
		}
		//�������
		case 8: {
			printf("--------����Աģʽ--------\n");
			admMode();
			break;
		}
		//�˳�
		case 0: {
			printf("--------�˳�--------\n");
			exitAMS();
			break;
		}
		default:{
			printf("����Ĳ˵���Ŵ���\n");
			break;
		}
		}
		printf("\n");
	} while (nSelection);
	system("pause");
	return 0;
}
