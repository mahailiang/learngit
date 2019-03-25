
#include "databass.h"

//打开或创建数据库
char do_sqlite3_open(const char *filename,sqlite3 **db)
{
	int ret;
	if((ret = sqlite3_open(filename,db)) != SQLITE_OK)
	{
		printf("sqlite3_open error : %s",sqlite3_errmsg(*db));
		return -1;
	}
	return 0;
}
//关闭数据库
void do_sqlite3_close(sqlite3* db)
{
	sqlite3_close(db);
}
//创建用户以及历史记录
char do_create_table(sqlite3 *db)
{
	//创建一个表存储历史记录1
	sprintf(sql,"create table history(word text,time char,name char);");
	//执行一条sql语句达到创建一个表
	if(SQLITE_OK !=sqlite3_exec(db,sql,NULL,NULL,&errmsg))
	{
		if(116 != *errmsg)
		{
			printf("sqlite3_exec1 error : %s",errmsg);
			printf("---------------\n");
		}
		else
		printf("数据库中的 history 打开成功\n");
	}
	else
		printf("create table stuinfo sucess\n");

	//创建一个表存储用户信息
	sprintf(sql,"create table stuinfo(name text,sex char,num char,spec text, nation text,tell text,qq text,duty text,jur char,pass text);");

	//执行一条sql语句达到创建一个表
	if(SQLITE_OK !=sqlite3_exec(db,sql,NULL,NULL,&errmsg))
	{
		if(116 != *errmsg)
		{
			printf("sqlite3_exec1 error : %s",errmsg);
			printf("---------------\n");
		}
		else
		printf("数据库中的 stuinfo 打开成功\n");
	}
	else
		printf("create table stuinfo sucess\n");
}



int do_have_stu(void *arg,int ncolum,char **f_value,char **f_name)
{
	//数据存在修改标志
	//	printf("存在,修改标签\n");
	struct Msg *msg = (struct Msg *)arg;
	msg->type = 'y';
	//	printf("判断之后%c\n",msg->type);
	return 0;
}

//查询数据
//1.执行查询的用户权限(管理员查询还是普通用户查询)
//2.如果是管理元查询那没事什么方式查询?学号,全部
//3.信息结构体
//4.acceptfd
char do_query_data(char jur,char mode,Msg *msg,sqlite3 *db,int acceptfd)
{
	Find find;
	find.msg = *msg;
	find.acceptfd = acceptfd;
	//按照学号查询一个人的信息
	if('u' == jur || 'n' == mode)
	{
		printf("开始学号查询数据\n");
		//判断是否存在,
		msg->type = 'n';
		sprintf(sql,"select * from stuinfo where num = '%s';",msg->info.num);
		//执行一条sql语句\
		//数据存在则进入回调函数 do_have_stu
		if(SQLITE_OK !=sqlite3_exec(db,sql,do_have_stu,(void*)(msg),&errmsg))
		{
			printf("sqlite3_exec error1 : %s\n",errmsg);
			return ERROR;
		}
		else
		{
			printf("do_query_data get  sucess1\n");
		}
		//printf("判断之后%c\n",msg->type);
		if(msg->type == 'n')
		{
			printf("学号为%s的不存在\n",msg->info.num);
			goto loop;
		}
		else
			printf("该学号信息成员存在\n");
		sprintf(sql,"select * from stuinfo where num = '%s';",msg->info.num);
	}
	//管理员查看所有信息
	else 
		sprintf(sql,"select * from stuinfo;");
	flag = 0;
	//执行一条sql语句
	if(SQLITE_OK !=sqlite3_exec(db,sql,do_callback_find,(void *)(&find),&errmsg))
	{
		printf("sqlite3_exec error1 : %s",errmsg);
		return ERROR;
	}
	else
	{
		printf("do_sqlite_add sucess1\n");
	}
loop:
	//修改标签代表数据发送完毕,没有后序记录数据发送\
	//客户端可以结束当前操作
	msg->type = 'n';
	my_send(msg,acceptfd);
	printf("发送成功\n");
	return OK;

}
int do_callback_find(void *arg,int ncolum,char **f_value,char **f_name)
{
	struct Find *find = (struct Find *)(arg);
	int i = 0;
	if(0 == flag)
	{
		for(i = 0; i < ncolum; i++)
			printf("%-15s",f_name[i]);
		printf("\n********************************************************************\n");
		flag = 1;
	}
	//修改标签代表还有数据客户端需要继续接受
	find->msg.type = 'y';
	get_value(&(find->msg),f_value);
	Msg msg = find->msg;

	printf("%-15s%-15c%-15s%-15s%-15s%-15s%-15s%-15s%-15c%-15s\n",\
			msg.info.name,msg.info.sex,msg.info.num,msg.info.spec,\
			msg.info.nation,msg.info.tell,msg.info.qq,msg.info.duty,msg.info.jur,msg.info.pass);
	printf("fas:%c\n",find->msg.type);
	my_send(&(find->msg),find->acceptfd);

	printf("\n");
	return 0;
}
void get_value(Msg *msg,char**f_value)
{
	strcpy(msg->info.name,f_value[0]);
	msg->info.sex = *f_value[1];
	strcpy(msg->info.num, f_value[2]);
	strcpy(msg->info.spec , f_value[3]);
	strcpy(msg->info.nation , f_value[4]);
	strcpy(msg->info.tell , f_value[5]);
	strcpy(msg->info.qq , f_value[6]);
	strcpy(msg->info.duty , f_value[7]);
	msg->info.jur =  *f_value[8];
	strcpy(msg->info.pass , f_value[9]);
}


int my_send(Msg *msg,int acceptfd)
{

	int recvbytes;
	recvbytes  = send(acceptfd,msg, SIZE, 0);
	if(recvbytes == -1)
	{
		perror("send error");
		return ERROR;
	}
	//延时避免粘包
	usleep(20000);
	//printf("发送之后是:%c\n",msg->type);
	return OK;
}
