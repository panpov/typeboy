include "src/inc/utils.inc"

section "vblank_interrupt", rom0[$0040]
    reti

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
    EnableLCD

    ; set up WRAM variables
    xor a
    ld [TIMER], a

    halt
    call init_type1

    .game_loop
        UpdateJoypad
        halt
        AddBetter [TIMER], 1
        
        call type1
        ; call type2

        jr .game_loop

section "graphics_data", rom0[GRAPHICS_DATA_START]
incbin "assets/tileset.chr"
incbin "assets/background.tlm"
