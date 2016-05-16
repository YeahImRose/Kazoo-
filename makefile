CXX=g++
CXXFLAGS=-Wall
OBJS=main.o
EXECUTABLE=main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"main.d" -MT"main.o" -o "main.o" "main.cpp"
	g++ -framework SDL2 -o "KazooQuestPP"  ./main.o   -lncurses -lpthread -lmenu


clean:
	rm -f $(EXECUTABLE)
	rm -f main.o 

main.o: main.cpp
