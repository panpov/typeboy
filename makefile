.PHONY: all

all: main.gb clean

main.gb: main.o type1.o type2.o
	rgblink --dmg --tiny --map main.map --sym main.sym -o main.gb main.o type1.o type2.o
	rgbfix -v -p 0xFF main.gb

main.o: src/main.asm src/*.inc
	rgbasm -o main.o src/main.asm

type1.o: src/type1.asm src/*.inc
	rgbasm -o type1.o src/type1.asm

type2.o: src/type2.asm src/*.inc
	rgbasm -o type2.o src/type2.asm

clean:
	rm *.sym *.o *.map