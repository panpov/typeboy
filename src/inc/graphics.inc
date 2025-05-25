if !def(GRAPHICS_INC)
def GRAPHICS_INC equ 1

include "src/inc/hardware.inc"
include "src/inc/utils.inc"

def TILES_COUNT                     equ (384)
def BYTES_PER_TILE                  equ (16)
def TILES_BYTE_SIZE                 equ (TILES_COUNT * BYTES_PER_TILE)

def TILEMAPS_COUNT                  equ (1)
def BYTES_PER_TILEMAP               equ (1024)
def TILEMAPS_BYTE_SIZE              equ (TILEMAPS_COUNT * BYTES_PER_TILEMAP)

def GRAPHICS_DATA_SIZE              equ (TILES_BYTE_SIZE + TILEMAPS_BYTE_SIZE)
def GRAPHICS_DATA_END               equ ($4000)
def GRAPHICS_DATA_START             equ (GRAPHICS_DATA_END - GRAPHICS_DATA_SIZE)

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

    ld a, WX_INIT
    ld [rWX], a
    ld a, WY_INIT
    ld [rWY], a

    ld a, SCX_INIT
    ld [rSCX], a
    ld a, SCY_INIT
    ld [rSCY], a
endm

macro EnableLCD
    ld a, LCDCF_ON | LCDCF_WIN9C00 | LCDCF_WINON | LCDCF_BG9800 | LCDCF_OBJ8 | LCDCF_OBJON | LCDCF_BGON
    ld [rLCDC], a
endm

endc