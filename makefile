main.cas: main.c graphics.h graphics.asm graphics.c games.h utils.c utils.h keyboard.c keyboard.h game_of_life.c game_of_life.asm countries.c typing.c reaction_test.c maze.c
	zcc +p2000 -O3 -SO3 -DAMALLOC -DREGSCALL --max-allocs-per-node20000 --list --c-code-in-asm -compiler=sdcc --math32 -o main \
	main.c graphics.c graphics.asm utils.c keyboard.c game_of_life.c game_of_life.asm countries.c typing.c reaction_test.c maze.c \
	-create-app

main.bin main.map main.rom: main.c graphics.h graphics.asm graphics.c games.h utils.c utils.h keyboard.c keyboard.h walker.c tetris.c game_of_ur.c game_of_life.c game_of_life.asm countries.c typing.c reaction_test.c maze.c
	zcc +embedded -clib=sdcc_iy \
	main.c walker.c graphics.c graphics.asm utils.c keyboard.c tetris.c game_of_ur.c game_of_life.c game_of_life.asm countries.c typing.c reaction_test.c maze.c \
	-startup=2 \
	-pragma-define:CRT_ORG_CODE=0x1000 \
	-pragma-define:CRT_ORG_DATA=0x6500 \
	-pragma-define:REGISTER_SP=0x9FFF \
	-pragma-define:CRT_STACK_SIZE=256 \
	-pragma-define:CRT_INCLUDE_PREAMBLE=1 \
	-pragma-define:CLIB_FOPEN_MAX=0 \
	-SO3 -bn main.bin \
	--max-allocs-per-node10000 \
	--fsigned-char \
	--c-code-in-asm --list \
	-create-app -m
	rm main.bin
	mv main.rom main.bin

clean:
	rm *.lis
