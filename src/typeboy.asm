include "src/hardware.inc"
include "src/utils.inc"

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
    ; pass
endm

section "header", rom0[$0100]
entrypoint:
    di
    jr main
    ds ($0150 - @), 0

section "main", rom0[$0150]
main:
    DisableLCD
    InitOAM
    InitJoypad
    InitGraphics
    InitSprites
    EnableLCD

    ; set up WRAM variables
    xor a
    ld [TIMER], a

    .game_loop
        AddBetter [TIMER], 1
        
        WriteTile $9905, $80
        WriteTile $9906, $99

        halt
        
        jr .game_loop

section "vblank_interrupt", rom0[$0040]
    reti

section "graphics_data", rom0[GRAPHICS_DATA_START]
incbin "assets/tileset.chr"
incbin "assets/background.tlm"
