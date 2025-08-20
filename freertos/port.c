
#include <stdint.h>
#include "portmacro.h"
#include "task.h"
/*
* 参考资料《STM32F10xxx Cortex-M3 programming manual》4.4.3，百度搜索“PM0056”即可找到这个文档
* 在Cortex-M中，内核外设SCB的地址范围为：0xE000ED00-0xE000ED3F
* 0xE000ED008为SCB外设中SCB_VTOR这个寄存器的地址，里面存放的是向量表的起始地址，即MSP的地址
*/
__asm void prvStartFirstTask( void )
{
    PRESERVE8

/* 在Cortex-M中，0xE000ED08是SCB_VTOR这个寄存器的地址，
里面存放的是向量表的起始地址，即MSP的地址 */
    ldr r0, =0xE000ED08
ldr r0, [r0]
ldr r0, [r0]

/* 设置主栈指针msp的值 */
msr msp, r0(7)

/* 使能全局中断 */
cpsie i
cpsie f
    dsb
    isb

/* 调用SVC去启动第一个任务 */
    svc 0(9)
    nop
    nop
}
/*
*参考资料《STM32F10xxx Cortex-M3 programming manual》4.4.3，百度搜索“PM0056”即可找到这个文档
* 在Cortex-M中，内核外设SCB中SHPR3寄存器用于设置SysTick和PendSV的异常优先级
* System handler priority register 3 (SCB_SHPR3) SCB_SHPR3：0xE000 ED20
* Bits 31:24 PRI_15[7:0]: Priority of system handler 15, SysTick exception
* Bits 23:16 PRI_14[7:0]: Priority of system handler 14, PendSV
*/
#define portNVIC_SYSPRI2_REG         (*(( volatile uint32_t *) 0xe000ed20))

#define portNVIC_PENDSV_PRI(((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 16UL)
#define portNVIC_SYSTICK_PRI(((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 24UL )

BaseType_t xPortStartScheduler( void )
{
/* 配置PendSV 和 SysTick 的中断优先级为最低 */
    portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
    portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;

/* 启动第一个任务，不再返回 */
    prvStartFirstTask();

/* 不应该运行到这里 */
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