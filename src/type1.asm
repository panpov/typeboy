include "src/utils.inc"
section "type1", rom0

def TILEID_A                         equ ($80)
def TILEID_Z                         equ ($99)


type1:
    ld a, [JOYPAD_PRESSED_ADDRESS]
    and PADF_RIGHT
    jr nz, .right_not_pressed

    WriteTile $9905, [CURR_CHARACTER]
    AddBetter [CURR_CHARACTER], 1
    ; call next_character

    .right_not_pressed
    ret

next_character:
    AddBetter [CURR_CHARACTER], 1

    ; check if we need to wrap around
    cp $9A
    jr nz, .not_wrap_around

    ; wrap around
    Copy [CURR_CHARACTER], TILEID_A

    .not_wrap_around
    ret

export type1, next_character