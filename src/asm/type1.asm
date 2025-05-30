include "src/inc/utils.inc"
section "type1", rom0

; init window position
def WINDOW_TYPE1_X                   equ (7)
def WINDOW_TYPE1_Y                   equ (110)

; tile addresses and tile ids
def TOP_LEFT_TILE_ADDRESS            equ ($9800)
def CENTER_TILE_ADDRESS              equ ($9905)
; def WINDOW_CENTER_ADDRESS            equ ($9C29)
def TILEID_A                         equ ($80)
; def TILEID_Z                         equ ($99)

; buffer
def BUFFER_ADDRESS_INIT              equ ($11) ; Add $C000 (_RAM)
def BUFFER_SIZE                      equ (18)
def CHARACTER_COUNT                  equ (26)

rsset _RAM + $10
def BUFFER_ADDRESS                   rb 1

init_type1:
    ; Initialize the window position to bottom
    Copy [rWX], WINDOW_TYPE1_X
    Copy [rWY], WINDOW_TYPE1_Y

    ; write "A" to the center of the background
    Copy [TOP_LEFT_TILE_ADDRESS], TILEID_A
    Copy [CURR_CHARACTER], TILEID_A
    Copy [CENTER_TILE_ADDRESS], TILEID_A

    ; initializes the buffer and characters in WRAM
    call init_characters
    ret

init_characters:
    ; the buffer address will point to the first character in the buffer
    Copy [BUFFER_ADDRESS], BUFFER_ADDRESS_INIT
    ld hl, _RAM + BUFFER_ADDRESS_INIT
    ld b, TILEID_A
    ld c, CHARACTER_COUNT

    ; initialize the characters in WRAM
    .init_next_character
        Copy [hli], b
        inc b

        ; initialize next character until 26th character
        dec c
        jr nz, .init_next_character
    ret

print_buffer:
    ; Get current buffer address and location to print
    ld h, 0
    Copy l, [BUFFER_ADDRESS]
    ld bc, _RAM
    add hl, bc
    ld de, $9C21
    ld b, BUFFER_SIZE

    ; print the characters in the buffer in the window
    .print_next_character
        Copy [de], [hli]
        inc de
        dec b
        jr nz, .print_next_character

    ; update the current character tile
    ; Copy [A_TILE_ADDRESS], [CURR_CHARACTER]
    ret

type1:
    ; check if the RIGHT DPAD is pressed
    call check_next_character

    ; check if the LEFT DPAD is pressed
    call check_prev_character

    call print_buffer

    ret

check_next_character:
    ;; using PADA_CURR will be used instead to check if button held
    ld a, [PADA_PRESSED]
    and PADF_RIGHT
    jr nz, .right_not_pressed

    ; if RIGHT DPAD is pressed, go to next character
    AddBetter [CURR_CHARACTER], 1
    call scroll_window_right
    WriteTile CENTER_TILE_ADDRESS, [CURR_CHARACTER]

    .right_not_pressed
    ret

check_prev_character:
    ;; using PADA_CURR will be used instead to check if button held
    ld a, [PADA_PRESSED]
    and PADF_LEFT
    jr nz, .left_not_pressed

    ; if LEFT DPAD is pressed, go to previous character
    AddBetter [CURR_CHARACTER], -1
    call scroll_window_left
    WriteTile CENTER_TILE_ADDRESS, [CURR_CHARACTER]

    .left_not_pressed
    ret

scroll_window_right:
    AddBetter [BUFFER_ADDRESS], 1
    ret

scroll_window_left:
    AddBetter [BUFFER_ADDRESS], -1
    ret

; shift all the characters (choices) depending on the button pressed
; shift_window_right:

;     ret





export type1, init_type1