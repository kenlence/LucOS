/*
 * Register switch for ARMv7-M processors.
 * r0 = previous task_struct, r1 = previous thread_info, r2 = next thread_info
 * previous and next are guaranteed not to be the same.
 */
ENTRY(__switch_to)
	.fnstart
	.cantunwind
	add	ip, r1, #TI_CPU_SAVE
	stmia	ip!, {r4 - r11}		@ Store most regs on stack
	str	sp, [ip], #4
	str	lr, [ip], #4
	mov	r5, r0
	add	r4, r2, #TI_CPU_SAVE
	ldr	r0, =thread_notify_head
	mov	r1, #THREAD_NOTIFY_SWITCH
	bl	atomic_notifier_call_chain
	mov	ip, r4
	mov	r0, r5
	ldmia	ip!, {r4 - r11}		@ Load all regs saved previously
	ldr	sp, [ip]
	ldr	pc, [ip, #4]!
	.fnend
ENDPROC(__switch_to)