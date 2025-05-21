all:
	gcc falling_sand.c -o app -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
run:
	gcc falling_sand.c -o app -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && ./app

