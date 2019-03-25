#include<stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>


#define N 32
#define SIZE sizeof(Msg)

char num2[N] = {0};

#define my_printf	do\
{\
	printf("%-15s%-15c%-15s%-15s%-15s%-15s%-15s%-15s%-15c%-15s\n",\
			msg.info.name,msg.info.sex,msg.info.num,msg.info.spec,\
			msg.info.nation,msg.info.tell,msg.info.qq,msg.info.duty,msg.info.jur,msg.info.pass);\
}while(0)

#define my_printf1	do\
{\
	printf("%-15s%-15c%-15s%-15s%-15s%-15s%-15s%-15s%-15c******\n",\
			msg.info.name,msg.info.sex,msg.info.num,msg.info.spec,\
			msg.info.nation,msg.info.tell,msg.info.qq,msg.info.duty,msg.info.jur);\
}while(0)


typedef struct Info{
	char name[N];
	char sex;
	char num[N];
	char spec[N];
	char nation[N];
	char tell[N];
	char qq[N];
	char duty[N];
	char jur;
	char pass[N];
}Info;

typedef struct Msg{
	char type;
	struct Info info;
}Msg;

typedef struct His{
	char type;
	char text[128];
	char tim[64];
	char name[N];
	char a[31];
}His;

int my_send(int sockfd,Msg *msg)
{

	if(send(sockfd,msg,sizeof(*msg),0) < 0)
	{
		perror("send error:");
		return -1;
	}
	else 
		return 0;
}

int my_recv(int sockfd, Msg *msg)
{
	int ret = 0;

	if((ret = recv(sockfd,msg,sizeof(*msg),0)) < 0)
	{
		perror("recv error:");
		return -1;
	}
	else
		//	printf("-----------ret = %d\n",ret);
		return 0;
}

//学号查询
int num_quene(int sockfd,Msg msg)
{
again2:
	msg.type = 'n';
	printf("输入学号:");
	scanf("%s",msg.info.num);
	getchar();
	if(strlen(msg.info.num) != 8)
	{
		memset(msg.info.num,0,sizeof(msg.info.num));
		printf("请输入正确学号\n");
		goto again2;
	}
	my_send(sockfd,&msg);
	my_recv(sockfd,&msg);
	if(msg.type == 'n'){
		printf("此学号的用户不存在\n");
		return 0 ;
	}
	printf("%-15s%-20s%-17s%-15s%-15s%-20s%-15s%-15s%-15s%-15s\n","姓名","性别","学号","专业","民族","电话","QQ","职务","权限","密码");
	if(msg.info.jur == 'r')
		my_printf1;
	else	
		my_printf;
	my_recv(sockfd,&msg);
	return 0;

}

//查询全部
int all_quene(int sockfd,Msg msg)
{
	msg.type = 'a';
	my_send(sockfd,&msg);
	printf("%-15s%-20s%-17s%-15s%-15s%-20s%-15s%-15s%-15s%-15s\n","姓名","性别","学号","专业","民族","电话","QQ","职务","权限","密码");
again3:
	my_recv(sockfd,&msg);
	if(msg.type == 'y'){
		if(msg.info.jur == 'r')
			my_printf1;
		else	
			my_printf;
		goto again3;
	}
	else{
		printf("显示完毕\n");

	}
	return 0;

}

//删除用户
int del_user(int sockfd,Msg msg)
{
	msg.type = 'd';
again4:
	printf("输入学号:");
	scanf("%s",msg.info.num);
	getchar();
	if(strlen(msg.info.num) != 8)
	{
		memset(msg.info.num,0,sizeof(msg.info.num));
		printf("请输入正确学号\n");
		goto again4;
	}
	my_send(sockfd,&msg);
	my_recv(sockfd,&msg);
	if(msg.type == 'n'){
		printf("此用户不存在\n");
	}
	else{
		printf("删除成功\n");
	}

	return 0;

}

//添加用户
int add_user(int sockfd,Msg msg)
{

	msg.type = 'A';
	printf("请输入姓名:");
	scanf("%s",msg.info.name);
	getchar();
	printf("请输入性别(男:m,女:f):");
	scanf("%c",&msg.info.sex);
	getchar();
	printf("请输入学号(8位):");
	scanf("%s",msg.info.num);
	getchar();
	printf("请输入专业:");
	scanf("%s",msg.info.spec);
	getchar();
	printf("请输入民族:");
	scanf("%s",msg.info.nation);
	getchar();
	printf("请输入电话号(11位):");
	scanf("%s",msg.info.tell);
	getchar();
	printf("请输入QQ号:");
	scanf("%s",msg.info.qq);
	printf("请输入职务(队长，副队长，队员):");
	scanf("%s",msg.info.duty);
	getchar();
again9:

	printf("请输入权限(管理员:r,普通用户:u):");
	scanf("%c",&msg.info.jur);
	getchar();
	if((msg.info.jur != 'r') && (msg.info.jur != 'u'))
	{
		printf("权限输入错误，请重新输入\n");
		goto again9;
	}
	printf("请输入密码:");
	scanf("%s",msg.info.pass);
	getchar();
	my_send(sockfd,&msg);
	my_recv(sockfd,&msg);
	if(msg.type == 'n')
	{
		printf("此学号的用户已存在\n");
	}

	/*else{
	  printf("-------------%c\n",msg.type);
	  }*/

	return 0;
}

//修改信息
int change_infor(int sockfd,Msg msg)
{
	int choiece = 0;
	char num1[N] = {0};
	strcpy(num1,num2);
	msg.type = 'm';
again5:
	printf("输入要修改用户的学号:");
	scanf("%s",msg.info.num);
	getchar();
	if(strlen(msg.info.num) != 8)
	{
		memset(msg.info.num,0,sizeof(msg.info.num));
		printf("请输入正确学号\n");
		goto again5;
	}
	my_send(sockfd,&msg);
	my_recv(sockfd,&msg);
	if(msg.type == 'y')
	{
		if((msg.info.jur == 'r') && strcmp(num1,msg.info.num))
		{
			my_printf1;
			printf("此用户是同级管理员，不可修改\n");
			msg.type = 'e';
			my_send(sockfd,&msg);
		}
		else
		{
			msg.type = 'y';
			my_send(sockfd,&msg);
			printf("%-15s%-20s%-17s%-15s%-15s%-20s%-15s%-15s%-15s%-15s\n","姓名","性别","学号","专业","民族","电话","QQ","职务","权限","密码");
			my_printf;
			while(1)
			{
				printf("-----1<姓名> 2<性别> 3<学号> 4<专业> 5<民族>-----\n");
				printf("-----6<电话> 7<QQ>  8<职务> 9<权限> 10<密码>-----\n");
				printf("-----------------0<保存信息>---------------------\n");
				printf("请输入编号:");
				scanf("%d",&choiece);
				getchar();
				switch(choiece)
				{
				case 0:
					goto again7;
					break;
				case 1:
					printf("请输入姓名:");
					scanf("%s",msg.info.name);
					getchar();
					break;
				case 2:
					printf("请输入性别(男:m,女:f):");
					scanf("%c",&msg.info.sex);
					getchar();
					break;
				case 3:
					printf("请输入学号(8位):");
					scanf("%s",msg.info.num);
					getchar();
					break;
				case 4:
					printf("请输入专业:");
					scanf("%s",msg.info.spec);
					getchar();
					break;
				case 5:
					printf("请输入民族:");
					scanf("%s",msg.info.nation);
					getchar();
					break;
				case 6:
					printf("请输入电话号(11位):");
					scanf("%s",msg.info.tell);
					getchar();
					break;
				case 7:
					printf("请输入QQ号:");
					scanf("%s",msg.info.qq);
					getchar();
					break;
				case 8:
					printf("请输入职务:");
					scanf("%s",msg.info.duty);
					getchar();
					break;
				case 9:
again10:printf("请输入权限(管理员:r,普通用户:u):");
		scanf("%c",&msg.info.jur);
		getchar();
		if((msg.info.jur != 'r') && (msg.info.jur != 'u'))
		{
			printf("权限输入错误，请重新输入\n");
			goto again10;
		}
		break;
				case 10:
		printf("请输入密码:");
		scanf("%s",msg.info.pass);
		getchar();
		break;
				default:
		break;
				}
			}
again7:			
			my_send(sockfd,&msg);

		}
	}
	if(msg.type == 'n')
	{
		printf("此用户不存在\n");
	}
	return 0;

}

//历史记录
int get_history(int sockfd,Msg msg)
{
	His his;
	int ret;
	msg.type = 'g';
	my_send(sockfd,&msg);
	if(recv(sockfd,&his,sizeof(struct His),0) < 0)
	{
		printf("recv error\n");
		return -1;
	}
	while(his.type != 'n')
	{
		printf("%s,%s,%s\n",his.name,his.tim,his.text);
		memset(&his,0,sizeof(his));
		if((ret =recv(sockfd,&his,sizeof(struct His),0)) < 0)
		{
			perror("recv error:");
			return -1;
		}
	}
	printf("接收完毕\n");
	return 0;
}

//返回上一级
int go_back(int sockfd,Msg msg)
{
	msg.type = 'q';
	my_send(sockfd,&msg);

	return 0;
}
//管理员模式
int do_root(int sockfd, Msg msg)
{

	int choiece;
	msg.type = 'r';
	printf("请输入用户名:");
	scanf("%s",msg.info.name);
	getchar();

	printf("请输入密码:");
	scanf("%s",msg.info.pass);
	getchar();
	msg.info.jur = 'r';
	if(send(sockfd,&msg,sizeof(msg),0) < 0)
	{
		printf("send error\n");
		return -1;
	}
	if(recv(sockfd,&msg,sizeof(msg),0) < 0)
	{
		printf("recv error\n");
		return -1;
	}

	if(msg.type == 'n'){
		printf("输入的用户名或密码错误\n");
		return 0;
	}
	strcpy(num2,msg.info.num);
	while(1){
		printf("-----1<学号查询> 2<查询全部> 3<删除用户> 4<添加用户>-----\n");
		printf("-----5<修改信息> 6<历史记录> 7<返回上级>-----------------\n");
		printf("请输入编号:");
		scanf("%d",&choiece);
		getchar();
		switch(choiece){
		case 1:
			num_quene(sockfd,msg);
			break;
		case 2:
			all_quene(sockfd,msg);
			break;
		case 3:
			del_user(sockfd,msg);
			break;
		case 4:
			add_user(sockfd,msg);
			break;
		case 5:
			change_infor(sockfd,msg);
			break;
		case 6:
			get_history(sockfd,msg);
			break;
		case 7:
			go_back(sockfd,msg);
			return 0;
		default:
			printf("please input 1-7\n");
			break;
		}
	}
	return 0;
}
//查询信息
int user_quene(int sockfd,Msg *msg1)
{
	Msg msg;
	msg1->type = 'g';
	my_send(sockfd,msg1);
	my_recv(sockfd,msg1);
	msg = *msg1;
	printf("%-15s%-20s%-17s%-15s%-15s%-20s%-15s%-15s%-15s%-15s\n","姓名","性别","学号","专业","民族","电话","QQ","职务","权限","密码");
	my_printf;
	my_recv(sockfd,msg1);
	return 0;
}
//修改信息
int user_change(int sockfd,Msg *msg1)
{
	Msg msg;
	msg1->type = 'm';
	my_send(sockfd,msg1);
	my_recv(sockfd,msg1);
	msg = *msg1;
	printf("%-15s%-20s%-17s%-15s%-15s%-20s%-15s%-15s%-15s%-15s\n","姓名","性别","学号","专业","民族","电话","QQ","职务","权限","密码");
	my_printf;
	int choiece = 0;
	while(1)
	{
		printf("-----1<性别> 2<专业> 3<民族>-----\n");
		printf("-----4<电话> 5<QQ>   6<密码>-----\n");
		printf("修改完成，请输入0保存\n");
		printf("请输入编号:");
		scanf("%d",&choiece);
		getchar();
		switch(choiece)
		{
		case 0:
			goto again8;
			break;
		case 1:
			printf("请输入性别:");
			scanf("%c",&msg1->info.sex);
			getchar();
			break;
		case 2:
			printf("请输入专业:");
			scanf("%s",msg1->info.spec);
			getchar();
			break;
		case 3:
			printf("请输入民族:");
			scanf("%s",msg1->info.nation);
			getchar();
			break;
		case 4:
			printf("请输入电话号:");
			scanf("%s",msg1->info.tell);
			getchar();
			break;
		case 5:
			printf("请输入QQ号:");
			scanf("%s",msg1->info.qq);
			getchar();
			break;
		case 6:
			printf("请输入密码:");
			scanf("%s",msg1->info.pass);
			getchar();
			break;
		default:
			break;
		}
	}
again8:
	my_send(sockfd,msg1);
	return 0;
}

//用户模式
int do_user(int sockfd ,Msg msg)
{

	int choiece;
	msg.type = 'u';
	printf("请输入用户名:");
	scanf("%s",msg.info.name);
	getchar();

	printf("请输入密码:");
	scanf("%s",msg.info.pass);
	getchar();
	msg.info.jur = 'u';
	if(send(sockfd,&msg,sizeof(msg),0) < 0)
	{
		printf("send error\n");
		return -1;
	}
	if(recv(sockfd,&msg,sizeof(msg),0) < 0)
	{
		printf("recv error\n");
		return -1;
	}
	usleep(10);
	if(msg.type == 'n'){
		printf("输入的用户名或密码错误\n");
		return 0;
	}
	while(1){
again6:
		printf("----------1<查询信息> 2<修改信息> 3<返回上级>----------\n");
		scanf("%d",&choiece);
		getchar();
		switch(choiece){
		case 1:
			user_quene(sockfd,&msg);
			break;
		case 2:
			user_change(sockfd,&msg);
			break;
		case 3:
			go_back(sockfd,msg);
			return 0;
		default:
			goto again6;
			break;

		}
	}
	return 0;
}

int do_quit(int sockfd, Msg msg)
{

	msg.type = 'q';
	my_send(sockfd,&msg);
}


int main(int argc, const char *argv[])
{
	//socket->填充->绑定->监听->等待连接->数据交互->关闭 
	int sockfd;
	int acceptfd;
	ssize_t recvbytes,sendbytes;
	//char buf[N] = {0};
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	socklen_t cli_len = sizeof(clientaddr);
	//创建网络通信的套接字
	sockfd = socket(AF_INET,SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket failed.\n");
		exit(-1);
	}
	printf("sockfd :%d.\n",sockfd); 


	//填充网络结构体
	memset(&serveraddr,0,sizeof(serveraddr));
	memset(&clientaddr,0,sizeof(clientaddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port   = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sockfd,(const struct sockaddr *)&serveraddr,addrlen) == -1){
		perror("connect failed.\n");
		exit(-1);
	}
	Msg msg;
	memset(&msg,0,sizeof(msg));
	int choiece = 0;
	while(1)
	{
		printf("****************************************\n");
		printf("**********1 root 2 user 3 quit**********\n");
		printf("****************************************\n");
again:	scanf("%d",&choiece);
		switch(choiece){
		case 1:
			do_root(sockfd,msg);
			break;
		case 2:
			do_user(sockfd,msg);
			break;
		case 3:
			do_quit(sockfd,msg);
			return 0;
		default:
			printf("please input 1 or 2 or 3\n");
			goto again;
		}

	}
}
