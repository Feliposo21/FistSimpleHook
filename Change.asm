include "Ricardo.h"
.data
extern jmpBackAddy: QWORD
.CODE

public OurFunct

 OurFunct PROC
    add eax, eax
    mov [rbp + 4], eax
    jmp [jmpBackAddy]
 OurFunct ENDP

END