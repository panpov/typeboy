if !def(UTILS_INC)
def UTILS_INC equ 1

; 
include "src/inc/hardware.inc"
include "src/inc/graphics.inc"
include "src/inc/joypad.inc"

rsset $C004 ; after joypad addresses
def TIMER                           rb 1
def CURR_TYPE                       rb 1
def CURR_CHARACTER                  rb 1  

def TICK_RATE                       equ (%00000111)
def DEFAULT_PALETTE                 equ (%11100100)
def WX_INIT                         equ (7)
def WY_INIT                         equ (144)
def SCX_INIT                        equ (0)
def SCY_INIT                        equ (0)

; in [CURR_TYPE]
def TYPE1_ID                        equ (1)
def TYPE2_ID                        equ (2)

; start screen
def EMPTY_TID                       equ (2)
def CURSOR_TID                      equ (1)
def INIT_X                          equ (56)
def TYPE1_Y                         equ (88)
def TYPE2_Y                         equ (104)
def SEL_DIST                        equ (16)

; sprite addresses
def SPR0                            equ (_OAMRAM)
def SPR1                            equ (_OAMRAM + sizeof_OAM_ATTRS)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; originally from mdagois's gca repo
; copy \2 into \1 through (a)
; example: Copy [$FF84], 10
macro Copy
    ld a, \2
    ld \1, a
endm

; increase \1 by \2, recommended for use with RAM locations
; example: AddBetter [$C000], 1
macro AddBetter
    ld a, \1
    add a, \2
    ld \1, a
endm

; write in bg index \1 the tile index \2
; \1: $9906, \2: $93 -- this writes tile $93 ('T') in bg index $9906
; 'A' ($80) - 'Z' ($99)
macro WriteTile
    ld a, \2
    ld [\1], a
endm

macro MoveSelectionSound
    Copy [rNR10], $08
    Copy [rNR11], $C0
    Copy [rNR12], $B1
    Copy [rNR13], $07
    Copy [rNR14], $C1
endm

macro ConfirmSelectionSound
    Copy [rNR10], $08
    Copy [rNR11], $80
    Copy [rNR12], $C2
    Copy [rNR13], $42
    Copy [rNR14], $C6
endm

endc
