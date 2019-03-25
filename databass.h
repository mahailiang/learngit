#ifndef __DATABASS_H__
#define __DATABASS_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>


#define SIZE sizeof(struct Msg)
#define N 32
#define OK 1
#define ERROR 0

typedef struct History
{
	char type;
	char word[128];
	char timedata[64];
	char name[N];
	char c[31];
}History;
//成员信息结构体
typedef struct Info
{
	char name[N];
	char sex;
	char  num[N];
	char spec[N];
	char nation[N];
	char tell[N];
	char qq[N];
	char duty[N];
	char jur;
	char pass[N];
}Info;
//接受信息结构体
typedef struct Msg {
	char type; 			//访问方式
	struct Info info; 	//成员信息结构体
}Msg;
typedef struct Find
{
	struct Msg msg;
	int acceptfd;
	char myname[N];
	struct History history;
}Find;

sqlite3 *db;
char sql[128];
	char *errmsg;
char flag;
//打开或创建数据库
char do_sqlite3_open(const char *filename,sqlite3 **db);

//关闭数据库
void do_sqlite3_close(sqlite3* db);


//创建用户以及历史记录表
char do_create_table(sqlite3 *db);

//查询数据
//1.执行查询的用户权限(管理员查询还是普通用户查询)
//2.如果是管理元查询那没事什么方式查询?学号,全部
//3.信息结构体
//4.数据库句柄
char do_query_data(char jur,char mode,Msg *msg,sqlite3 *db,int acceptfd);
//查询数据库信息时的回调函数
int do_callback_find(void *arg,int ncolum,char **f_value,char **f_name);

void get_value(Msg *msg,char**f_value);

int my_send(Msg *msg,int acceptfd);
#endif
