main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	mov	r3, #0
	str	r3, [fp, #-8]
	b	.L14
.L15:
	bl	stop_light
	ldr	r0, [fp, #-8]
	bl	inc
	str	r0, [fp, #-8]
.L14:
	ldr	r3, [fp, #-8]
	cmp	r3, #35
	ble	.L15
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, lr}
	bx	lr
	.size	main, .-main
	.bss
	.align	2
