/*cpu_context在thread_info中的偏移是4，如果thread_info结构体，也要同步改这里 */
#define TI_CPU_SAVE 4

#include "linkage.h"

ENTRY(__switch_to)
 	.fnstart
	//声明无法回溯调用链
 	.cantunwind

	/* ip(R12), 内部调用暂存寄存器 */
	/* ip是prev的thread_info.cpu_context*/
	add	ip, r1, #TI_CPU_SAVE
	/*保存r4-r11，r0-r3属于函数调用的寄存器组，默认传参用，不需要保存 */
	/*R12开始的寄存器为特殊寄存，单独处理
	 * r12:ip 内部调用寄存器
	 * r13:sp 栈指针
	 * r14:lr 函数调用返回寄存器！
	 * r15:pc 指令寄存器
	 */
	 /*stmia指令执行过程中 ip也会自加，所以是按地址一个个往下写 */
 	stmia ip!, {r4 - sl, fp, sp, lr} @ Store most regs on stack

	/* 至此，prev的寄存器已经保存完毕 */

	/* r5指向prev的task*/
	/* r4指向next的thread_info.cpu_context */
	mov	r5, r0
	add	r4, r2, #TI_CPU_SAVE
	/*为什么不用r0-r3，因为有个notify调用(这里删掉了)，需要r0-r3 */

	/*又把上面r4,r5的值放回r0和ip */
	/*r0，prev task_struct */
	/*ip, next thread_info.cpu_context */
 	mov	ip, r4
	mov	r0, r5

	/*把新进程的CPU恢复出来 */
 	ldmia	r4, {r4 - sl, fp, sp, pc} 	@ Load all regs saved previously
 	.fnend
ENDPROC(__switch_to)