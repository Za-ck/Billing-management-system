#pragma once

#define MAX_QUERY 100//最大匹配数

#define FALSE 0 //上机失败 
#define TRUE 1 //上机成功
#define UNUSE 2 //不能使用
#define ENOUGHMONEY 3 //余额不足

#define BILLINGPATH "E:\\dataBilling.ams"//计费信息地址
#define CARDPATH "E:\\dataCard.ams"//卡信息地址
#define MONEYPATH "E:\\dataMoney.txt"//充值退费信息

#define UNIT 15//最小收费单元（分钟）
#define CHARGE 0.5//每个计费单元收费（RMB：元）

#define CARDCHARNUM 256

#define CODENUM 4//验证码位数

#define ADMINISTRATOR "123"//管理员密码