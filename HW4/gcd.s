	.text
	.syntax unified
	.thumb
	.global	gcd	
	.type gcd, %function
gcd:
	push {r4-r7}
	cmp r0, r1
	beq end
	blt lthan
	subs r0, r0, r1
	b gcd
lthan:
	subs r1, r1, r0
	b gcd
end:
	pop {r4-r7}	
	bx lr
