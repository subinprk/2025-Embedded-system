	.file	"main.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__CCP__ = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	clock_init
	.type	clock_init, @function
clock_init:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(-40)
	out __CCP__,r24
	sts 97,__zero_reg__
.L2:
	lds r24,99
	sbrc r24,0
	rjmp .L2
/* epilogue start */
	ret
	.size	clock_init, .-clock_init
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"CNT=%u FLAGS=%02X\r\n"
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
	in r28,__SP_L__
	in r29,__SP_H__
	subi r28,64
	sbc r29,__zero_reg__
	out __SP_L__,r28
	out __SP_H__,r29
/* prologue: function */
/* frame size = 64 */
/* stack size = 64 */
.L__stack_usage = 64
	call clock_init
	ldi r18,lo8(319999)
	ldi r24,hi8(319999)
	ldi r25,hlo8(319999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	call USART2_init
	call timer_init_1khz
	ldi r24,lo8(32)
	sts 1185,r24
	sts 1190,r24
	call TWI0_init
	call motor_init
	call scheduler_init
	ldi r18,lo8(319999)
	ldi r24,hi8(319999)
	ldi r25,hlo8(319999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	call TWI0_reset_bus
	ldi r18,lo8(159999)
	ldi r24,hi8(159999)
	ldi r25,hlo8(159999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	call TWI0_reset_bus
	ldi r18,lo8(159999)
	ldi r24,hi8(159999)
	ldi r25,hlo8(159999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	call TWI0_reset_bus
	ldi r18,lo8(159999)
	ldi r24,hi8(159999)
	ldi r25,hlo8(159999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	call initial_debugging
	movw r16,r28
	subi r16,-1
	sbci r17,-1
.L5:
	call scheduler_service_tasks
	lds r24,dbg_print.1
	lds r25,dbg_print.1+1
	adiw r24,1
	sts dbg_print.1,r24
	sts dbg_print.1+1,r25
	cpi r24,16
	sbci r25,39
	brlo .L5
	sts dbg_print.1,__zero_reg__
	sts dbg_print.1+1,__zero_reg__
	lds r18,2698
	lds r19,2698+1
	lds r24,2694
	push __zero_reg__
	push r24
	push r19
	push r18
	ldi r24,lo8(.LC0)
	ldi r25,hi8(.LC0)
	push r25
	push r24
	push __zero_reg__
	ldi r24,lo8(64)
	push r24
	push r17
	push r16
	call snprintf
	movw r24,r16
	call USART2_sendString
	out __SP_L__,r28
	out __SP_H__,r29
	rjmp .L5
	.size	main, .-main
	.text
.global	__vector_12
	.type	__vector_12, @function
__vector_12:
	__gcc_isr 1
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 0...4 */
.L__stack_usage = 0 + __gcc_isr.n_pushed
	ldi r24,lo8(1)
	sts 2694,r24
	ldi r24,lo8(32)
	sts 1191,r24
/* epilogue start */
	__gcc_isr 2
	reti
	__gcc_isr 0,r24
	.size	__vector_12, .-__vector_12
	.local	dbg_print.1
	.comm	dbg_print.1,2,1
	.ident	"GCC: (GNU) 15.2.0"
.global __do_clear_bss
