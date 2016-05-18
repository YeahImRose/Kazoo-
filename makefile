CXX=g++
CXXFLAGS=-Wall
OBJS=main.o
EXECUTABLE=main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	g++ -D_THREAD_SAFE -I/usr/local/include/SDL2 -I/usr/local/Cellar/sdl2_mixer/2.0.1/include/SDL2 -I/usr/local/Cellar/sdl2/2.0.4/include/SDL2 -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"main.d" -MT"main.o" -o "main.o" "./main.cpp"
	g++ -L/usr/local/Cellar/sdl2/2.0.4/include/SDL2 -L/usr/local/Cellar/sdl2_mixer/2.0.1 -o "KazooQuestPP"  ./main.o   -lncurses -lSDL2 -lSDL2_mixer -lpthread -lmenu


clean:
	rm -f $(EXECUTABLE)
	rm -f main.o 

main.o: main.cpp