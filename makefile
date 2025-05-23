.PHONY: all

all: main.gb clean

main.gb: main.o
	rgblink --dmg --tiny --map main.map --sym main.sym -o main.gb main.o
	rgbfix -v -p 0xFF main.gb

main.o: src/main.asm src/type1.asm src/type2.asm src/hardware.inc src/utils.inc
	rgbasm -o main.o src/main.asm
	rgbasm -o type1.o src/type1.asm
	rgbasm -o type2.o src/type2.asm

clean:
	rm *.sym *.o *.map