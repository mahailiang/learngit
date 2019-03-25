#include "databass.h"


//接受数据函数
char my_recv(struct Msg *msg,int acceptfd)
{
	int ret;
	int recvbytes;
	struct Msg m1;
	if((ret = recv(acceptfd,&m1, SIZE, 0)) < 0)
	{
		perror("recv error");
		return ERROR;
	}
	if(ret == 0)
	{
		printf("连接中断\n");
		return ERROR;
	}

	*msg = m1;
	return OK;

}
int do_select_stu(void *arg,int ncolum,char **f_value,char **f_name)
{
	
	printf("成员信息存在\n");
	struct Msg *msg = (struct Msg *)arg;
	msg->type = 'y';
	sprintf(sql,"delete from stuinfo where num = %s;",msg->info.num);
	//执行一条sql语句
	if(SQLITE_OK !=sqlite3_exec(db,sql,NULL,NULL,&errmsg))
	{
		printf("sqlite3_exec error1 : %s",errmsg);
		return ERROR;
	}
	else
	{  
		printf("do_sqlite_add sucess1\n");
	}
	printf("成功删除成员信息\n");
return 0;
}

//删除信息的函数
char del_user(struct Msg *msg,int acceptfd)
{
	msg->type = 'n';
	printf("判断是有有这样的用户\n");
	sprintf(sql,"select * from stuinfo where num = %s;",msg->info.num);
	//执行一条sql语句
	if(SQLITE_OK !=sqlite3_exec(db,sql,do_select_stu,msg,&errmsg))
	{
		printf("sqlite3_exec error1 : %s",errmsg);
		return ERROR;
	}
	else
	{
		printf("do del_user sucess1\n");
	}
	my_send(msg,acceptfd);
	return OK;
}
int do_change_stu(void *arg,int ncolum,char **f_value,char **f_name)
{
	struct Find *find = (struct Find *)(arg);
	//判断是有数据且返回原有数据
	printf("此成员信息存在\n");
	find->msg.type = 'y';
	/*
	char ss[N];
	strcpy(ss,find->msg.info.num);
		printf("ss = %s\n",ss);
*/
	get_value(&(find->msg),f_value);
	/*	printf("num = %s\n",find->msg.info.num);
	if(find->msg.info.jur == 'r' && strcmp(find->msg.info.num,ss) != 0 )
	{
		
	find->msg.type = 's';
		my_send(&(find->msg),find->acceptfd);
		return 0;
	}*/
		my_send(&(find->msg),find->acceptfd);
		//接受返回信息查看是否是其他管理员
		my_recv(&(find->msg),find->acceptfd);
		if(find->msg.type == 'e')
		{
			find->msg.type = 's';
	//是其他管理员用户不执行后面操作
			return 0;
		}
	//删除原有数据
	sprintf(sql,"delete from stuinfo where num = %s;",find->msg.info.num);
	//执行一条sql语句
	if(SQLITE_OK !=sqlite3_exec(db,sql,NULL,NULL,&errmsg))
	{
		printf("sqlite3_exec error1 : %s",errmsg);
		return ERROR;
	}
	else
	{  
		printf("do_sqlite_add sucess1\n");
	}
	printf("修改后的标签是%c\n",find->msg.type);
	
	return 0;
}

//按照学号查询信息的函数
char num_quene(struct Msg *msg,int acceptfd)
{
	char ret;
	printf("进入学号查询函数\n");
	ret = do_query_data('r','n',msg,db,acceptfd);
	return ret;
	//数据查找信息是否存在
}
//查询全部的函数
char all_quene(struct Msg *msg,int acceptfd)
{	char ret;
	ret = do_query_data('r','a',msg,db,acceptfd);
	msg->type = 0;
	return ret;

	//数据库信息返回
}
int do_add_stu(void *arg,int ncolum,char **f_value,char **f_name)
{
	printf("信息存在\n");
	Msg *msg = (Msg *)arg;
	msg->type = 'n';
	return 0;
}
//添加学员信息的函数
char add_user(struct Msg *msg,int acceptfd)
{
	Msg msg1 = *msg;
	msg->type == 'y';
	sprintf(sql,"select * from stuinfo where num = %s;",msg->info.num);
	//执行一条sql语句
	if(SQLITE_OK !=sqlite3_exec(db,sql,do_add_stu,(void *)msg,&errmsg))
	{
		printf("add_user sqlite3 sql error : %s\n",errmsg);
		return ERROR;
	}
	else
	{
		printf("do_sqlite_add sucess1\n");
	}
	printf("此时的标签是 %c\n",msg->type);
	my_send(msg,acceptfd);
	if(msg->type == 'n')
	{
		printf("添加失败,学号信息存在\n");
		return 2;
	}

	sprintf(sql,"insert into  stuinfo values('%s','%c','%s','%s','%s','%s','%s','%s','%c','%s');",
			msg1.info.name,msg1.info.sex,msg1.info.num,msg1.info.spec,msg1.info.nation,msg1.info.tell,
			msg1.info.qq,msg1.info.duty,msg1.info.jur,msg1.info.pass);

	if(SQLITE_OK !=sqlite3_exec(db,sql,NULL,NULL,&errmsg))
	{

		printf("add_user error ******: %s\n",errmsg);
		return ERROR;
	}
	else
	{
		printf("do_sqlite_add sucess\n");
		printf("新加入用户:%s\n",msg1.info.name);
	}

	//接受新数据
	
	return OK;
}
//修改信息(通过学号)的函数
char change_info(struct Msg *msg,int acceptfd)
{
	int ret;
	Find find;
	find.msg = *msg;
	find.acceptfd = acceptfd;
	find.msg.type = 'n';
	printf("开始修改\n");
	sprintf(sql,"select * from stuinfo where num = %s;",msg->info.num);
	//执行一条sql语句
	if(SQLITE_OK !=sqlite3_exec(db,sql,do_change_stu,(void *)(&find),&errmsg))
	{
		printf("sqlite3_exec error1 : %s",errmsg);
		return ERROR;
	}
	else
	{
		printf("do_sqlite_add sucess1\n");
	}
	printf("此时的标签是%c\n",find.msg.type);
	//管理员用户或没有用户
	
	if(find.msg.type == 's')
	{
		printf("管理员信息不能修改\n");
	return OK;
	}
	if(find.msg.type == 'n')
	{
		my_send(&(find.msg),find.acceptfd);
		printf("用户不存在不能修改\n");
	return OK;
	}
	//接受修好后的数据
	ret = my_recv(msg,acceptfd);
	if(ret == ERROR)
		return ERROR;
	Msg msg1 = *msg;
	sprintf(sql,"insert into  stuinfo values('%s','%c','%s','%s','%s','%s','%s','%s','%c','%s');",
			msg1.info.name,msg1.info.sex,msg1.info.num,msg1.info.spec,msg1.info.nation,msg1.info.tell,
			msg1.info.qq,msg1.info.duty,msg1.info.jur,msg1.info.pass);

	if(SQLITE_OK !=sqlite3_exec(db,sql,NULL,NULL,&errmsg))
	{

		printf("add_user error ******: %s\n",errmsg);
		return ERROR;
	}
	else
	{
		printf("do_sqlite_add sucess\n");
		printf("用户:%s的数据修改成功\n",msg1.info.name);
	}
	return OK;
}
void get_history_value(History *history,char**f_value)
{
	strcpy(history->word, f_value[0]);
	strcpy(history->timedata , f_value[1]);
	strcpy(history->name , f_value[2]);
}
int do_put_history(void *arg,int ncolum,char **f_value,char **f_name)
{
	printf("开始发送历史信息\n");
	struct Find *find = (struct Find *)(arg);
	int i = 0;
	if(0 == flag)
	{
		for(i = 0; i < ncolum; i++)
			printf("%-15s",f_name[i]);
		printf("\n********************************************************************\n");
		flag = 1;
	}
		find->history.type = 'y';
	get_history_value(&(find->history),f_value);
	for(i = 0; i < ncolum; i++)
	{
		printf("%-15s",f_value[i]);
	}
	
     printf("%s,%s,%s\n",find->history.word,find->history.timedata,find->history.name);
int ret;
	 if((ret = send(find->acceptfd,&(find->history),sizeof(struct History),0)) == -1)
	{
		perror("send history error");
		return ERROR;
	}
	printf("sizeof = %d,ret = %d",sizeof(struct History),ret);
	printf("成功发送一次,%c\n",find->history.type);
	usleep(20000);
	printf("\n");
	return 0;
}


//查看历史信息的函数
char get_history(struct Msg *msg,int acceptfd)
{	
	
	printf("进打印历史记录\n");
	sprintf(sql,"select * from history;");
	Find find;
	find.acceptfd = acceptfd;
	flag = 0;
	//执行一条sql语句
	if(SQLITE_OK !=sqlite3_exec(db,sql,do_put_history,(void *)(&find),&errmsg))
	{
		printf("sqlite3_exec 查看历史信息 error1 : %s\n",errmsg);
		return ERROR;
	}
	else
	{
		printf("do_sqlite_add sucess1\n");
	}


	find.history.type = 'n';
	printf("发送完成\n");

	if(send(acceptfd,&(find.history),sizeof(struct History),0) == -1)
	{
		perror("send history error");
		return ERROR;
	}
	//打印数据库历史信息表中的信息
	return OK;
}


int do_log_in(void *arg,int ncolum,char **f_value,char **f_name)
{
	printf("进入回调函数\n");
	struct Msg *msg1 = (struct Msg *)arg;
	msg1->type = 'y';
	get_value(msg1,f_value);
	return 0;
}
//管理员登录界面的函数
char Administrator(struct Msg *msg,int acceptfd)
{
	Msg msg1 = *msg;
	char ret;
	int recvbytes;
	printf("----------------\n");
	struct History history;
	strcpy(history.name,msg->info.name);
	//读取用户和密码
	printf("进入管理员登录;\n");
	msg->type = 'n';
	//判断账号密码是否匹配
	printf("--------\n");
	printf("%s,%s,%c\n",msg->info.name,msg->info.pass,msg->info.jur);

	sprintf(sql,"select * from stuinfo where name = '%s' and jur ='%c' and pass = '%s';",
			msg->info.name,msg->info.jur,msg->info.pass);
	//执行一条sql语句
	if(SQLITE_OK !=sqlite3_exec(db,sql,do_log_in,(void *)msg,&errmsg))
	{
		printf("sqlite3_exec error1 : %s\n",errmsg);
		return ERROR;
	}
	else
	{
		printf("do_sqlite_add sucess1\n");
	}
	printf("判断成功,msg->type:%c\n",msg->type);

	if(msg->type == 'y')
	{	
		recvbytes  = send(acceptfd, msg , SIZE, 0);
		if(recvbytes == -1)
		{
			perror("send error");
			return ERROR;
		}
		printf("管理用户%s登录成功\n",msg->info.name);
		//修改用户会改变当前登录name,提前保存
		char name[N];
		strcpy(name,msg->info.name);
		sprintf(history.word,"管理员登录成功");
	//	printf("+++++++++++++++++++++++\n");
		add_sql_history(&history);
		while(1)
		{
			ret = my_recv(&msg1,acceptfd);
			if(ret == ERROR)
				return ERROR;
			printf("接受到数据,%c\n",msg1.type);
			/*
			*/
			//判断是否管理员
			switch(msg1.type)
			{
			case 'n':
				//按照学号查询
				add_root_handle(&msg1,name);
				if(num_quene(&msg1,acceptfd) == ERROR)
					return ERROR;
				break;
			case 'a':
				//查询全部
				add_root_handle(&msg1,name);
				if(all_quene(&msg1,acceptfd) == ERROR)
					return ERROR;
				break;
			case 'd':
				//删除信息
				add_root_handle(&msg1,name);
				if(del_user(&msg1,acceptfd) == ERROR)
					return ERROR;
				break;
			case 'A':
				//添加学员信息 
				add_root_handle(&msg1,name);
				if((ret = add_user(&msg1,acceptfd)) == ERROR)
					return ERROR;
			if(ret == 2)
			{
				msg1.type = 'h';
				add_root_handle(&msg1,name);
			}	
			break;
			case 'm':
				//修改信息(通过学号)
				add_root_handle(&msg1,name);
				if(change_info(&msg1,acceptfd) == ERROR)
					return ERROR;
				break;
			case 'g':
				//查看历史信息
				add_root_handle(&msg1,name);
				if(get_history(&msg1,acceptfd) == ERROR)
					return ERROR;
				break;
			case 'q':
				add_root_handle(&msg1,name);
				return OK;
				//退出上一级

			}
		}

	}
	else
	{
		msg1.type = 'n';
		printf("登录失败\n");
		recvbytes  = send(acceptfd, &msg1 , sizeof(char), 0);
		if(recvbytes == -1)
		{
			perror("send error");
			return ERROR;
		}
	}

}
//普通用户端数据查询
char user_num_quene(struct Msg *msg,int acceptfd)
{

	return do_query_data('u','n',msg,db,acceptfd);
}
int user_change_exec(void *arg,int ncolum,char **f_value,char **f_name)
{
	struct Msg *msg = (struct Msg *) arg;
	//获取信息
	get_value(msg,f_value);
	
	return 0;
}
//普通用户端数据修改
char user_change_quene(struct Msg *msg,int acceptfd)
{

	//查找原有数据原有数据 
	sprintf(sql,"select * from stuinfo where num = '%s';",msg->info.num);
	//执行一条sql语句
	if(SQLITE_OK !=sqlite3_exec(db,sql,user_change_exec,(void *)msg,&errmsg))
	{
		printf("sqlite3_exec error1 : %s",errmsg);
		return ERROR;
	}
	else
	{  
		printf("do_sqlite_add sucess1\n");
	}
	//当前信息发送回去

	Msg msg2 = *msg;

	if(send(acceptfd,msg , SIZE, 0) == -1)
	{
		perror("send error");
		return ERROR;
	}


	if(my_recv(msg,acceptfd) == ERROR)
		return ERROR;

//删除原有数据

	sprintf(sql,"delete from stuinfo where num = '%s';",msg2.info.num);
	//执行一条sql语句
	if(SQLITE_OK !=sqlite3_exec(db,sql,NULL,NULL,&errmsg))
	{
		printf("sqlite3_exec error1 : %s",errmsg);
		return ERROR;
	}
	else
	{  
		printf("do_sqlite_add sucess1\n");
	}
	printf("成功删除成员原有信息\n");

	//上传新数据
	Msg msg1 = *msg;
	sprintf(sql,"insert into  stuinfo values('%s','%c','%s','%s','%s','%s','%s','%s','%c','%s');",
			msg1.info.name,msg1.info.sex,msg1.info.num,msg1.info.spec,msg1.info.nation,msg1.info.tell,
			msg1.info.qq,msg1.info.duty,msg1.info.jur,msg1.info.pass);

	if(SQLITE_OK !=sqlite3_exec(db,sql,NULL,NULL,&errmsg))
	{

		printf("add_user error ******: %s\n",errmsg);
		return ERROR;
	}
	else
	{
		printf("do_sqlite_add sucess\n");
		printf("用户:%s的数据修改成功\n",msg1.info.name);
	}
	return OK;


}
//普通用户登录界面
char Common(struct Msg *msg,int acceptfd)
{
	char ret;
	int recvbytes;
	struct History history;
	strcpy(history.name,msg->info.name);
	//读取用户和密码
	msg->type = 'n';
	printf("--------\n");
	printf("%s,%s,%c\n",msg->info.name,msg->info.pass,msg->info.jur);

	sprintf(sql,"select * from stuinfo where name = '%s' and jur ='%c' and pass = '%s';",
			msg->info.name,msg->info.jur,msg->info.pass);
	//执行一条sql语句
	if(SQLITE_OK !=sqlite3_exec(db,sql,do_log_in,(void *)msg,&errmsg))
	{
		printf("sqlite3_exec error1 : %s\n",errmsg);
		return ERROR;
	}
	else
	{
		printf("do_sqlite_add sucess1\n");
	}
	printf("判断成功,msg->type:%c\n",msg->type);

	if(msg->type == 'y')
	{	

		recvbytes  = send(acceptfd,msg , SIZE, 0);
		if(recvbytes == -1)
		{
			perror("send error");
			return ERROR;
		}
		printf("普通用户%s登录成功\n",msg->info.name);
		sprintf(history.word,"%s登录成功",msg->info.name);
		add_sql_history(&history);
		while(1)
		{
			ret = my_recv(msg,acceptfd);
			if(ret == ERROR)
				return ERROR;
			printf("接受到数据,%c\n",msg->type);
			/*
			*/
			//判断是否管理员
			switch(msg->type)
			{
			case 'g':
				//按照学号查询
				add_user_handle(msg,'g');
				if(user_num_quene(msg,acceptfd) == ERROR)
					return ERROR;
				break;
			case 'm':
				//查询全部
				add_user_handle(msg,'m');
				if(user_change_quene(msg,acceptfd) == ERROR)
					return ERROR;
				break;
			case 'q':
				//删除信息
				add_user_handle(msg,'q');
				return OK;
				break;
			}
		}
	}
	else
	{
		msg->type = 'n';
		recvbytes  = send(acceptfd,msg , SIZE, 0);
		if(recvbytes == -1)
		{
			perror("send error");
			return ERROR;
		}
	}
	return OK;


}
//线程函数
void *  client_request_handler(void * args)
{	
	printf("创建线程\n");
	Msg msg;
	char buf[N];
	int ret;
	int acceptfd = *((int*)args);
	//分离自动释放资源
	pthread_detach(pthread_self());
	while(1)
	{
		if( my_recv(&msg,acceptfd) == ERROR)
			pthread_exit(NULL);
		printf("接受到数据,%c\n",msg.type);
		//判断是否管理员
		switch(msg.type)
		{
		case 'r':
			//管理员登录
			if(Administrator(&msg,acceptfd) == ERROR)
			goto loop;
			break;

		case 'u':
			//普通用户登录界面
			if(Common(&msg,acceptfd) == ERROR)
			goto loop;
			break;
		case 'q':
			//退出
			goto loop;

		}
		/*	printf("接受到数据:%s\n",buf);
			if(strncmp(buf,"quit",4) == 0)
			{
			printf("子线程退出\n");
			pthread_exit(NULL);
			}
			recvbytes  = send(acceptfd, "ok", sizeof("ok"), 0);
			if(recvbytes == -1)
			{
			perror("send error");
			exit(-1);
			}*/
	}
loop:
	printf("成功退出\n");
				pthread_exit(NULL);
	//pthread_exit(NULL);
}
int main(int argc, const char *argv[])
{
	if(argc < 3)
	{
		printf("输入信息有误");
		return -1;
	}
	int sockfd;
	int acceptfd;
	ssize_t recvbytes;
	char buf[N] = {0};
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	socklen_t cli_len = sizeof(clientaddr);

	//1.创建网络套接字

	sockfd = socket(AF_INET,SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket failed.\n");
		exit(-1);
	}
	printf("sockfd :%d.\n",sockfd); 
	//2.填充信息
	memset(&serveraddr,0,sizeof(serveraddr));
	memset(&clientaddr,0,sizeof(clientaddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port   = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	//3.绑定
	if(bind(sockfd, (const struct sockaddr *)&serveraddr,addrlen) == -1){
		printf("bind failed.\n");
		exit(-1);
	}

	//4.监听
	if(listen(sockfd,10) == -1){
		printf("listen failed.\n");
		exit(-1);
	}


	//定义一个表
	fd_set readfds;
	//清空表
	FD_ZERO(&readfds);
	//表中添加监听对象
	FD_SET(sockfd,&readfds);
	pthread_t thread[N];
	int tid = 0;

	int nfds = sockfd;
	int retval;
	int i = 0;

	//打开数据库
	do_sqlite3_open("sub.db",&db);
	//创建表
	do_create_table(db);	

	while(1)
	{
		//select之后表中数据会清除所以重新添加监听对象
		FD_SET(sockfd,&readfds);

		//5.select
		retval =select(nfds + 1, &readfds, NULL,NULL,NULL);

		//判断是否集合中关注的事件
		for(i = 0;i <= nfds; i ++)
		{
			printf("------i :%d.\n",i);
			if(FD_ISSET(i,&readfds))
			{
				if(i == sockfd)
				{
					//等待连接
					acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&cli_len);
					if(acceptfd == -1)
					{
						printf("acceptfd failed.\n");
						exit(-1);
					}
					printf("ip : %s.\n",inet_ntoa(clientaddr.sin_addr));
					//创建一个线程
					pthread_create(&(thread[tid++]),NULL,client_request_handler,(void *)&acceptfd);
				}
			}
		}

	}
	close(sockfd);
	return 0;
}
