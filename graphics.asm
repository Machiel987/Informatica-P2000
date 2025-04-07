; Game of life per byte logic

;--------------------------------------------------------
; Interfacing
;--------------------------------------------------------
    EXTERN _rollTableX
    EXTERN _rollTableYUp
    EXTERN _rollTableYDn
    EXTERN _setPixel

    GLOBAL _unsafeHorzLine
    GLOBAL _unsafeVertLine
    GLOBAL _unsafeFillBlock
    GLOBAL _rollLeft2
    GLOBAL _rollRight2
    GLOBAL _rollUp
    GLOBAL _rollDown
    GLOBAL _yAdrLUT

;--------------------------------------------------------
; code
;--------------------------------------------------------
    SECTION code_compiler 

MACRO RollByte AFlip,ANFlip
;    LD E,A                  ; E = nextFlip
;    AND ANFlip
;    LD D,A                  ; D = temporary storage
;    LD A,C
;    AND AFlip
;    OR D
;    LD (HL),A

;    LD C,E                  ; flip = nextFlip

    LD E,A
    AND ANFlip
    LD D,A
    LD A,E
    EX AF,AF'
    AND AFlip
    OR D

    LD (HL),A
ENDM


_unsafeHorzLine:
    LD IY,0
    ADD IY,SP

    LD DE,_yAdrLUT
    LD L,(IY + 4)
    LD H,0
    ADD HL,HL
    ADD HL,DE
    LD A,(HL)
    INC HL
    LD H,(HL)
    LD L,A
    PUSH HL

    AND 0xF0
    LD L,A

    LD B,0
    LD C,(IY + 2)
    INC C
    SRL C                   ; C = (xmin + 1) >> 1

    ADD HL,BC               ; HL = beginAdr

    LD A,(IY + 3)
    INC A
    SRL A
    SUB C
    OR A,A
    JR Z,CleanStackH

    LD B,A                  ; B = maxI (for DJNZ)

    LD A,(IY + 5)
    OR A,A
    JR Z,NoWtH

    POP DE
    LD A,E
    AND 0xF
    OR A,A
    JR Z,Case0
    DEC A
    DEC A
    JR Z,Case1
    JR Case2

Case0:
    LD A,(HL)
    OR 0b00100011
    LD (HL),A
    INC HL
    DJNZ Case0

    JR DoneH

Case1:
    LD A,(HL)
    OR 0b00101100
    LD (HL),A
    INC HL
    DJNZ Case1

    JR DoneH

Case2:
    LD A,(HL)
    OR 0b01110000
    LD (HL),A
    INC HL
    DJNZ Case2

    JR DoneH


NoWtH:
    POP DE
    LD A,E
    AND 0xF
    OR A,A
    JR Z,Case3
    DEC A
    DEC A
    JR Z,Case4
    JR Case5

Case3:
    LD A,(HL)
    AND 0b11111100
    LD (HL),A
    INC HL
    DJNZ Case3

    JR DoneH

Case4:
    LD A,(HL)
    AND 0b11110011
    LD (HL),A
    INC HL
    DJNZ Case4

    JR DoneH

Case5:
    LD A,(HL)
    AND 0b10101111
    LD (HL),A
    INC HL
    DJNZ Case5

    JR DoneH

CleanStackH:
    POP AF

DoneH:
    PUSH IY

    LD H,(IY + 5)
    PUSH HL
    INC SP

    LD L,(IY + 4)
    LD A,(IY + 2)
    CALL _setPixel

    POP IY

    LD H,(IY + 5)
    PUSH HL
    INC SP

    LD L,(IY + 4)
    LD A,(IY + 3)
    CALL _setPixel

    RET


_unsafeVertLine:
    PUSH IX
    LD IX,0
    ADD IX,SP

    LD A,(IX + 7)
    PUSH AF
    INC SP
    LD A,(IX + 4)
    LD L,(IX + 5)
    CALL _setPixel

    LD A,(IX + 5)
    LD L,(IX + 6)
    CP L
    JP Z,DoneV

    LD A,(IX + 7)
    PUSH AF
    INC SP
    LD A,(IX + 4)
    CALL _setPixel

    LD A,(IX + 7)
    PUSH AF
    INC SP
    LD A,(IX + 4)
    LD L,(IX + 5)
    INC L
    CALL _setPixel

    LD A,(IX + 7)
    PUSH AF
    INC SP
    LD A,(IX + 4)
    LD L,(IX + 6)
    DEC L
    CALL _setPixel


    LD DE,_yAdrLUT          ; DE = yAdrLUT
    LD H,0
    LD L,(IX + 5)
    INC L
    INC L
    ADD HL,HL
    ADD HL,DE

    LD A,(HL)
    AND 0xF0
    INC HL
    LD H,(HL)               ; HL = beginAdr
    LD L,A

    LD B,0
    LD C,(IX + 4)
    SRL C                   ; C = x >> 1

    ADD HL,BC
    PUSH HL

    LD H,0
    LD L,(IX + 6)
    INC L
    ADD HL,HL
    ADD HL,DE

    LD A,(HL)
    AND 0xF0
    INC HL
    LD H,(HL)               ; HL = endAdr
    LD L,A

    ADD HL,BC

    POP DE
    EX DE,HL                ; HL = beginAdr, DE = endAdr

    LD A,L
    CP E
    JR NZ,Continue

    LD A,H
    CP D
    JR Z,DoneV

Continue:
    LD B,0
    LD C,80

    LD A,(IX + 7)
    OR A,A
    JR Z,NoWtV

    LD A,(IX + 4)
    AND 1
    JR NZ,Uneven0

Even0:
    LD A,(HL)
    OR 0b00110101
    LD (HL),A
    ADD HL,BC
    LD A,L
    CP E
    JR NZ,Even0

    LD A,H
    CP D
    JR NZ,Even0

    JR DoneV

Uneven0:
    LD A,(HL)
    OR 0b01101010
    LD (HL),A
    ADD HL,BC
    LD A,L
    CP E
    JR NZ,Uneven0

    LD A,H
    CP D
    JR NZ,Uneven0

    JR DoneV

NoWtV:
    LD A,(IX + 4)
    AND 1
    JR NZ,Uneven1

Even1:
    LD A,(HL)
    AND 0b11101010
    LD (HL),A
    ADD HL,BC
    LD A,L
    CP E
    JR NZ,Even1

    LD A,H
    CP D
    JR NZ,Even1

    JR DoneV

Uneven1:
    LD A,(HL)
    AND 0b10110101
    LD (HL),A
    ADD HL,BC
    LD A,L
    CP E
    JR NZ,Uneven1

    LD A,H
    CP D
    JR NZ,Uneven1

DoneV:
    POP IX
    RET

; Not finished, DO NOT USE
_unsafeFillBlock:
    LD IY,0
    ADD IY,SP

    LD DE,_yAdrLUT          ; DE = yAdrLUT
    LD L,(IY + 3)
    LD H,0
    INC L
    INC L
    ADD HL,HL
    ADD HL,DE

    LD A,(HL)
    AND 0xF0
    INC HL
    LD H,(HL)
    LD L,A

    PUSH HL

    LD C,(IY + 2)
    INC C
    SRL C
    LD B,0

    ADD HL,BC               ; HL = p_char

    LD A,(IY + 4)
    INC A
    SRL A

    SUB C
    JR Z,CleanStackB
    LD B,A                  ; B = lineLen
    LD D,B                  ; D = lineLen

    LD A,80
    SUB B
    LD E,A                  ; E = lineDiff

    EXX

    LD DE,_yAdrLUT
    LD L,(IY + 5)
    LD H,0
    INC L
    ADD HL,HL
    ADD HL,DE

    LD A,(HL)
    AND 0xF0
    INC HL
    LD H,(HL)
    LD L,A                  

    POP DE
    
    EX DE,HL                ; DE' = yAdrLUT[ymax + 1] & 0xFFF0
                            ; HL' = yAdrLUT[ymin + 2] & 0xFFF0

    LD B,0
    LD C,80                 ; BC' = 80

    EXX
    LD C,(IY + 6)           ; C = dest

    JR InLoop

OutLoop:
    LD B,D
    LD D,0
    ADD HL,DE
    LD D,B

    EXX
    ADD HL,BC

    LD A,L
    CP E
    JR NZ,OutEnd

    LD A,H
    CP D
    JR Z,DoneB

OutEnd:
    EXX

InLoop:
    LD (HL),C
    INC HL
    DJNZ InLoop

    JR OutLoop

CleanStackB:
    POP AF

DoneB:
    RET


_rollLeft2:
    LD B,A                  ; B = counter (DJNZ)
    PUSH DE
    EX DE,HL                ; HL = currentAdr

    EXX
    POP DE                  ; DE' = currentAdr
    LD HL,_rollTableX       ; HL' = rollTable
    LD B,0
    LD C,(DE)               ; BC' = C' = *currentAdr
    ADD HL,BC
    LD A,(HL)
    EXX

    EX AF,AF'
    LD C,0b00110101

LoopL:
    EXX
    INC DE
    LD C,(DE)
    LD HL,_rollTableX
    ADD HL,BC
    LD A,(HL)
    EXX

    RollByte C,0b01101010

    INC HL

    DJNZ LoopL

    DEC HL
    LD A,(HL)
    AND C
    LD (HL),A

    RET


_rollRight2:
    LD B,A                  ; B = counter (DJNZ)
    PUSH DE
    EX DE,HL                ; HL = currentAdr

    EXX
    POP DE                  ; DE' = currentAdr
    LD HL,_rollTableX       ; HL' = rollTable
    LD B,0
    LD C,(DE)               ; BC' = C' = *currentAdr
    ADD HL,BC
    LD A,(HL)
    EXX

    EX AF,AF'
    LD C,0b01101010

LoopR:
    EXX
    DEC DE
    LD C,(DE)
    LD HL,_rollTableX
    ADD HL,BC
    LD A,(HL)
    EXX

    RollByte C,0b00110101

    DEC HL

    DJNZ LoopR

    INC HL
    LD A,(HL)
    AND C
    LD (HL),A

    RET


_rollUp:
    LD B,A                  ; B = counter (DJNZ)
    PUSH DE
    EX DE,HL                ; HL = currentAdr

    EXX
    POP DE                  ; DE' = currentAdr
    LD HL,_rollTableYUp     ; HL' = rollTable
    LD B,0
    LD C,(DE)               ; BC' = C' = *currentAdr
    ADD HL,BC
    LD A,(HL)
    EXX

    EX AF,AF'
    LD C,0xF

LoopU:
    EXX
    LD C,80
    EX DE,HL
    ADD HL,BC
    EX DE,HL

    LD C,(DE)
    LD HL,_rollTableYUp
    ADD HL,BC
    LD A,(HL)
    EXX

    RollByte C,0x70

    LD D,0
    LD E,80
    ADD HL,DE

    DJNZ LoopU

    SBC HL,DE
    LD A,(HL)
    AND 0x2F
    LD (HL),A

    RET


_rollDown:
    LD B,A                  ; B = counter (DJNZ)
    PUSH DE
    EX DE,HL                ; HL = currentAdr

    EXX
    POP DE                  ; DE' = currentAdr
    LD HL,_rollTableYDn     ; HL' = rollTable
    LD B,0
    LD C,(DE)               ; BC' = C' = *currentAdr
    ADD HL,BC
    LD A,(HL)
    EXX

    EX AF,AF'
    LD C,0x7C

LoopD:
    EXX
    LD C,80
    EX DE,HL
    SBC HL,BC
    EX DE,HL

    LD C,(DE)
    LD HL,_rollTableYDn
    ADD HL,BC
    LD A,(HL)
    EXX

    RollByte C,0x03

    LD D,0
    LD E,80
    SBC HL,DE

    DJNZ LoopD

    ADD HL,DE
    LD A,(HL)
    AND C
    LD (HL),A

    RET