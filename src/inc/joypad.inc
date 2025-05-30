if !def(JOYPAD_INC)
def JOYPAD_INC equ 1

include "src/inc/hardware.inc"

rsset _RAM ; $C000
def PADA_CURR           rb 1
def PADA_PREV           rb 1
def PADA_PRESSED        rb 1
def PADA_RELEASED       rb 1

; joypad
macro InitJoypad
    ld a, $FF
    ld [PADA_CURR], a
    ld [PADA_PREV], a
    ld [PADA_PRESSED], a
    ld [PADA_RELEASED], a
endm

; from FA24 CPSCI-240
; will probably need to rework
macro UpdateJoypad
    ld a, [PADA_CURR]
    ld [PADA_PREV], a
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
    ld [PADA_CURR], a
    ld b, a ; (b) contains current joypad byte

    ; update pressed
    ; for each bit: if 1 before, 0 now then it was pressed, set to 0

    ; load previous joypad byte (from (c))
    ld a, c
    cpl ; now previous holds are 1's
    or b ; or with current joypad byte
    
    ; now any that were previous held or currently not held are 1's
    ; and all presses are 0's
    ld [PADA_PRESSED], a

    ; update released
    ; for each bit, if 0 before, and 1 now, then it was released

    ; load current joypad byte (in (b))
    ld a, b
    cpl ; now current holds are 1's
    or c ; or previous joypad byte

    ; now any that were previous not hold and current hold are 1's
    ; and all releases are 0's
    ld [PADA_RELEASED], a

    ld a, P1F_GET_NONE
    ld [rP1], a
endm

endc