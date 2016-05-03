# Kazoo-

##Building
To compile:
```
g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"main.d" -MT"main.d" -o "main.o" "./main.cpp"
```
then
```
g++  -o "KazooQuestPP"  ./main.o   -lncurses -lmenu
```
