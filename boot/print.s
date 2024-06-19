print:
	pusha

start:
	mov al, [bx] 	; 'bx' - base address of the string
	cmp al, 0 		; check for end of the string
	je done

	; tty
	mov ah, 0x0E
	int 10h

	; inc character index
	inc bx
	jmp start

done:
	popa
	ret

print_nl:
	pusha

	mov ah, 0x0E
	mov al, 0x0A ; '\n'
	int 10h

	mov al, 0x0D ; '\r'
	int 10h

	popa
	ret
