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
	printf("欢迎进入计费管理系统\n");
	initCardNode(&cardLinkList);//创建卡信息表头
	initBillingNode(&billingLinkList);//创建消费信息表头
	int nSelection = -1;//菜单项选项
	do {
		outputMenu();
		scanf("%d", &nSelection);
		switch (nSelection) {
		//添加卡
		case 1: {
			printf("--------添加卡-------\n");
			add();
			break;
		}
		//查询卡
		case 2: {
			printf("--------查询卡-------\n");
			query();
			break;
		}
		//上机
		case 3: {
			printf("--------上机--------\n");
			logon();
			break;
		}
		//下机
		case 4: {
			printf("--------下机--------\n");
			settle();
			break;
		}
		//充值
		case 5: {
			printf("--------充值--------\n");
			addMoney();
			break;
		}
		//退费
		case 6: {
			printf("--------退费--------\n");
			refundMoney();
			break;
		}
		//查询统计
		//注销卡
		case 7: {
			printf("--------注销卡-------\n");
			annul();
			break;
		}
		//调试入口
		case 8: {
			printf("--------管理员模式--------\n");
			admMode();
			break;
		}
		//退出
		case 0: {
			printf("--------退出--------\n");
			exitAMS();
			break;
		}
		default:{
			printf("输入的菜单编号错误\n");
			break;
		}
		}
		printf("\n");
	} while (nSelection);
	system("pause");
	return 0;
}
