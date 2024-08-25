section .data
    SYS_READ    dd 3
    SYS_WRITE   dd 4
    SYS_OPEN    dd 5
    SYS_CLOSE   dd 6
    O_RDWR      dd 2
    SYS_SEEK    dd 19
    SEEK_SET    dd 0

    stdin dd 0
    stdout dd 1
    buffer dd 1

    input_file dd -1
    input_file_flag dd "-i", 0

    output_file dd -1
    output_file_flag dd "-o", 0

    file_not_found dd "file not found", 0
    error_msg_length dd 14

    newline dd 10

section .text
global _start
global system_call
extern strlen
extern strncmp

_start:
    pop     dword ecx    ; ecx = argc
    mov     esi, esp     ; esi = argv
    mov     eax, ecx     ; put the number of arguments into eax

    ; shift num of args two bytes to the left (mult by pointer size (= 4))
    shl     eax, 2       ; compute the size of argv in bytes

    add     eax, esi     ; add the size to the address of argv 
    add     eax, 4       ; skip NULL at the end of argv

    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc
 
    call    main        ; int main( int argc, char *argv[], char *envp[] )

    ; exit
    mov     ebx, eax
    mov     eax, 1
    int     0x80
    nop

system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller



; print a new line to stdout
print_new_line:
    push ebp
    mov  ebp, esp

    push dword 1          
    push dword newline           
    push dword [stdout]   
    push dword 4   
    call system_call
    add  esp, 16

    pop  ebp
    ret

main:
    ; save the caller's state
    push ebp             
    mov ebp, esp
    pushad
        
    ; get the program's arguments
    mov ebx, [ebp+8]    ; argc
    mov ecx, [ebp+12]   ; argv
    mov edx, [ebp+16]   ; envp

    ; set default input stream
    mov eax, [stdin]
    mov [input_file], eax

    ; set default output stream
    mov eax, [stdout]
    mov [output_file], eax

print_loop:
    cmp ebx, 0
    jz  encode

    ;get the length of the current argument to print
    push dword ecx  

    push dword [ecx]     
    call strlen
    add  esp, 4

    pop ecx ;

    ; print the current argument as string
    push dword eax     ; length of the argument
    push dword [ecx]   ; point the the argument
    push dword [stdout]  
    push dword [SYS_WRITE]  
    call system_call    ; print(arg, argsize, output_stream)
    add  esp, 16
    call print_new_line
input:
    ; check if it is a "-i" flag
    push dword ecx

    push dword 2   
    push dword input_file_flag
    push dword [ecx]
    call strncmp
    add  esp, 12

    pop ecx

    ; check if the flag -i exists
    cmp  eax, 0
    jne  output

    ; open the file
    mov eax, [ecx + 2] 

    push dword 511       ; set perms
    push dword 0         ; set access: read
    push dword eax       ; path
    push dword [SYS_OPEN]         
    call system_call     ;open(path, read)
    add  esp, 16

    mov [input_file], eax

    ; check for error
    cmp dword [input_file], 0
    js error
    je step_setup

output:
    push dword ecx  ; save the current state

    push dword 2   
    push dword output_file_flag
    push dword [ecx]
    call strncmp
    add  esp, 12

    pop ecx ; restore state

    ; if it is not a "-o" flag, continue
    cmp eax, 0
    jne step_setup

    ; open the file if it exists
    mov eax, [ecx + 2]     

    push dword 511       ; set perms to all
    push dword 577       ; set access: W/R/T
    push dword eax       ; path
    push dword [SYS_OPEN]  
    call system_call    ; open(path, perms=all, access=WRT)
    add  esp, 16

    mov [output_file], eax

step_setup:
    ; setup for the next argument
    dec ebx     
    add ecx, 4
    jmp print_loop

error:
    ; find the length of the error msg
    push dword file_not_found   
    call strlen
    add  esp, 4

    ; print the message to stdout
    push dword eax           
    push dword file_not_found         
    push dword [stdout]     
    push dword 4   
    call system_call ;print(error msg)
    add  esp, 16
    call print_new_line ;print(\n)

encode:
    ; read a single character from the input file
    push dword 1           
    push dword buffer         
    push dword [input_file]     
    push dword [SYS_READ]
    call system_call ; read(inputfile, size=1, buffer)
    add  esp, 16

    ; if we have finished reading, end the encoding
    cmp eax, 1
    jne end

    push dword [buffer]
    call encode_char
    add  esp, 4

    jmp encode

end:
    call print_new_line

    ; close the files
    push dword [input_file]
    push 6
    call system_call
    add  esp, 8

    push dword [output_file]
    push 6
    call system_call
    add  esp, 8

    ; exit gracefuly
    popad          
    mov eax, 0 
    pop ebp     
    ret            


; encode a character in ascii range 'A' to 'z' by adding 1 to its ascii value
encode_char:
    push ebp
    mov  ebp, esp
    pushad

    cmp [ebp + 8], dword 'A'
    js  print_encoded_char           ; eax < 'A'

    cmp [ebp + 8], dword 'z'
    ja  print_encoded_char           ; 'z' < eax

    inc dword [ebp + 8] 

print_encoded_char: 
    mov ebx, ebp
    add ebx, 8

    push dword 1     ; length
    push dword ebx   ; pointer
    push dword [output_file]
    push dword [SYS_WRITE]   
    call system_call    ;print (outputfile, length=1)
    add  esp, 16

    popad
    mov eax, 0
    pop ebp
    ret
