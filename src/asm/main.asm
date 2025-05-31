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
    ld [CURR_TYPE], a

    ; for proper sprite initialization
    halt

    ; cursor sprite
    Copy [SPR0 + OAMA_X], INIT_X
    Copy [SPR0 + OAMA_Y], TYPE1_Y
    Copy [SPR0 + OAMA_TILEID], CURSOR_TID
    Copy [SPR0 + OAMA_FLAGS], OAMF_PAL0
    jp .start_screen

    .type1_init
        call init_type1

    .type1_loop
        UpdateJoypad
        AddBetter [TIMER], 1
        WriteTile $9800, $B1

        halt
        call type1

        jr .type1_loop
    
    .type2_init
        ld de, $24
    
    .type2_loop
        UpdateJoypad
        AddBetter [TIMER], 1
        WriteTile $9800, $B2
        
        halt
        call type2

        jr .type2_loop

    .start_screen
        UpdateJoypad
        AddBetter [TIMER], 1

        halt
        ; (a) should be TIMER, since prev line updates timer
        and TICK_RATE
        jr nz, .start_screen

        call move_selection
        call confirm_selection

        ; COULD make this into a function
        ; skip next section if no selection has been made
        ; -----------------------------------------------
        ld a, [CURR_TYPE]
        cp a, 0
        jr z, .start_screen

        ; hide cursor and update background
        Copy [SPR0 + OAMA_TILEID], EMPTY_TID
        DisableLCD
        UpdateTilemap BACKGROUND, _SCRN0
        EnableLCD

        ; jump to loop of selected type
        ld a, [CURR_TYPE]
        cp a, TYPE1_ID
        jp nz, .type2_init

        ; switch to type 1 loop
        jp .type1_init
        ; -----------------------------------------------

        jp .start_screen

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

confirm_selection:
    ; check if start pressed
    ld a, [PADA_CURR]
    and PADF_START
    jr nz, .start_unpressed

    ConfirmSelectionSound

    ; check if type 1 selected
    ld a, [SPR0 + OAMA_Y]
    cp a, TYPE1_Y
    jr nz, .type2_selected

    ; type1_selected
    Copy [CURR_TYPE], TYPE1_ID
    jr .start_unpressed
    .type2_selected
    
    ; type2_selected
    Copy [CURR_TYPE], TYPE2_ID
    .start_unpressed
    ret

section "graphics_data", rom0[GRAPHICS_DATA_START]
incbin "assets/tileset.chr"

START_SCREEN:
    incbin "assets/start_screen.tlm"

WINDOW:
    incbin "assets/window.tlm"

BACKGROUND:
    incbin "assets/background.tlm"