CXX=g++
CXXFLAGS=-Wall
OBJS=main.o
EXECUTABLE=main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	g++ -D_THREAD_SAFE -I/usr/local/include/SDL2 -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"main.d" -MT"main.o" -o "main.o" "./main.cpp"
	g++ -L/usr/local/lib -o "KazooQuestPP"  ./main.o   -lncurses -lSDL2 -lSDL2_mixer -lpthread -lmenu


clean:
	rm -f $(EXECUTABLE)
	rm -f main.o 

main.o: main.cpp