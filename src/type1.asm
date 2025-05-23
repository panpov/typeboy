include "src/utils.inc"
section "one_type", rom0

type1:
    ld a, [JOYPAD_PRESSED_ADDRESS]
    and PADF_RIGHT
    jr nz, .right_not_pressed

    ; call next_character

    .right_not_pressed
    ret

; next_character:
;     AddBetter [CURR_CHAR], 1

;     ; check if we need to wrap around
;     cp $9A
;     jr nz, .not_wrap_around

;     ; wrap around
;     ld a, 0x00
;     ld [CHARACTER], a