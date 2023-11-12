/*
 * Register switch for ARMv7-M processors.
 * r0 = previous task_struct, r1 = previous thread_info, r2 = next thread_info
 * previous and next are guaranteed not to be the same.
 */
//ENTRY(__switch_to)
.global __switch_to
__switch_to:
	.fnstart
	//声明无法回溯调用链
	.cantunwind

	/* ip(R12), 内部调用暂存寄存器 */
	/* ip是prev的thread_info.cpu_context*/
	/*cpu_context在thread_info中的偏移是4，如果thread_info结构体，也要同步改这里 */
	add	ip, r1, #4
	/*保存r4-r11，r0-r3属于函数调用的寄存器组，默认传参用，不需要保存 */
	/*R12开始的寄存器为特殊寄存，单独处理
	 * r12:ip 内部调用寄存器
	 * r13:sp 栈指针
	 * r14:lr 函数调用返回寄存器！
	 * r15:pc 指令寄存器
	 */
	 /*stmia指令执行过程中 ip也会自加，所以是按地址一个个往下写 */
	stmia	ip!, {r4 - r11}		@ Store most regs on stack
	/*截止现在就剩sp 和 pc没保存了，下面就保存sp和pc*/
	/* 指令含义：把sp写到ip的地址中，然后ip = ip + 4 */
	str	sp, [ip], #4
	/*这里保存pc实际保存的是lr，lr是函数调用返回寄存器，指向的是__switch_to下面的barrier() */
	str	lr, [ip], #4

	/* 至此，prev的寄存器已经保存完毕 */

	/* r5指向prev的task*/
	mov	r5, r0
	/* r4指向next的thread_info.cpu_context */
	add	r4, r2, #4
	/*为什么不用r0-r3，因为下面有个notify调用，需要r0-r3 */

	//线程切换的回调，这里不加这个功能
	//ldr	r0, =thread_notify_head
	//mov	r1, #THREAD_NOTIFY_SWITCH
	//bl	atomic_notifier_call_chain

	/*又把上面r4,r5的值放回r0和ip */
	/*r0，prev task_struct */
	/*ip, next thread_info.cpu_context */
	mov	ip, r4
	mov	r0, r5
	/*把新进程的CPU恢复出来 */
	ldmia	ip!, {r4 - r11}		@ Load all regs saved previously
	ldr	sp, [ip]
	ldr	pc, [ip, #4]!
	.fnend
//ENDPROC(__switch_to)