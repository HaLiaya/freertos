
#include "task.h"



										/*xTaskCreateStatic()函数*/
#if( configSUPPORT_STATIC_ALLOCATION == 1 ) //任务创建使用：静态创建
/*
@param:
			pxTaskCode:任务入口，即任务的函数名称
			pcName:任务名称，字符串形式，方便调试
			ulStackDepth:任务栈大小，单位为字
			pvParameters:任务形参
			puxStackBuffer:任务栈起始地址
			pxTaskBuffer:任务控制块指针
*/
TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode, const char * const pcName,\
																const uint32_t ulStackDepth,void * const pvParameters,\
                                StackType_t * const puxStackBuffer,TCB_t * const pxTaskBuffer )
{
    TCB_t *pxNewTCB;
    TaskHandle_t xReturn; /*定义一个任务句柄xReturn，任务句柄用于指向任务的TCB。*/

if ( ( pxTaskBuffer != NULL ) && ( puxStackBuffer != NULL ) )
    {
        pxNewTCB = ( TCB_t * ) pxTaskBuffer;
        pxNewTCB->pxStack = ( StackType_t * ) puxStackBuffer;

/* 创建新的任务 */
        prvInitialiseNewTask( pxTaskCode,        /* 任务入口 */
                            pcName,            /* 任务名称，字符串形式 */
                            ulStackDepth,      /* 任务栈大小，单位为字 */
                            pvParameters,      /* 任务形参 */
														&xReturn,          /* 任务句柄 */
                            pxNewTCB);         /* 任务栈起始地址 */

    }
else
    {
        xReturn = NULL;
    }

/* 返回任务句柄，如果任务创建成功，此时xReturn应该指向任务控制块 */
return xReturn;
}

#endif /* configSUPPORT_STATIC_ALLOCATION */

/**************************************************************************************************/
									/* prvInitialiseNewTask()函数*/
/*
@param
		pxTaskCode:任务入口。
		
		pcName:任务名称，字符串形式。
				
		ulStackDepth:任务栈大小，单位为字。
				
		pvParameters:任务形参。
				
		pxCreatedTask:任务句柄。
				
		pxNewTCB:任务控制块指针。
*/
static void prvInitialiseNewTask(TaskFunction_t pxTaskCode,const char * const pcName,
																	const uint32_t ulStackDepth,void * const pvParameters,
																	TaskHandle_t * const pxCreatedTask,TCB_t *pxNewTCB )
{
    StackType_t *pxTopOfStack;
    UBaseType_t x;

/* 获取栈顶地址 */
    pxTopOfStack = pxNewTCB->pxStack + ( ulStackDepth - ( uint32_t ) 1 );
/* 向下做8字节对齐 */
    pxTopOfStack = ( StackType_t * ) \
( ( ( uint32_t ) pxTopOfStack ) & ( ~( ( uint32_t ) 0x0007 ) ) );

/* 将任务的名字存储在TCB中 */
for ( x = ( UBaseType_t ) 0; x < ( UBaseType_t ) configMAX_TASK_NAME_LEN; x++ )
    {
        pxNewTCB->pcTaskName[ x ] = pcName[ x ];

if ( pcName[ x ] == 0x00 )
        {
break;
        }
    }
/* 任务名字的长度不能超过configMAX_TASK_NAME_LEN */
    pxNewTCB->pcTaskName[ configMAX_TASK_NAME_LEN - 1 ] = '\0';

/* 初始化TCB中的xStateListItem节点 */
    vListInitialiseItem( &( pxNewTCB->xStateListItem ) );
/* 设置xStateListItem节点的拥有者 */
    listSET_LIST_ITEM_OWNER( &( pxNewTCB->xStateListItem ), pxNewTCB );


/* 初始化任务栈 */
    pxNewTCB->pxTopOfStack = pxPortInitialiseStack( pxTopOfStack,
                            pxTaskCode,
                            pvParameters );


/* 让任务句柄指向任务控制块 */
if ( ( void * ) pxCreatedTask != NULL )
    {
        *pxCreatedTask = ( TaskHandle_t ) pxNewTCB;
    }
}

/*******************************************************************/

											/*pxPortInitialiseStack()函数*/
#define portINITIAL_XPSR            ( 0x01000000 )
#define portSTART_ADDRESS_MASK      ( ( StackType_t ) 0xfffffffeUL )

static void prvTaskExitError( void )
{
/* 函数停止在这里 */
for (;;);
}

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,TaskFunction_t pxCode,void *pvParameters )
{
/* 异常发生时，自动加载到CPU寄存器的内容 */
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_XPSR;
    pxTopOfStack--;
    *pxTopOfStack = ( ( StackType_t ) pxCode ) & portSTART_ADDRESS_MASK;
    pxTopOfStack--;
    *pxTopOfStack = ( StackType_t ) prvTaskExitError;
    pxTopOfStack -= 5;      /* R12, R3, R2 and R1 默认初始化为0 */
    *pxTopOfStack = ( StackType_t ) pvParameters;   

/* 异常发生时，手动加载到CPU寄存器的内容 */
    pxTopOfStack -= 8;

/* 返回栈顶指针，此时pxTopOfStack指向空闲栈 */
return pxTopOfStack;
}

/************************定义就绪列表******************************/
/* 任务就绪列表 */
List_t pxReadyTasksLists[ configMAX_PRIORITIES ];
/************************就绪列表初始化******************************/
/*就绪列表初始化*/
void prvInitialiseTaskLists( void )
{
    UBaseType_t uxPriority;

		for ( uxPriority = ( UBaseType_t ) 0U; uxPriority < ( UBaseType_t ) configMAX_PRIORITIES; uxPriority++ )
		{
				vListInitialise( &( pxReadyTasksLists[ uxPriority ] ) );
		}
}
/************************将任务插入到就绪列表******************************/
/* 初始化与任务相关的列表，如就绪列表 */
//prvInitialiseTaskLists();

//Task1_Handle =                                    /* 任务句柄 */
//xTaskCreateStatic( (TaskFunction_t)Task1_Entry,   /* 任务入口 */
//    (char *)"Task1",               /* 任务名称，字符串形式 */
//    (uint32_t)TASK1_STACK_SIZE ,   /* 任务栈大小，单位为字 */
//    (void *) NULL,                 /* 任务形参 */
//    (StackType_t *)Task1Stack,     /* 任务栈起始地址 */
//    (TCB_t *)&Task1TCB );          /* 任务控制块 */

///* 将任务添加到就绪列表 */
//vListInsertEnd( &( pxReadyTasksLists[1] ),
//&( ((TCB_t *)(&Task1TCB))->xStateListItem ) );

//Task2_Handle =                                    /* 任务句柄 */
//    xTaskCreateStatic( (TaskFunction_t)Task2_Entry,   /* 任务入口 */
//    (char *)"Task2",               /* 任务名称，字符串形式 */
//    (uint32_t)TASK2_STACK_SIZE ,   /* 任务栈大小，单位为字 */
//    (void *) NULL,                 /* 任务形参 */
//    (StackType_t *)Task2Stack,     /* 任务栈起始地址 */
//    (TCB_t *)&Task2TCB );          /* 任务控制块 */
///* 将任务添加到就绪列表 */
//vListInsertEnd( &( pxReadyTasksLists[2] ),
//&( ((TCB_t *)(&Task2TCB))->xStateListItem ) );

/************************启动调度器******************************/
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
TCB_t  *pxCurrentTCB; //用于指向当前正在运行或者即将要运行 的任务的任务控制块
void vTaskStartScheduler( void )
{
/* 手动指定第一个运行的任务 */
    pxCurrentTCB = &Task1TCB;

/* 启动调度器 */
if ( xPortStartScheduler() != pdFALSE )
    {
/* 调度器启动成功，则不会返回，即不会来到这里 */(2)
    }
}
/************************vTaskSwitchContext()函数******************************/
void vTaskSwitchContext( void )
{
/* 两个任务轮流切换 */
if ( pxCurrentTCB == &Task1TCB )
    {
        pxCurrentTCB = &Task2TCB;
    }
else
    {
        pxCurrentTCB = &Task1TCB;
    }
}