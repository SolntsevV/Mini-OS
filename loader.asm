bits 16
org 0x7C00
start:
	mov ah,0
	int 0x13
	push 0x8000
	mov bp,sp
	mov cl,0x2
	mov ch,0x0
	mov dl,0x80
	mov bx,[esp]
	mov ah,0x2
	mov al,0x80
	int 0x13
	jmp [esp]

	end:
	pop bp
	ret

times 510-($-$$) db 0
dw 0xAA55