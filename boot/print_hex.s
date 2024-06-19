print_hex:
	pusha
	mov cx, 0	; index variable

; Strategy: get the last character of 'dx' and convert it to ASCII
; Numeric ASCII: 0 (0x30) to 9 (0x39), so just add 0x30 to byte N
; Abcs ASCII: A (0x41) to F (0x46) we'll add 0x40
; Then move ASCII byte to the correct position on the result string
hex_loop:
	cmp cx, 4	; loop 4 times
	je end

	; convert last char of 'dx' to ASCII
	mov ax, dx
	and ax, 0x000F 	; masking first three to zeros
	add al, 0x30	; adding 0x30 to N to convert it to ASCII 'N'
	cmp al, 0x39	; if N > 9, add extra 8 to represent A to F
	jle hex_convert
	add al, 7

; Step 2 - get the correct position of the string to place ASCII
; character. bx <- base_addr + str_len - char_index
hex_convert:
	mov bx, HEX_OUT + 5	; base + length
	sub bx, cx
	mov [bx], al		; copy ASCII char to the position on 'bx'
	ror dx, 4			; 0x1234 -> 0x4123 -> 0x3412 -> 0x2341 -> 0x1234

	; inc index and loop
	inc cx
	jmp hex_loop

end:
	mov bx, HEX_OUT
	call print

	popa
	ret

HEX_OUT:
	db '0x0000', 0 ; reserve memory for string
