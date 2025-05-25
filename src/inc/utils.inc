if !def(UTILS_INC)
def UTILS_INC equ 1

; 
include "src/inc/hardware.inc"
include "src/inc/graphics.inc"
include "src/inc/joypad.inc"

rsset $C004 ; after joypad addresses
def TIMER                           rb 1
def CURR_CHARACTER                  rb 1  

def DEFAULT_PALETTE                 equ (%11100100)
def WX_INIT                         equ (7)
def WY_INIT                         equ (144)
def SCX_INIT                        equ (0)
def SCY_INIT                        equ (0)

; sprite addresses
def SPRITE_0_ADDRESS                equ (_OAMRAM)
def SPRITE_1_ADDRESS                equ (_OAMRAM + sizeof_OAM_ATTRS)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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

endc
