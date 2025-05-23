include "src/hardware.inc"
include "src/utils.inc"

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
        
        WriteTile $9800, $80
        WriteTile $9905, $80
        WriteTile $9906, $99

        halt
        
        jr .game_loop

section "vblank_interrupt", rom0[$0040]
    reti

section "graphics_data", rom0[GRAPHICS_DATA_START]
incbin "assets/tileset.chr"
incbin "assets/background.tlm"
