/**
 * @FileName:      01_xianxing.c
 * @Abstract 				
 * @author ling
 * @version 1
 * @date 2018-12-13
 */
#include <stdio.h>
#include <stdlib.h>
#define N 10
#define OK 1
#define ERRNO 0
#define TRUE 1
#define FALSE 0
typedef char Status; 	//定义函数返回值的类型 如OK等
typedef int Elemtype; 	//定义元素的数据类型
ty
typedef struct List     //结构体定义
{
	Elemtype date[N];   
	int Length;         //元素个数的定义,计数从1开始,不同于数组下标
}List,*pList;

/* =======================================*//*{{{*/
/**
 * @Abstract   	创建一个结构体,并将结构体的地址通过q返回
 * 
 *
 * @Param     q 结构体地址的地址
 *
 * @Returns    	操作成功返回OK
 * 				操作失败返回ERRNO
 */
/* =======================================*/
Status initlist(pList *q)
{/*{{{*/a
	pList p = (pList)malloc(sizeof(List)); //申请空间
	if(NULL == p) //判断空间申请成功与否
		return ERRNO;
	p->Length = 0; //初始化结构体
	*q = p;        //传参,通过指针
	return OK;
}
/*}}}*/
/* =======================================*/
/**
 * @Abstract   释放结构体
 *
 * @Param    p 结构体的地址
 *
 * @Returns    成功返回OK 
 */
/* =======================================*/
Status list_free(pList p)
{/*{{{*/ 
	free(p);  //释放内存
	return OK;
}/*}}}*/
/* =======================================*/
/**
 * @Abstract   判断结构体中数值是否满了
 *
 * @Param    p 结构体的地址
 *
 * @Returns    满   返回TRUE
 * 			   没满 返回FALSE
 */
/* =======================================*/
Status list_full(pList p)
{/*{{{*/ 
//	if(N == p.Length)
//		return TRUE;
//	return FALSE;
	return (N == p->Length)?TRUE:FALSE;
}/*}}}*/

/* =======================================*/
/**
 * @Abstract 		添加一个元素e到最后一位数据位之后  
 *
 * @Param         P 结构体的地址
 *
 * @Param         e 参数
 *
 * @Returns       	操作成功返回OK
 * 					操作失败返回ERRNO
 */
/* =======================================*/
Status list_add_end(pList p,Elemtype e)
{/*{{{*/ 
	if(list_full(p))
		return ERRNO;
	p->date[p->Length++] = e;
	return OK;
}/*}}}*/
/* =======================================*/
/**
 * @Abstract   		显示所有数据
 *
 * @Param         	p 结构体的地址
 *
 * @Returns      	成功 OK
 */
/* =======================================*/
Status list_show(pList p)
{/*{{{*/
	int i;
	for(i = 0; i < p->Length; i++)
		printf("%d ",p->date[i]);
	printf("\n");
	return OK;
}/*}}}*/
/* =======================================*/
/**
 * @Abstract   		删除最后一个元素
 *
 * @Param         p 结构体的地址
 * @Param         e 用来存放删除的数据值
 *
 * @Returns       	成功放回OK
 * 					错误返回ERRNO
 */
/* =======================================*/
Status list_del_end(pList p,Elemtype *e)
{/*{{{*/
	if(0 == p->Length)
		return ERRNO;
	*e = p->date[--p->Length];
	return OK;
}/*}}}*/
/* =======================================*/
/**
 * @Abstract   		删除数组中的第i个元素,并将值送入e中
 *
 * @Param         P 结构体的地址 
 * @Param         i 
 * @Param         e
 *
 * @Returns        成功返回 OK
 * 				   失败返回 ERRNO	
 */
/* =======================================*/
Status list_del(pList p,int i,Elemtype *e)
{/*{{{*/
	int j;
	if(i > p->Length|| i <1)
		return ERRNO;
	*e = p->date[i-1];
	for(j = i - 1; j < p->Length - 1; j++)
	{
		p->date[j] = p->date[j+1];
	}
	p->Length--;
	return OK;
}/*}}}*/

/* =======================================*/
/**
 * @Abstract   		添加一个元素e到指定位之后,
 *
 * @Param         p 结构体的地址
 * @Param         i 位置参数 如:0代表的是写第一个空间参数,
 * 				    1代表的是写第二个空间参数
 * @Param         e 
 *
 * @Returns       成功返回 OK
 * 				  失败返回 ERRNO
 */
/* =======================================*/
Status list_add(pList p,int i,Elemtype e)
{/*{{{*/
	int j;
	if(list_full(p) || i > p->Length || i < 0)
		return ERRNO;
	for(j = p->Length;j >i;j--)
	{
		p->date[j] = p->date[j-1];
	}
	p->date[i] = e;
	p->Length++;

	return OK;
}/*}}}*/
Status list_change(pList p,Elemtype e,Elemtype a)
{/*{{{*/
	int i = 0;
	for(i; i < p->Length; i++)
	{
		if(e == p->date[i])
			p->date[i] = a;
	}
	return OK;
}/*}}}*/
/* =======================================*/
/**
 * @Abstract   		查找第一个与e相同的元素在数组中的位置(1开始计数)
 *
 * @Param         p 结构体的地址
 * @Param         e 需要查找的数据
 *
 * @Returns       如有相同的数据返回位置
 * 				  如果没有查找到就返回 0
 */
/* =======================================*/
int list_find(pList p,Elemtype e)
{/*{{{*/
	int i = 0;
	for(i; i < p->Length; i++)
	{
		if(e == p->date[i])
			return i+1;
	}
	return 0;

}/*}}}*//*}}}*/
int main(int argc, const char *argv[])
{
	Elemtype e;
	/********************************/
	pList p;
	if(0 == initlist(&p))
		return -1;
	printf("%p,%d\n",p,p->Length);
	list_free(p);
	p->Length = 100;
	printf("%p,%d\n",p,p->Length);

#if 1
	pList q;
	if(0 == initlist(&q))
		return -1;
	printf("%p,%d\n",q,p->Length);
	list_free(q);
	list_add_end(q,4);
	list_add_end(q,52);
	list_add_end(q,6);
	list_add_end(q,41);
	list_add_end(q,52);
	list_add_end(q,63);
	list_show(q);
	list_del(q,6,&e);
	list_show(q);
	printf("%d\n",e);
	list_del(q,1,&e);
	list_show(q);
	printf("%d\n",e);
	if(list_add(q,4,52))
		list_show(q);
	list_add(q,1,58);
	list_show(q);
	if(list_del(q,0,&e))
		list_show(q);
	list_change(q,52,100);
	list_show(q);
	printf("%d\n",list_find(q,41));
#endif
	return 0;
}

