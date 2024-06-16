[bits 32]

VIDEO_MEMORY equ 0xB8000
WHITE_BLACK  equ 0x0F

print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY

print_string_pm_loop:
	mov al, [ebx] ; [ebx] - character address
	mov ah, WHITE_BLACK

	cmp al, 0 ; check for end of string
	je print_string_pm_done

	mov [edx], ax	; store character and attribute vmem
	add ebx, 1 		; move to the next character
	add edx, 2 		; move to the next vmem position

	jmp print_string_pm_loop

print_string_pm_done:
	popa
	ret
