; Game of life per byte logic

;--------------------------------------------------------
; Interfacing
;--------------------------------------------------------
    EXTERN _popCountHigh
    EXTERN _evolve_w
    EXTERN _evolve_h
    EXTERN _evolve_changeListIn
    EXTERN _evolve_changeListOut
    GLOBAL _evolveBoard
    GLOBAL _drawBoard

;--------------------------------------------------------
; code
;--------------------------------------------------------
    SECTION code_compiler

_drawBoard:
    EX DE,HL
    LD B,0

    EXX
    LD A,(_evolve_h)
    LD B,A

Loop:
    EXX
    LD A,(_evolve_w)
    LD C,A

    LDIR

    EX DE,HL
    NEG
    ADD 80
    LD C,A
    ADD HL,BC
    EX DE,HL

    EXX
    DJNZ Loop

    RET



; Input : HL points to byte to process
; Output:  L new byte

; Work  : IY points to byte to process
;         IX points to the change mask
;         HL point to popCount
;          B out
;          C circum
;          D = 0

MACRO HLpopcountA offsetA,bitsA
    LD A,(IY+offsetA)
    AND A,bitsA
    LD L,A
ENDM

MACRO HLpopcountAB offsetA,bitsA,offsetB,bitsB
    LD A,(IY+offsetA)
    AND A,bitsA
    LD E,A
    LD A,(IY+offsetB)
    AND A,bitsB
    OR A,E
    LD L,A
ENDM

MACRO GOLtest mybit,setlabel,nextlabel
;   if (((val & pxNumToChar0) && (circum == 2)) || (circum == 3))
    CP A,3
    JR Z,setlabel
    CP A,2
    JR NZ,nextlabel
    BIT mybit,(IY)
    JR Z,nextlabel
setlabel:
;       out |= pxNumToCharN;
    SET mybit,B
nextlabel:
ENDM

_runByte:
    PUSH IY
    PUSH HL
    POP IY

;   unsigned char out = val & ~change;
;    LD A,(IX)
;    CPL
;    AND (HL)

    EXX
;    LD B,A
    LD B,0x20
    LD A,(_popCountHigh)
    LD H,A
;    LD D,(IX)

;    BIT 0,D
;    JR Z,_next0
;   circum =  popCount[(adr[-81] & 0b01000000) | (adr[-1] & 0b00001010)];
    HLpopcountAB -81,0b01000000,-1,0b00001010
    LD C,(HL)
;   circum += popCount[(adr[-80] & 0b01010000) | (adr[0] & 0b00001110)];
    HLpopcountAB -80,0b01010000,0,0b00001110
    LD A,(HL)
    ADD A,C
;   if (((val & pxNumToChar0) && (circum == 2)) || (circum == 3))
;       out |= pxNumToChar0;
    GOLtest 0,_set0,_next0

;    BIT 1,D
;    JR Z,_next1
;   circum =  popCount[(adr[-80] & 0b01010000) | (adr[0] & 0b00001101)];
    HLpopcountAB -80,0b01010000,0,0b00001101
    LD C,(HL)

;   circum += popCount[(adr[-79] & 0b00010000) | (adr[1] & 0b00000101)];
    HLpopcountAB -79,0b00010000,1,0b00000101
    LD A,(HL)
    ADD A,C

;   if (((val & pxNumToChar1) && (circum == 2)) || (circum == 3))
;       out |= pxNumToChar1;
    GOLtest 1,_set1,_next1

;    BIT 2,D
;    JR Z,_next2
;   circum =  popCount[adr[-1] & 0b01001010];
    HLpopcountA -1,0b01001010
    LD C,(HL)

;   circum += popCount[adr[0]  & 0b01011011];
    HLpopcountA 0,0b01011011
    LD A,(HL)
    ADD A,C

;   if (((val & pxNumToChar2) && (circum == 2)) || (circum == 3))
;       out |= pxNumToChar2;
    GOLtest 2,_set2,_next2

;    BIT 3,D
;    JR Z,_next3
;   circum =  popCount[adr[0] & 0b01010111];
    HLpopcountA 0,0b01010111
    LD C,(HL)

;   circum += popCount[adr[1] & 0b00010101];
    HLpopcountA 1,0b00010101
    LD A,(HL)
    ADD A,C

;   if (((val & pxNumToChar3) && (circum == 2)) || (circum == 3))
;       out |= pxNumToChar3;
    GOLtest 3,_set3,_next3

;    BIT 4,D
;    JR Z,_next4
;   circum =  popCount[(adr[-1] & 0b01001000) | (adr[79] & 0b00000010)];
    HLpopcountAB -1,0b01001000,79,0b00000010
    LD C,(HL)

;   circum += popCount[(adr[0]  & 0b01001100) | (adr[80] & 0b00000011)];
    HLpopcountAB 0,0b01001100,80,0b00000011
    LD A,(HL)
    ADD A,C

;   if (((val & pxNumToChar4) && (circum == 2)) || (circum == 3))
;       out |= pxNumToChar4;
    GOLtest 4,_set4,_next4

;    BIT 6,D
;    JR Z,_next6
;   circum =  popCount[(adr[0] & 0b00011100) | (adr[80] & 0b00000011)];
    HLpopcountAB 0,0b00011100,80,0b00000011
    LD C,(HL)

;   circum += popCount[(adr[1] & 0b00010100) | (adr[81] & 0b00000001)];
    HLpopcountAB 1,0b00010100,81,0b00000001
    LD A,(HL)
    ADD A,C

;   if (((val & pxNumToChar5) && (circum == 2)) || (circum == 3))
;       out |= pxNumToChar5;
    GOLtest 6,_set6,_next6

;   return out;
    LD A,B
    EXX
    POP IY
    RET


_evolveBoard:
    PUSH IX

    LD IX,(_evolve_changeListIn)
    LD IY,(_evolve_changeListOut)

    LD A,(_evolve_h)
    LD C,A

OuterLoop:
    PUSH BC
    LD A,(_evolve_w)
    LD B,A

InnerLoop:
    LD A,(IX)
    OR A,A
    JR Z,Done

    ; Preserves every register, uses HL & IX, returns in A
    CALL _runByte

    LD (DE),A

    CP A,(HL)
    JR Z,Done

    XOR (HL)

    EXX
    PUSH IY
    POP HL

    LD BC,(_evolve_w)
    LD B,0
    INC C
    INC C

    LD E,A      ; Store the previous A in E, so we can restore after AND

    LD (HL),0x5F
    INC HL

    AND 0b01001010
    JR Z,Part2
;    LD A,(HL)
;    OR A,0x15
;    LD (HL),A
    LD (HL),1
Part2:
    LD A,E
    OR A       ; Clear carry
    SBC HL,BC
    AND 0b00000010
    JR Z,Part3
;    LD A,(HL)
;    OR A,0x10
;    LD (HL),A
    LD (HL),1
Part3:
    LD A,E
    DEC HL
    AND 0b00000011
    JR Z,Part4
;    LD A,(HL)
;    OR A,0x50
;    LD (HL),A
    LD (HL),1;
Part4:
    LD A,E
    DEC HL
    AND 0b00000001
    JR Z,Part5
;    LD A,(HL)
;    OR A,0x40
;    LD (HL),A
    LD (HL),1
Part5:
    LD A,E
    ADD HL,BC
    AND 0b00010101
    JR Z,Part6
;    LD A,(HL)
;    OR A,0x4A
;    LD (HL),A
    LD (HL),1
Part6:
    LD A,E
    ADD HL,BC
    AND 0b00010000
    JR Z,Part7
;    LD A,(HL)
;    OR A,0x02
;    LD (HL),A
    LD (HL),1
Part7:
    LD A,E
    INC HL
    AND 0b01010000
    JR Z,Part8
;    LD A,(HL)
;    OR A,0x03
;    LD (HL),A
    LD (HL),1
Part8:
    LD A,E
    INC HL
    AND 0b01000000
    JR Z,End
;    LD A,(HL)
;    OR A,0x01
;    LD (HL),A
    LD (HL),1
End:
    EXX         ; No need to restore A back to original value (E)

Done:
    INC HL
    INC DE
    INC IX
    INC IY

    DJNZ InnerLoop

    INC IX
    INC IX
    INC IY
    INC IY

    LD A,(_evolve_w)
    LD B,A
    LD A,80
    SUB B
    LD B,0
    LD C,A
    ADD HL,BC

    POP BC
    DEC C
    JP NZ,OuterLoop      ; No DJNZ here, jump is too far

    POP IX

    RET

SECTION data_compiler_aligned

ALIGN 256

aligntest:
    DB 0
