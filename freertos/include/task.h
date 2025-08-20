#ifndef _MY__TASK__H_
#define _MY__TASK__H_
#include "list.h"
#include "projdefs.h"
#include "FreeRTOSConfig.h"

#define taskYIELD()                 portYIELD()

/*任务控制块类型声明*/
typedef struct tskTaskControlBlock
{
volatile StackType_t    *pxTopOfStack;    /* 栈顶 */

    ListItem_t              xStateListItem;   /* 任务节点 */

    StackType_t             *pxStack;         /* 任务栈起始地址 */
/* 任务名称，字符串形式 */
char                    pcTaskName[ configMAX_TASK_NAME_LEN ];
} tskTCB;
typedef tskTCB TCB_t;

/* 任务句柄 */
typedef void * TaskHandle_t;


static void prvInitialiseNewTask(TaskFunction_t pxTaskCode, const char * const pcName, const uint32_t ulStackDepth,\
																	void * const pvParameters,TaskHandle_t * const pxCreatedTask, TCB_t *pxNewTCB ) ;
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,TaskFunction_t pxCode,void *pvParameters );
void vTaskSwitchContext( void );
#endif