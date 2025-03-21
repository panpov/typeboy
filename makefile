.PHONY: all

all: typeboy.gb clean

typeboy.gb: typeboy.o
	rgblink --dmg --tiny --map typeboy.map --sym typeboy.sym -o typeboy.gb typeboy.o
	rgbfix -v -p 0xFF typeboy.gb

typeboy.o: src/typeboy.asm src/hardware.inc src/utils.inc
	rgbasm -o typeboy.o src/typeboy.asm

clean:
	rm *.sym *.o *.map