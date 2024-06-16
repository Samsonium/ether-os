[org 0x7C00] ; bootloader offset
KERNEL_OFFSET equ 0x1000 ; set on linking in -Ttext arg

	mov [BOOT_DRIVE], dl
	mov bp, 0x9000
	mov sp, bp

	mov bx, MSG_REAL_MODE
	call print
	call print_nl

	call load_kernel	; read kernel from the disk
	call switch_to_pm	; go to protected 32bit mode

	jmp $				; fool check

%include "./src/boot/print.s"
%include "./src/boot/print_hex.s"
%include "./src/boot/disk.s"
%include "./src/boot/gdt.s"
%include "./src/boot/print_pm.s"
%include "./src/boot/32bit-switch.s"

[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print
	call print_nl

	mov bx, KERNEL_OFFSET
	mov dh, 16
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

[bits 32]
BEGIN_PM:	
	call KERNEL_OFFSET
	jmp $

BOOT_DRIVE		db 0
MSG_REAL_MODE 	db "Started in 16bit real mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory...", 0

times 510-($-$$) db 0
dw 0xAA55
