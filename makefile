.PHONY: all

all: main.gb clean

main.gb: main.o type1.o type2.o
	rgblink --dmg --tiny --map main.map --sym main.sym -o main.gb main.o type1.o type2.o
	rgbfix -v -p 0xFF main.gb

main.o: src/asm/main.asm src/inc/*.inc
	rgbasm -o main.o src/asm/main.asm

type1.o: src/asm/type1.asm src/inc/*.inc
	rgbasm -o type1.o src/asm/type1.asm

type2.o: src/asm/type2.asm src/inc/*.inc
	rgbasm -o type2.o src/asm/type2.asm

clean:
	rm *.sym *.o *.map