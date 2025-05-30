; file: main.asm
; authors: Jun Seo and Pan Pov
; brief: main operations of TypeBoy

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

    ; for proper sprite initialization
    halt

    ; selection sprite
    Copy [SPR0 + OAMA_X], INIT_X
    Copy [SPR0 + OAMA_Y], TYPE1_Y
    Copy [SPR0 + OAMA_TILEID], CURSOR_TILEID
    Copy [SPR0 + OAMA_FLAGS], OAMF_PAL0

    .start_screen
        UpdateJoypad
        
        AddBetter [TIMER], 1

        halt
        ; (a) should be TIMER, since prev line updates timer
        and TICK_RATE
        jr nz, .start_screen

        call move_selection

        jr .start_screen

    .type1_loop

        jr .type1_loop
    
    .type2_loop

        jr .type1_loop

move_selection:
    ; check if up pressed
    ld a, [PADA_CURR]
    and PADF_UP
    jr nz, .up_unpressed
    
    ; do not move up if already on first option
    ld a, [SPR0 + OAMA_Y]
    cp a, TYPE1_Y
    jr z, .up_unpressed

    AddBetter [SPR0 + OAMA_Y], -SEL_DIST
    MoveSelectionSound
    .up_unpressed
    
    ; check if down pressed
    ld a, [PADA_CURR]
    and PADF_DOWN
    jr nz, .down_unpressed
    
    ; do not move down if already on second option
    ld a, [SPR0 + OAMA_Y]
    cp a, TYPE2_Y
    jr z, .down_unpressed

    AddBetter [SPR0 + OAMA_Y], SEL_DIST
    MoveSelectionSound
    .down_unpressed
    ret

; in progress
confirm_selection:

    ret

section "graphics_data", rom0[GRAPHICS_DATA_START]
incbin "assets/tileset.chr"
incbin "assets/background.tlm"
