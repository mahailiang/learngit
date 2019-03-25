#include "databass.h"
#include <time.h>
int  add_sql_history(struct History *history)
{
	//获取时间 
	struct tm *tp;
	time_t t;
	char timedata[64] = {0}; 
	time(&t);
	tp = localtime(&t);
	sprintf(timedata,"%d-%d-%d %d:%d:%d",tp->tm_year+1900,tp->tm_mon+1,\
			tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
	//添加到数据库
	sprintf(sql,"insert into  history values('%s','%s','%s');",\
			history->word,timedata,history->name);

	if(SQLITE_OK !=sqlite3_exec(db,sql,NULL,NULL,&errmsg))
	{
		printf("sqlite3_exec error ******: %s\n",errmsg);
	}
	else
	{
		printf("新加历史记录成功:\n");
	}


	return OK;

}

//添加管理员操作
void add_root_handle(struct Msg *msg,char*name)
{
	struct History *history;
	//获取时间
	strcpy(history->name,name);
	switch(msg->type)
	{
	case 'n':

		sprintf(history->word,"通过学号:%s查看信息",msg->info.num);
			add_sql_history(history);
		break;
	case 'a':
		sprintf(history->word,"查看全部信息");
			add_sql_history(history);
		break;
	case 'd':
		sprintf(history->word,"删除学号为:%s信息",msg->info.num);
			add_sql_history(history);
		break;
	case 'A':
		sprintf(history->word,"添加成员%s的信息",msg->info.name);
			add_sql_history(history);
		break;
	case 'm':
		sprintf(history->word,"通过学号:%s修改信息",msg->info.num);
			add_sql_history(history);
		break;
	case 'g':
		sprintf(history->word,"查看历史信息");
			add_sql_history(history);
		break;
	case 'h':
		sprintf(history->word,"添加信息失败,学号:%s已存在\n",msg->info.num);
			add_sql_history(history);
		break;
	case 'q':
		sprintf(history->word,"退出");
			add_sql_history(history);
		break;
	}


}
//添加普通用户的操作
void add_user_handle(struct Msg *msg,char c)
{
	struct History *history;
	//获取时间
	strcpy(history->name,msg->info.name);
	switch(c)
	{
	case 'g':
		sprintf(history->word,"查看了信息",msg->info.num);
			add_sql_history(history);
		break;
	case 'm':
		sprintf(history->word,"修改了信息");
			add_sql_history(history);
		break;
	case 'q':
		sprintf(history->word,"退出");
			add_sql_history(history);
		break;
	}


}
