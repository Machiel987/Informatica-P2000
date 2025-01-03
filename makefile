main.bin main.map main.rom: main.c graphics.h graphics.c games.h walker.c utils.c utils.h keyboard.c keyboard.h game_of_life.c game_of_life.asm
	zcc \
	+embedded -clib=sdcc_iy \
	main.c graphics.c walker.c utils.c keyboard.c graphics.asm game_of_life.c game_of_life.asm \
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

#	+embedded -clib=new \
#	-compiler=sccz80 -O3 \

#	-S --c-code-in-asm -z80-verb \

clean:
	rm *.lis
