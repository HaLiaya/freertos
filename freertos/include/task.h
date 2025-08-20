#ifndef _MY__TASK__H_
#define _MY__TASK__H_
#include "list.h"
#include "projdefs.h"
#include "FreeRTOSConfig.h"

#define taskYIELD()                 portYIELD()

/*������ƿ���������*/
typedef struct tskTaskControlBlock
{
volatile StackType_t    *pxTopOfStack;    /* ջ�� */

    ListItem_t              xStateListItem;   /* ����ڵ� */

    StackType_t             *pxStack;         /* ����ջ��ʼ��ַ */
/* �������ƣ��ַ�����ʽ */
char                    pcTaskName[ configMAX_TASK_NAME_LEN ];
} tskTCB;
typedef tskTCB TCB_t;

/* ������ */
typedef void * TaskHandle_t;


static void prvInitialiseNewTask(TaskFunction_t pxTaskCode, const char * const pcName, const uint32_t ulStackDepth,\
																	void * const pvParameters,TaskHandle_t * const pxCreatedTask, TCB_t *pxNewTCB ) ;
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,TaskFunction_t pxCode,void *pvParameters );
void vTaskSwitchContext( void );
#endif