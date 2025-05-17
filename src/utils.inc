if !def(UTILS_RGBINC)
def UTILS_RGBINC equ 1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include "src/hardware.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

rsset $C010
def TIMER   rb 1

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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

rsset _RAM ; $C000
def JOYPAD_CURRENT_ADDRESS  rb 1
def JOYPAD_PREVIOUS_ADDRESS rb 1
def JOYPAD_PRESSED_ADDRESS  rb 1
def JOYPAD_RELEASED_ADDRESS rb 1

macro UpdateJoypad

    ld a, [JOYPAD_CURRENT_ADDRESS]
    ld [JOYPAD_PREVIOUS_ADDRESS], a
    ld c, a ; (c) contains previous joypad

    ld a, P1F_GET_BTN ; that's %00010000
    ld [rP1], a

    ; wait!
    ld a, [rP1]
    ld a, [rP1]
    ld a, [rP1]
    ld a, [rP1]
    ld a, [rP1]

    ; read button poll result
    ld a, [rP1]

    ; save button result in b
    and $0F
    ld b, a

    ld a, P1F_GET_DPAD
    ld [rP1], a

    ; wait
    ld a, [rP1]

    ; read dpad poll result
    ld a, [rP1]

    ; move dpad result and load button result into (a)
    and $0F
    swap a
    or a, b

    ; store in our custom joypad byte
    ld [JOYPAD_CURRENT_ADDRESS], a
    ld b, a ; (b) contains current joypad byte

    ; update pressed
    ; for each bit: if 1 before, 0 now then it was pressed, set to 0

    ; load previous joypad byte (from (c))
    ld a, c
    cpl ; now previous holds are 1's
    or b ; or with current joypad byte
    
    ; now any that were previous held or currently not held are 1's
    ; and all presses are 0's
    ld [JOYPAD_PRESSED_ADDRESS], a

    ; update released
    ; for each bit, if 0 before, and 1 now, then it was released

    ; load current joypad byte (in (b))
    ld a, b
    cpl ; now current holds are 1's
    or c ; or previous joypad byte

    ; now any that were previous not hold and current hold are 1's
    ; and all releases are 0's
    ld [JOYPAD_RELEASED_ADDRESS], a

    ld a, P1F_GET_NONE
    ld [rP1], a

endm

endc
