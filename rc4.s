	AREA RC4_Code, CODE, READONLY
	EXPORT rc4_encrypt ;Make function visible to main.c
	
rc4_encrypt
	;R0 Contains the address of s_array
	;R1 containd the address of key
	;R2 contains the address of plaintext
	;R3 contains the address of ciphertext
	PUSH {R4-R11, LR}
	
	;calculate the Length of key string
	MOV R6, #0
	MOV R12, R1
	
KeyLen_Loop
	LDRB R8, [R12], #1
	CMP R8, #0
	ADDNE R6, R6, #1
	BNE KeyLen_Loop
	
	; Init S_array
	MOV R4, #0
	
Init_Loop
	STRB R4, [R0, R4]
	ADD R4, R4, #1
	CMP R4, #256
	BNE Init_Loop
	
	; KSA Scrambling
	MOV R4, #0
	MOV R5, #0
	MOV R7, #0
	
KSA_Loop
	LDRB R8, [R0, R4]
	LDRB R9, [R1, R7]
	
	; j = (j + S[i] + key[key_index]) mod 256
	ADD R5, R5, R8
	ADD R5, R5, R9
	AND R5, R5, #0xFF
	
	; Swap S[i] and S[j]
	LDRB R10, [R0, R5]
	STRB R10, [R0, R4]
	STRB R8, [R0, R5]
	
	;Update Indices
	ADD R4, R4, #1
	ADD R7, R7, #1
	
	CMP R7, R6
	MOVEQ R7, #0
	
	CMP R4, #256
	BNE KSA_Loop
	
	;PRGA Encryption
	MOV R4, #0
	MOV R5, #0

PRGA_Loop
	LDRB R11, [R2], #1
	CMP R11, #0
	BEQ End_Encrypt
	
	;i = (i+1) mod 256
	ADD R9, R5, R8
	AND R5, R5, #0xFF
	LDRB R9, [R0, R5]
	
	;Swap S[i] and S[j]
	STRB R9, [R0, R4]
	STRB R8, [R0, R5]
	
	;Keystream Byte = S[(S[i] + S[j]) mod 256]
	ADD R10, R8, R9
	AND R10, R10, #0xFF
	LDRB R10, [R0, R10]
	
	;The Encryption
	EOR R11, R11, R10
	STRB R11, [R3], #1
	B PRGA_Loop

End_Encrypt
	;CLean up and return to C
	MOV R11, #0
	STRB R11, [R3]
	
	;Restore the original registers we borrowed
	POP {R4-R11, LR}
	
	;Return execution to the main() function in C
	BX LR
	END
	