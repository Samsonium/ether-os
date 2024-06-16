
; load 'dh' sectors from drive 'dl' into ES:BX
disk_load:
	pusha
	push dx

	mov ah, 0x02	; read function
	mov al, dh		; number of sectors to read (0x01 ... 0x80)
	mov cl, 0x02	; sector (0x01 ... 0x11)

	mov ch, 0x00	; cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')
	mov dh, 0x00	; head number (0x0 ... 0xF)

	; [es:bx] <- pointer to buffer where the data will be stored
	int 13h
	jc disk_error

	pop dx
	cmp al, dh	; BIOS also sets 'al' to the # of sectors read
	jne sectors_error
	popa
	ret

disk_error:
	mov bx, DISK_ERROR
	call print
	call print_nl

	mov dh, ah		; ah = error code, dl = disk drive dropped the error
	call print_hex
	jmp disk_loop

sectors_error:
	mov bx, SECTORS_ERROR
	call print

disk_loop:
	jmp $

DISK_ERROR:
	db "Disk read error", 0

SECTORS_ERROR:
	db "Incorrect number of sectors read", 0
	
