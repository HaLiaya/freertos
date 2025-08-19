#ifndef __LIST_H__
#define __LIST_H__
#include "portmacro.h"

/*一般节点即中间节点定义*/
struct xLIST_ITEM{
	TickType_t xItemValue;							/* 辅助值，用于帮助节点做顺序排列 */
	struct xLIST_ITEM * pxNext;					/* 指向链表下一个节点 */
	struct xLIST_ITEM * pxPrevious;			/* 指向链表前一个节点 */
	void * pvOwner; 										/* 指向拥有该节点的内核对象，通常是TCB */
	void *pvContainer;									/* 指向该节点所在的链表 */
};
typedef struct xLIST_ITEM ListItem_t; /* 节点数据类型重定义 */

/*生产者节点即头尾节点定义*/
struct xMINI_LIST_ITEM
{
    TickType_t xItemValue;                      /* 辅助值，用于帮助节点做升序排列 */
    struct xLIST_ITEM *  pxNext;                /* 指向链表下一个节点 */
    struct xLIST_ITEM *  pxPrevious;            /* 指向链表前一个节点 */
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;  /* 精简节点数据类型重定义 */

/*链表根节点节点定义*/
typedef struct xLIST
{
	UBaseType_t uxNumberOfItems;						/* 链表节点计数器 */
	ListItem_t *  pxIndex;          				/* 链表节点索引指针 */
	MiniListItem_t xListEnd;                /* 链表最后一个节点 */
} List_t;

#endif