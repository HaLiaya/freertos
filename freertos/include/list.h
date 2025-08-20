#ifndef __LIST_H__
#define __LIST_H__
#include "portmacro.h"
/*
	��FreeRTOS�е�������һ��˫��ѭ������
*/

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

/*�ڵ���κ�С����*/

/* ��ʼ���ڵ��ӵ���� */
#define listSET_LIST_ITEM_OWNER( pxListItem, pxOwner )\
        ( ( pxListItem )->pvOwner = ( void * ) ( pxOwner ) )
/* ��ȡ�ڵ�ӵ���� */
#define listGET_LIST_ITEM_OWNER( pxListItem )\
        ( ( pxListItem )->pvOwner )
/* ��ʼ���ڵ�������ֵ */
#define listSET_LIST_ITEM_VALUE( pxListItem, xValue )\
        ( ( pxListItem )->xItemValue = ( xValue ) )
/* ��ȡ�ڵ�������ֵ */
#define listGET_LIST_ITEM_VALUE( pxListItem )\
        ( ( pxListItem )->xItemValue )
/* ��ȡ������ڵ�(ͷ���ڵ�)�Ľڵ��������ֵ */
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxList )\
        ( ( ( pxList )->xListEnd ).pxNext->xItemValue )
/* ��ȡ�������ڽڵ� */
#define listGET_HEAD_ENTRY( pxList )\
        ( ( ( pxList )->xListEnd ).pxNext )

/* ��ȡ�ڵ����һ���ڵ� */
#define listGET_NEXT( pxListItem )\
        ( ( pxListItem )->pxNext )

/* ��ȡ��������һ���ڵ�(β��ǽڵ�) */
#define listGET_END_MARKER( pxList )\
        ( ( ListItem_t const * ) ( &( ( pxList )->xListEnd ) ) )
/* �ж������Ƿ�Ϊ�� */
#define listLIST_IS_EMPTY( pxList )\
        ( ( BaseType_t ) ( ( pxList )->uxNumberOfItems == ( UBaseType_t ))

/* ��ȡ����Ľڵ��� */
#define listCURRENT_LIST_LENGTH( pxList )\
        ( ( pxList )->uxNumberOfItems )
				
/* ��ȡ�����һ���ڵ��OWNER����TCB */ 
/*
	�ڵ��� listGET_OWNER_OF_NEXT_ENTRY ��֮ǰ����Ӧ���ȼ�������Ƿ�Ϊ�ա�����( pxConstList )->pxIndexʼ��ָ��xListEndβ��ǽڵ㣬��β��ڵ㲢û�� pvOwner ��Ա���ᵼ��δ������Ϊ��
*/
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )\
{																										\
    List_t * const pxConstList = ( pxList );				\
    /* �ڵ�����ָ�������һ���ڵ� */\
    ( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;\
    /* ���������ɶ�ã� */\
    if( ( void * ) ( pxConstList )->pxIndex == ( void * ) &( ( pxConstList )->xListEnd ) )\
    {\
        ( pxConstList )->pxIndex = ( pxConstList )->pxIndex->pxNext;\
    }\
    /* ��ȡ�ڵ��OWNER����TCB */\
    ( pxTCB ) = ( pxConstList )->pxIndex->pvOwner;\
}
#endif