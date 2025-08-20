
#include "task.h"



										/*xTaskCreateStatic()����*/
#if( configSUPPORT_STATIC_ALLOCATION == 1 ) //���񴴽�ʹ�ã���̬����
/*
@param:
			pxTaskCode:������ڣ�������ĺ�������
			pcName:�������ƣ��ַ�����ʽ���������
			ulStackDepth:����ջ��С����λΪ��
			pvParameters:�����β�
			puxStackBuffer:����ջ��ʼ��ַ
			pxTaskBuffer:������ƿ�ָ��
*/
TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode, const char * const pcName,\
																const uint32_t ulStackDepth,void * const pvParameters,\
                                StackType_t * const puxStackBuffer,TCB_t * const pxTaskBuffer )
{
    TCB_t *pxNewTCB;
    TaskHandle_t xReturn; /*����һ��������xReturn������������ָ�������TCB��*/

if ( ( pxTaskBuffer != NULL ) && ( puxStackBuffer != NULL ) )
    {
        pxNewTCB = ( TCB_t * ) pxTaskBuffer;
        pxNewTCB->pxStack = ( StackType_t * ) puxStackBuffer;

/* �����µ����� */
        prvInitialiseNewTask( pxTaskCode,        /* ������� */
                            pcName,            /* �������ƣ��ַ�����ʽ */
                            ulStackDepth,      /* ����ջ��С����λΪ�� */
                            pvParameters,      /* �����β� */
														&xReturn,          /* ������ */
                            pxNewTCB);         /* ����ջ��ʼ��ַ */

    }
else
    {
        xReturn = NULL;
    }

/* ������������������񴴽��ɹ�����ʱxReturnӦ��ָ��������ƿ� */
return xReturn;
}

#endif /* configSUPPORT_STATIC_ALLOCATION */

/**************************************************************************************************/
									/* prvInitialiseNewTask()����*/
/*
@param
		pxTaskCode:������ڡ�
		
		pcName:�������ƣ��ַ�����ʽ��
				
		ulStackDepth:����ջ��С����λΪ�֡�
				
		pvParameters:�����βΡ�
				
		pxCreatedTask:��������
				
		pxNewTCB:������ƿ�ָ�롣
*/
static void prvInitialiseNewTask(TaskFunction_t pxTaskCode,const char * const pcName,
																	const uint32_t ulStackDepth,void * const pvParameters,
																	TaskHandle_t * const pxCreatedTask,TCB_t *pxNewTCB )
{
    StackType_t *pxTopOfStack;
    UBaseType_t x;

/* ��ȡջ����ַ */
    pxTopOfStack = pxNewTCB->pxStack + ( ulStackDepth - ( uint32_t ) 1 );
/* ������8�ֽڶ��� */
    pxTopOfStack = ( StackType_t * ) \
( ( ( uint32_t ) pxTopOfStack ) & ( ~( ( uint32_t ) 0x0007 ) ) );

/* ����������ִ洢��TCB�� */
for ( x = ( UBaseType_t ) 0; x < ( UBaseType_t ) configMAX_TASK_NAME_LEN; x++ )
    {
        pxNewTCB->pcTaskName[ x ] = pcName[ x ];

if ( pcName[ x ] == 0x00 )
        {
break;
        }
    }
/* �������ֵĳ��Ȳ��ܳ���configMAX_TASK_NAME_LEN */
    pxNewTCB->pcTaskName[ configMAX_TASK_NAME_LEN - 1 ] = '\0';

/* ��ʼ��TCB�е�xStateListItem�ڵ� */
    vListInitialiseItem( &( pxNewTCB->xStateListItem ) );
/* ����xStateListItem�ڵ��ӵ���� */
    listSET_LIST_ITEM_OWNER( &( pxNewTCB->xStateListItem ), pxNewTCB );


/* ��ʼ������ջ */
    pxNewTCB->pxTopOfStack = pxPortInitialiseStack( pxTopOfStack,
                            pxTaskCode,
                            pvParameters );


/* ��������ָ��������ƿ� */
if ( ( void * ) pxCreatedTask != NULL )
    {
        *pxCreatedTask = ( TaskHandle_t ) pxNewTCB;
    }
}

/*******************************************************************/

											/*pxPortInitialiseStack()����*/
#define portINITIAL_XPSR            ( 0x01000000 )
#define portSTART_ADDRESS_MASK      ( ( StackType_t ) 0xfffffffeUL )

static void prvTaskExitError( void )
{
/* ����ֹͣ������ */
for (;;);
}

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,TaskFunction_t pxCode,void *pvParameters )
{
/* �쳣����ʱ���Զ����ص�CPU�Ĵ��������� */
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_XPSR;
    pxTopOfStack--;
    *pxTopOfStack = ( ( StackType_t ) pxCode ) & portSTART_ADDRESS_MASK;
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) prvTaskExitError;
    pxTopOfStack -= 5;      /* R12, R3, R2 and R1 Ĭ�ϳ�ʼ��Ϊ0 */
    *pxTopOfStack = ( StackType_t ) pvParameters;   

/* �쳣����ʱ���ֶ����ص�CPU�Ĵ��������� */
    pxTopOfStack -= 8;

/* ����ջ��ָ�룬��ʱpxTopOfStackָ�����ջ */
return pxTopOfStack;
}

/************************��������б�******************************/
/* ��������б� */
List_t pxReadyTasksLists[ configMAX_PRIORITIES ];
/************************�����б��ʼ��******************************/
/*�����б��ʼ��*/
void prvInitialiseTaskLists( void )
{
    UBaseType_t uxPriority;

		for ( uxPriority = ( UBaseType_t ) 0U; uxPriority < ( UBaseType_t ) configMAX_PRIORITIES; uxPriority++ )
		{
				vListInitialise( &( pxReadyTasksLists[ uxPriority ] ) );
		}
}
/************************��������뵽�����б�******************************/
/* ��ʼ����������ص��б�������б� */
//prvInitialiseTaskLists();

//Task1_Handle =                                    /* ������ */
//xTaskCreateStatic( (TaskFunction_t)Task1_Entry,   /* ������� */
//    (char *)"Task1",               /* �������ƣ��ַ�����ʽ */
//    (uint32_t)TASK1_STACK_SIZE ,   /* ����ջ��С����λΪ�� */
//    (void *) NULL,                 /* �����β� */
//    (StackType_t *)Task1Stack,     /* ����ջ��ʼ��ַ */
//    (TCB_t *)&Task1TCB );          /* ������ƿ� */

///* ��������ӵ������б� */
//vListInsertEnd( &( pxReadyTasksLists[1] ),
//&( ((TCB_t *)(&Task1TCB))->xStateListItem ) );

//Task2_Handle =                                    /* ������ */
//    xTaskCreateStatic( (TaskFunction_t)Task2_Entry,   /* ������� */
//    (char *)"Task2",               /* �������ƣ��ַ�����ʽ */
//    (uint32_t)TASK2_STACK_SIZE ,   /* ����ջ��С����λΪ�� */
//    (void *) NULL,                 /* �����β� */
//    (StackType_t *)Task2Stack,     /* ����ջ��ʼ��ַ */
//    (TCB_t *)&Task2TCB );          /* ������ƿ� */
///* ��������ӵ������б� */
//vListInsertEnd( &( pxReadyTasksLists[2] ),
//&( ((TCB_t *)(&Task2TCB))->xStateListItem ) );

/************************����������******************************/
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
TCB_t  *pxCurrentTCB; //����ָ��ǰ�������л��߼���Ҫ���� �������������ƿ�
void vTaskStartScheduler( void )
{
/* �ֶ�ָ����һ�����е����� */
    pxCurrentTCB = &Task1TCB;

/* ���������� */
if ( xPortStartScheduler() != pdFALSE )
    {
/* �����������ɹ����򲻻᷵�أ��������������� */(2)
    }
}
/************************vTaskSwitchContext()����******************************/
void vTaskSwitchContext( void )
{
/* �������������л� */
if ( pxCurrentTCB == &Task1TCB )
    {
        pxCurrentTCB = &Task2TCB;
    }
else
    {
        pxCurrentTCB = &Task1TCB;
    }
}