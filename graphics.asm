; Game of life per byte logic

;--------------------------------------------------------
; Interfacing
;--------------------------------------------------------
    EXTERN _rollTableX
    EXTERN _rollTableYUp
    EXTERN _rollTableYDn
    GLOBAL _rollLeft
    GLOBAL _rollRight
    GLOBAL _rollUp
    GLOBAL _rollDown

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

_rollLeft:
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


_rollRight:
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