if !def(UTILS_RGBINC)
def UTILS_RGBINC equ 1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

include "src/hardware.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

rsset _RAM ; $C000
def JOYPAD_CURRENT_ADDRESS          rb 1
def JOYPAD_PREVIOUS_ADDRESS         rb 1
def JOYPAD_PRESSED_ADDRESS          rb 1
def JOYPAD_RELEASED_ADDRESS         rb 1
def TIMER                           rb 1

def TILES_COUNT                     equ (384)
def BYTES_PER_TILE                  equ (16)
def TILES_BYTE_SIZE                 equ (TILES_COUNT * BYTES_PER_TILE)

def TILEMAPS_COUNT                  equ (1)
def BYTES_PER_TILEMAP               equ (1024)
def TILEMAPS_BYTE_SIZE              equ (TILEMAPS_COUNT * BYTES_PER_TILEMAP)

def GRAPHICS_DATA_SIZE              equ (TILES_BYTE_SIZE + TILEMAPS_BYTE_SIZE)
def GRAPHICS_DATA_END               equ ($4000)
def GRAPHICS_DATA_START             equ (GRAPHICS_DATA_END - GRAPHICS_DATA_SIZE)

def DEFAULT_PALETTE                 equ (%11100100)
def WINDOW_INIT_X                   equ (7)
def WINDOW_INIT_Y                   equ (144)

; sprite addresses
def SPRITE_0_ADDRESS                equ (_OAMRAM)
def SPRITE_1_ADDRESS                equ (_OAMRAM + sizeof_OAM_ATTRS)


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

; INITIALIZATION

macro DisableLCD
    .wait\@
    ld a, [rLY]
    cp a, SCRN_Y
    jr nz, .wait\@

    xor a
    ld [rLCDC], a
endm

macro InitOAM
    ld c, OAM_COUNT
    ld hl, _OAMRAM + OAMA_Y
    ld de, sizeof_OAM_ATTRS
    
    .init\@
    ld [hl], 0
    add hl, de
    dec c
    jr nz, .init\@
endm

macro InitJoypad
    ld a, $FF
    ld [JOYPAD_CURRENT_ADDRESS], a
    ld [JOYPAD_PREVIOUS_ADDRESS], a
    ld [JOYPAD_PRESSED_ADDRESS], a
    ld [JOYPAD_RELEASED_ADDRESS], a
endm

macro LoadGraphicsIntoVRAM
    ld de, GRAPHICS_DATA_START
    ld hl, _VRAM8000
    
    .load\@
    ld a, [de]
    inc de
    ld [hli], a
    ld a, d
    cp a, high(GRAPHICS_DATA_END)
    jr nz, .load\@
endm

macro InitGraphics
    ld a, DEFAULT_PALETTE    
    ld [rBGP], a
    ld [rOBP0], a
    xor $FF
    ld [rOBP1], a  

    LoadGraphicsIntoVRAM
    ld a, IEF_VBLANK
    ld [rIE], a
    ei

    ; window hidden
    ld a, WINDOW_INIT_X
    ld [rWX], a
    ld a, WINDOW_INIT_Y
    ld [rWY], a

    xor a
    ld [rSCX], a
    ld [rSCY], a
endm

macro EnableLCD
    ld a, LCDCF_ON | LCDCF_WIN9C00 | LCDCF_WINON | LCDCF_BG9800 | LCDCF_OBJ8 | LCDCF_OBJON | LCDCF_BGON
    ld [rLCDC], a
endm

macro InitSprites
    ; initialize the cursor
    Copy [SPRITE_0_ADDRESS + OAMA_X], 8
    Copy [SPRITE_0_ADDRESS + OAMA_Y], 16
    Copy [SPRITE_0_ADDRESS + OAMA_TILEID], SPRITE_0_ADDRESS
    Copy [SPRITE_0_ADDRESS + OAMA_FLAGS], OAMF_PAL0
endm

endc
