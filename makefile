CXX=g++
CXXFLAGS=-Wall
OBJS=main.o
EXECUTABLE=main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"main.d" -MT"main.o" -o "main.o" "main.cpp"
	g++  -o "KazooQuestPP"  ./main.o   -lncurses -lmenu


clean:
	rm -f $(EXECUTABLE)
	rm -f main.o 

main.o: main.cpp
