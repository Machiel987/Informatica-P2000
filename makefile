main.bin main.map main.rom: main.c graphics.h graphics.c
	zcc \
	+embedded -clib=sdcc_iy \
	main.c graphics.c \
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
	-create-app -m
	rm main.bin
	mv main.rom main.bin

#	+embedded -clib=new \
#	-compiler=sccz80 -O3 \

#	-S --c-code-in-asm -z80-verb \