
#include <stdint.h>
#include "portmacro.h"
#include "task.h"
/*
* �ο����ϡ�STM32F10xxx Cortex-M3 programming manual��4.4.3���ٶ�������PM0056�������ҵ�����ĵ�
* ��Cortex-M�У��ں�����SCB�ĵ�ַ��ΧΪ��0xE000ED00-0xE000ED3F
* 0xE000ED008ΪSCB������SCB_VTOR����Ĵ����ĵ�ַ�������ŵ������������ʼ��ַ����MSP�ĵ�ַ
*/
__asm void prvStartFirstTask( void )
{
    PRESERVE8

/* ��Cortex-M�У�0xE000ED08��SCB_VTOR����Ĵ����ĵ�ַ��
�����ŵ������������ʼ��ַ����MSP�ĵ�ַ */
    ldr r0, =0xE000ED08
ldr r0, [r0]
ldr r0, [r0]

/* ������ջָ��msp��ֵ */
msr msp, r0(7)

/* ʹ��ȫ���ж� */
cpsie i
cpsie f
    dsb
    isb

/* ����SVCȥ������һ������ */
    svc 0(9)
    nop
    nop
}
/*
*�ο����ϡ�STM32F10xxx Cortex-M3 programming manual��4.4.3���ٶ�������PM0056�������ҵ�����ĵ�
* ��Cortex-M�У��ں�����SCB��SHPR3�Ĵ�����������SysTick��PendSV���쳣���ȼ�
* System handler priority register 3 (SCB_SHPR3) SCB_SHPR3��0xE000 ED20
* Bits 31:24 PRI_15[7:0]: Priority of system handler 15, SysTick exception
* Bits 23:16 PRI_14[7:0]: Priority of system handler 14, PendSV
*/
#define portNVIC_SYSPRI2_REG         (*(( volatile uint32_t *) 0xe000ed20))

#define portNVIC_PENDSV_PRI(((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 16UL)
#define portNVIC_SYSTICK_PRI(((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 24UL )

BaseType_t xPortStartScheduler( void )
{
/* ����PendSV �� SysTick ���ж����ȼ�Ϊ��� */
    portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
    portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;

/* ������һ�����񣬲��ٷ��� */
    prvStartFirstTask();

/* ��Ӧ�����е����� */
return 0;
}
__asm void vPortSVCHandler( void )
{
extern pxCurrentTCB;

    PRESERVE8

    ldrr3, =pxCurrentTCB
		ldr r1, [r3]
		ldr r0, [r1]
		ldmia r0!, {r4-r11}
		msr psp, r0
		isb
		mov r0, #0
		msrbasepri, r0
				orr r14, #0xd
		
		bx r14
}
__asm void xPortPendSVHandler( void )
{
extern pxCurrentTCB;
extern vTaskSwitchContext;

    PRESERVE8

    mrs r0, psp
    isb

    ldrr3, =pxCurrentTCB
ldrr2, [r3]

stmdb r0!, {r4-r11}
str r0, [r2]

stmdb sp!, {r3, r14}
    mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY
    msr basepri, r0
    dsb
    isb
    bl vTaskSwitchContext
    mov r0, #0
    msr basepri, r0
    ldmia sp!, {r3, r14}

    ldr r1, [r3]
    ldr r0, [r1]
    ldmia r0!, {r4-r11}
    msr psp, r0
    isb
    bx r14
    nop
}