.text

.global host2netl
host2netl:

	pushl %ebp /*Push ebp onto stack*/
	movl %esp, %ebp /*Copy stack pointer to ebp*/
	subl $4, %esp /*make space for data*/

	movb 8(%ebp), %al /* swap the bytes around, 0 ->3, 1->2, 2->1, 3->0*/
	movb %al, 3(%esp)
	movb 9(%ebp), %al
	movb %al, 2(%esp)
	movb 10(%ebp), %al
	movb %al, 1(%esp)
	movb 11(%ebp), %al
	movb %al, (%esp)

	pop %eax /* set return value*/
	pop %ebp 

	ret

