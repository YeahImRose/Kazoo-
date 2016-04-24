#include "main.h"

//Setting colors for different OS's
#ifdef _WIN32
enum colors {
	black=0,
	blue=1,
	green=2,
	cyan=3,
	red=4,
	magenta=5,
	brown=6,
	lgray=7,
	dgray=8,
	lblue=9,
	lgreen=10,
	lcyan=11,
	lred=12,
	lmagen=13,
	yellow=14,
	white=15
};
#elif __APPLE__
enum colors {
	black=30,
	red=31,
	green=32,
	yellow=33,
	blue=34,
	magenta=35,
	cyan=36,
	white=37
};
#elif __linux__
enum colors {
	black=30,
	red=31,
	green=32,
	yellow=33,
	blue=34,
	magenta=35,
	cyan=36,
	white=37
};
#else
#error "Unknown compiler"
#endif

//Setting color print function for different OS's
#ifdef _WIN32
std::string print(int color, std::string text) {
	printf("0x1B[%dm;%d!", color, text);
}
#elif __APPLE__
std::string print(int color, std::string text ) {
	printf("\x1b[%dm%s\x1b[0m", color, text.c_str());
}
#elif __linux__
std::string print(int color, std::string text ) {
	printf("\x1b[%dm%s\x1b[0m", color, text.c_str());
}
#else
#error "Unknown compiler"
#endif

void clear() {
	#ifdef _WIN32
	system("CLS");
	#elif __APPLE__
	system("clear");
	#elif __linux__
	system("clear");
	#endif
}

int main() {
	clear();
	print(cyan, "Hello and welcome to Kazoo Quest++!\n");
	print(cyan, "This is the remake of the original Kazoo Quest.\n");
	print(green, "This is a alpha build- expect there to be bugs.\n");
	print(white, "Press any key to continue.\n");
	#ifdef _WIN32
	system("pause");
	#elif __APPLE__
	system("read -n1 -p ' ' key");
	#elif __linux__
	system("read -n1 -p ' ' key");
	#endif
	system("clear");
	return 0;
}
