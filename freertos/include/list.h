#ifndef __LIST_H__
#define __LIST_H__
#include "portmacro.h"

/*һ��ڵ㼴�м�ڵ㶨��*/
struct xLIST_ITEM{
	TickType_t xItemValue;							/* ����ֵ�����ڰ����ڵ���˳������ */
	struct xLIST_ITEM * pxNext;					/* ָ��������һ���ڵ� */
	struct xLIST_ITEM * pxPrevious;			/* ָ������ǰһ���ڵ� */
	void * pvOwner; 										/* ָ��ӵ�иýڵ���ں˶���ͨ����TCB */
	void *pvContainer;									/* ָ��ýڵ����ڵ����� */
};
typedef struct xLIST_ITEM ListItem_t; /* �ڵ����������ض��� */

/*�����߽ڵ㼴ͷβ�ڵ㶨��*/
struct xMINI_LIST_ITEM
{
    TickType_t xItemValue;                      /* ����ֵ�����ڰ����ڵ����������� */
    struct xLIST_ITEM *  pxNext;                /* ָ��������һ���ڵ� */
    struct xLIST_ITEM *  pxPrevious;            /* ָ������ǰһ���ڵ� */
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;  /* ����ڵ����������ض��� */

/*������ڵ�ڵ㶨��*/
typedef struct xLIST
{
	UBaseType_t uxNumberOfItems;						/* ����ڵ������ */
	ListItem_t *  pxIndex;          				/* ����ڵ�����ָ�� */
	MiniListItem_t xListEnd;                /* �������һ���ڵ� */
} List_t;

#endif