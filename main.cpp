#include "main.h"
#include "map.h"
#include "enemies.h"
#include "allies.h"
#include <SDL.h>
#include <SDL_mixer.h>
/*extern "C" {
	#include <avcodec>
	#include <avformat>
}*/

#define gamev "Pre-Alpha 0.52"

#ifdef _WIN32
	#define os 1
#endif
#ifdef __APPLE__
	#include <mach-o/dyld.h>
	#define os 2
#endif

using namespace std;

MENU *menu;
ITEM **items;
WINDOW *wmenu;
WINDOW *info;
WINDOW *user;
WINDOW *wpart;
WINDOW *arrows;
WINDOW *pname;

Mix_Music *bgsong[50];  // Background Music
Mix_Chunk *lvlup, *ehit, *uhit, *ahit;  // For Sounds
int songlen[50];
int songpos = 0;
wav_hdr wavHeader;

player plr ={{"Player"}, {25, 25, 7, 1, 2, 15, 10}, {0, 20, 1, 10}};

const char *choices[][10] = {
		{"Test battle", "Save", "Load", "Options", "Exit", "Test map"},
		{"Training dummy", "Fodder enemy", "Test boss", "Back"},
		{"Attack", "Spells", "Check enemy", "Items", "RUNNN!!!"},
		{"Firebolt", "Frost", "Bolt", "Quake", "Poison", "Lifesteal", "Heal"},
		{"Liberty Prime", "Nep-Nep", "Something-something Verne", "Chen"},
		{"Delete save", "Remove current ally", "Back"},
		{"Attack", "Skills"},
		{},
		{},
		{"Yes", "No"},
		{"   Initiates debug battle", "   Save game", "   Load game", "   Open options menu", "   Closes the game", "Enters debug map mode"},
		{"   Lots of health", "   Could be a bit stronger", "   A tough-ish enemy", "   Return to main menu"},
		{"   Basic attack", "   Pew pew magic!", "   Display enemy stats and info", "   View/Use items", "   Run away"},
		{"   An incendiary explosion with a chance to inflict burn", "   An ancient spell from the ice age- chance to inflict freeze", "   A strike of lightning from the sky- chance to inflict paralysis", "   A spell that erupts the ground to pummel the enemy", "   A venomous spell that will probably inflict poison", "   A spell from the vampire underworld that absorbs the enemy's health", "   An angelic spell to restore some of your health"},
		{"   Throws nukes to defeat communism", "   What do you mean, \"Fourth Wall\"?", "   <Insert lore here>", "   Adorbs"},
		{"   Deletes your save...", "   Un-equip your current ally", "   Return to main menu"},
		{"   Basic attack", "   Ally's skills"},
		{},
		{},
		{" ", " "}
	};

std::string inventory[][5] = {
		{"Book", "Another Book", "Random Book", "Liberty Book", "Short Book"},
		{"Honking Book", "MLG Book", "Fab Book", "Debug Book", "Kappa Book"},
		{"Book Book", "Histoire's Tome/Book", "Imaginary Book", "Frozen Book", "THE BOOK"},
		{"Picture Book", "Coloring Book", "Hardcover Book", "Mak Book", "Biilbe"},
		{"Extra 1", "Extra 2", "Extra 3", "Extra 4", "Extra 5"},
		{"   A simple book", "   Another simple book", "   You don't know where this came from", "   The cover shows an eagle flying over thousands of tanks", "   It's just a piece of paper"},
		{"   A mysterious honking is always emitted from this book", "   A book that is 2 1337 4 u", "   The fab levels of this book are beyond comprehension", "   A book that kills bugs- a programmers dream", "   A book full of the greatest memes in history"},
		{"   A book-ish book", "   How did this get here?", "   The book you wanted to write but couldn't be bothered to", "   Might do something special!", "   The only book"},
		{"   A book for young childrens", "   A book for da colors", "   Might hurt if you hit yourself with it", "   A non-copyrighted book", "   A famous religious book"},
		{" ", " ", " ", " ", " "}
};

std::string mapa[10][10] = {
		//North \/, south /\, east <, west >
		{"", "", "", "", "", "", "", "", "", ""},
		{"", "", "", "", "", "", "", "", "", ""},
		{"", cave, 	cave1,	cave2,	outside1,	"", "", "", "", ""},
		{"", "", "", "", forest1, "", "", "", "", ""},
		{"", "", "", "", "", "", "", "", "", ""},
		{"", "", "", "", "", "", "", "", "", ""}
};
std::string mapinfo[10][10] = {
		{"Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing"},
		{"Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing"},
		{"Nothing", icave    , icave1   , icave2   , ioutside1, "Nothing", "Nothing", "Nothing", "Nothing", "Nothing"},
		{"Nothing", "Nothing", "Nothing", "Nothing",  iforest1, "Nothing", "Nothing", "Nothing", "Nothing", "Nothing"},
		{"Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing"},
		{"Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing", "Nothing"}
};

/*void musicPlayer(void *udata, Uint8 *stream, int len)
{
    int i, pos=*(int*)udata;

    // fill buffer with...uh...music...
    for(i=0; i<len; i++)
        stream[i]=(i+pos);

    // set udata for next time
    pos+=len;
    *(int*)udata=pos;
}*/
int getFileSize(FILE* inFile);

unsigned DecodeMP3SafeInt(unsigned nVal)
{
    // nVal has 4 bytes (8-bits each)
    //  - discard most significant bit from each byte
    //  - reverse byte order
    //  - concatenate the 4 * 7-bit nibbles into a 24-bit size.
    unsigned char *pValParts = reinterpret_cast<unsigned char *>(&nVal);
    return (pValParts[3] & 0x7F)         |
            ((pValParts[2] & 0x7F) << 7)  |
            ((pValParts[1] & 0x7F) << 14) |
            ((pValParts[0] & 0x7F) << 21);
}

int alen(std::string filePath) {
	int len = 0;
	if(filePath.substr(filePath.find_last_of(".")) == ".wav") {
		wav_hdr wavHeader;
		int headerSize = sizeof(wav_hdr), filelength = 0;
		FILE* wavFile = fopen(filePath.c_str(), "r");
		if (wavFile == nullptr) {}

		//Read the header
		size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
		//cout << "Header Read " << bytesRead << " bytes." << endl;
		if (bytesRead > 0)
		{
			//Read the data
			//uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number     of bytes per sample
			//uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?
			static const uint16_t BUFFER_SIZE = 4096;
			int8_t* buffer = new int8_t[BUFFER_SIZE];
			delete [] buffer;
			buffer = nullptr;
			filelength = getFileSize(wavFile);

			//cout << "Data size                  :" << wavHeader.ChunkSize << endl;
			//cout << "Number of bytes per second :" << wavHeader.bytesPerSec << endl;

		}
		fclose(wavFile);
		len = wavHeader.ChunkSize / wavHeader.bytesPerSec;
		wavHeader = WAV_HEADER();
	}
	if(filePath.substr(filePath.find_last_of(".")) == ".mp3") {
		//Question life
	}
	return len;
}

void proBar() {
	while(1) {
	this_thread::sleep_for( chrono::seconds( 1 ) );
	curs_set(FALSE);
	if(Mix_PlayingMusic() == 1 && Mix_PausedMusic() == 0) {
		songpos++;
	}
	int lks = loops % songs.size();
	int dist = songpos * 25 / songlen[lks];
	std::string sec;
	std::string songstr;
	std::string songsec;

	if(songlen[lks] % 60 < 10) {
		songsec = "0" + to_string(songlen[lks] % 60);
	} else {
		songsec = to_string(songlen[lks] % 60);
	}

	if((songlen[lks] / 60) > 9) {
		songstr = "/" + to_string(songlen[lks] / 60) + ":" + songsec;
	} else {
		songstr = "/0" + to_string(songlen[lks] / 60) + ":" + songsec;
	}
	move(29, 0); clrtoeol();
	//int smin = (songpos / 60);
	if((songpos / 60) > 0) {
		if(songpos % 60 < 10) {
			sec = "0" + to_string(songpos % 60);
		} else {
			sec = to_string(songpos % 60);
		}
		mvprintw(29, 1, ("0" + to_string((songpos / 60)) + ":" + sec + songstr).c_str());
	} else if((songpos / 60) > 9) {
		if(songpos % 60 < 10) {
			sec = "0" + to_string(songpos % 60);
		} else {
			sec = to_string(songpos % 60);
		}
		mvprintw(29, 1, (to_string((songpos / 60)) + ":" + sec + songstr).c_str());
	} else if((songpos / 60) < 1) {
		if(songpos % 60 < 10) {
			sec = "0" + to_string(songpos % 60);
		} else {
			sec = to_string(songpos % 60);
		}
		mvprintw(29, 1, ("00:" + sec  + songstr).c_str());
	}
	mvprintw(29, 15, "[");
	mvprintw(29, 41, "]");
	for(i=0; i < dist; i++) {
		mvprintw(29, i + 16, "#");
		//mvprintw(27, 1, std::to_string(dist).c_str());
	}
	refresh();
	}
}

int getFileSize(FILE* inFile) {
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}

void convertFile(std::string file) {
	//TO DO: Find Windows solution for this
	file = file.substr(0, file.size()-4);
	system((dir + "/mp3towav.sh \"" + dir + "Sounds/bgmusic/" + file + ".mp3\" \"" + dir + "/Sounds/bgmusic/" + file + ".wav\" ").c_str());
	system(("rm \"" + dir + "Sounds/bgmusic/" + file + ".mp3\"").c_str());
}

void musicFinished() {
	Mix_ExpireChannel(-1, 1);
	loops++;
	int lks = loops % songs.size();
	std::string nowsong = songs[lks];
	std::string sec;
	std::string songstr;
	std::string songsec;
	Mix_PlayMusic(bgsong[lks],0);
	move(26, 20); clrtoeol();
	nowsong = nowsong.substr(0, nowsong.size()-4);
	mvprintw(26, 20, nowsong.c_str());
	move(29, 0); clrtoeol();
	if(songlen[lks] % 60 < 10) {
		songsec = "0" + to_string(songlen[lks] % 60);
	} else {
		songsec = to_string(songlen[lks] % 60);
	}

	if((songlen[lks] / 60) > 9) {
		songstr = "/" + to_string(songlen[lks] / 60) + ":" + songsec;
	} else {
		songstr = "/0" + to_string(songlen[lks] / 60) + ":" + songsec;
	}
	move(29, 0); clrtoeol();
	//int smin = (songpos / 60);
	if((songpos / 60) > 0) {
		if(songpos % 60 < 10) {
			sec = "0" + to_string(songpos % 60);
		} else {
			sec = to_string(songpos % 60);
		}
		mvprintw(29, 1, ("0" + to_string((songpos / 60)) + ":" + sec + songstr).c_str());
	} else if((songpos / 60) > 9) {
		if(songpos % 60 < 10) {
			sec = "0" + to_string(songpos % 60);
		} else {
			sec = to_string(songpos % 60);
		}
		mvprintw(29, 1, (to_string((songpos / 60)) + ":" + sec + songstr).c_str());
	} else if((songpos / 60) < 1) {
		if(songpos % 60 < 10) {
			sec = "0" + to_string(songpos % 60);
		} else {
			sec = to_string(songpos % 60);
		}
		mvprintw(29, 1, ("00:" + sec  + songstr).c_str());
	}
	mvprintw(29, 15, "[");
	mvprintw(29, 41, "]");
	refresh();
	songpos = 0;
}

void setsound() {
	if( SDL_Init(SDL_INIT_AUDIO) < 0 ){
		return;
	}
	if( Mix_Init(MIX_INIT_MP3) < 0 ){
		return;
	}
	Mix_OpenAudio(44100, AUDIO_S16, 2, 4960);
	for(i=0;i < songs.size(); i++) {
		std::string sname = songs[i];
		bgsong[i] = Mix_LoadMUS(((dir + "Sounds/bgmusic/" + sname).c_str()));
		songlen[i] = alen((dir + "Sounds/bgmusic/" + sname).c_str());
	}
	lvlup = Mix_LoadWAV((dir + "Sounds/count.wav").c_str());
	Mix_VolumeMusic(MIX_MAX_VOLUME/2);
	Mix_HookMusicFinished(musicFinished);
}

int main() {
	getdir();
	struct dirent *pDirent;
	DIR *pDir;
    pDir = opendir ((dir + "/Sounds/bgmusic").c_str());
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n", (dir + "/Sounds/bgmusic").c_str());
        return 1;
    }
	while ((pDirent = readdir(pDir)) != NULL) {
		if(strcmp((pDirent->d_name), ".") == 0) {continue;}
		if(strcmp((pDirent->d_name), "..") == 0) {continue;}
		if(strcmp((pDirent->d_name), ".DS_Store") == 0) {continue;}
		std::string tname = pDirent->d_name;
		if(tname.substr(tname.find_last_of(".")) == ".mp3") {
			convertFile(tname);
		}
		tname = tname.substr(0, tname.size()-4);
		tname += ".wav";
		songs.push_back(tname);
	}
	setsound();
	std::thread bar( proBar );
	while(1) {
	part = noally;
	xpos = 1;
	ypos = 2;
	using namespace std;
	system("printf '\e[8;30;125t'");
	plr.spell[firebolt] = 1;
	plr.spell[lfirebolt] = 1; plr.spell[lfrost] = 1; plr.spell[lbolt] = 1; plr.spell[lquake] = 1;
	plr.spell[lpoison] = 1; plr.spell[llifesteal] = 1; plr.spell[lheal] = 1;
	system("clear");
	if(modchecked == 0) {
		modcheck();
	}
	if(savechecked == 0) {
		savecheck();
	}
	initscr();
	curs_set(FALSE);
	signal(SIGWINCH, NULL);
	werase(stdscr);
	mvprintw(1, 1, "This is the remake of the original Kazoo Quest.");
	mvprintw(2, 1, "This is a alpha build- expect there to be bugs.");
	mvprintw(3, 1, "Press any key to continue.");
	getch();
	pages = 4;
	endwin();
	system("clear");
	text.resize(0);

	//Temporarily disabled due to bugs issues with proBar
	//loadbar();
	Mix_PlayMusic(bgsong[0],0);
	mainm();
	}
}

void loadbar() {
	initscr();
	cbreak();
	noecho();
	clear();

	mvprintw(0, 56, "Loading Game...");
	for(i=0; i < 60; i++) {
		mvaddch(1, i + 30, ACS_CKBOARD);
		refresh();
		usleep(i*800);
	}
	erase();
	endwin();
	Mix_PlayMusic(bgsong[0],0);
	mainm();
}

void help() {
	erase();
	prints("Menu navigation:");
	prints("Use the Up/Down arrow keys to navigate menus.");
	prints("Use the Left/Right arrow keys to switch between menu pages.");
	prints("Press escape to exit out of some menus(for example, the inventory).");
	prints("Control + c immediately closes the game.");
	prints("Shift + s saves the game.");
	prints("");
	prints("Music controls:");
	prints("\"<\" goes to the previous song, \">\" goes to the next song.");
	prints("");
	prints("Press any key to close this screen");
	refresh();
	wrefresh(info);
	getch();
	erase();
	if(fmenu == 0)
		main();
	if(fmenu == 1)
		mainm();
	if(fmenu == 2)
		battle();
	if(fmenu == 3)
		prompt();
	if(fmenu == 4)
		uspell();
	if(fmenu == 5)
		inv();
}

void save() {
	int hassave = 0;
	if (FILE *file = fopen((dir + "Saves/savegame").c_str(), "r")) {
		fclose(file);
		hassave = 1;
	}
	if(hassave == 0) {
		system(("mkdir " + dir + "Saves").c_str());
	}
	//Clear save file
	std::ofstream ofs;
	ofs.open((dir + "Saves/savegame").c_str(), std::ofstream::out | std::ofstream::trunc);
	ofs.close();

	std::ofstream file;
	file.open((dir + "Saves/savegame").c_str());
	file << haspart << ",";
	file << plr.name[0] << ",";
	for(i=0;i < 7; i++)
		file << plr.stat[i] << ",";
	for(i=0; i < 4; i++)
		file << plr.xp[i] << ",";
	for(i=0; i < 8; i++)
		file << plr.spell[i] << ",";

	for(i=0; i < 3; i++)
		file << part.info[i] << ",";
	for(i=0;i < 5; i++)
		file << part.stat[i] << ",";
	for(i=0; i < 4; i++)
		file << part.xp[i] << ",";
	for(i=0; i < 2; i++)
		file << part.skill[i] << ",";
	//Always save these since they will always exist
	for(i=0; i < 3; i++)
		file << pprime.info[i] << ",";
	for(i=0;i < 5; i++)
		file << pprime.stat[i] << ",";
	for(i=0; i < 4; i++)
		file << pprime.xp[i] << ",";
	for(i=0; i < 2; i++)
		file << pprime.skill[i] << ",";

	for(i=0; i < 3; i++)
		file << pchen.info[i] << ",";
	for(i=0;i < 5; i++)
		file << pchen.stat[i] << ",";
	for(i=0; i < 4; i++)
		file << pchen.xp[i] << ",";
	for(i=0; i < 2; i++)
		file << pchen.skill[i] << ",";

	for(i=0; i < 3; i++)
		file << pverne.info[i] << ",";
	for(i=0;i < 5; i++)
		file << pverne.stat[i] << ",";
	for(i=0; i < 4; i++)
		file << pverne.xp[i] << ",";
	for(i=0; i < 2; i++)
		file << pverne.skill[i] << ",";

	file.close();
	queue.resize(0);
	queue.push_back("Game saved!");
}

void load() {
	std::string line;
	std::ifstream file;
	file.open((dir + "Saves/savegame").c_str());

	if(!file.is_open()) {
		perror("Error opening save");
		mainm();
	}
	std::string data[1000];
	int r = 0;
	int total = 0;
	while(getline(file, line, ',')) {
		data[r] = line;
		r++;
	}
	haspart = std::stoi(data[total]);
	total++;
	plr.name[0] = data[total];
	for(i=0;i < 7; i++) {
		total++;
		plr.stat[i] = std::stoi(data[total]); }
	for(i=0; i < 4; i++) {
		total++;
		plr.xp[i] = std::stoi(data[total]); }
	for(i=0; i < 8; i++){
		total++;
		plr.spell[i] = std::stoi(data[total]);}

	if(haspart == 1) {
		for(i=0; i < 3; i++){
			total++;
			part.info[i] = data[total];}
		for(i=0;i < 5; i++){
			total++;
			part.stat[i] = std::stoi(data[total]);}
		for(i=0; i < 4; i++){
			total++;
			part.xp[i] = std::stoi(data[total]);}
		for(i=0; i < 2; i++){
			total++;
			part.skill[i] = std::stoi(data[total]);}

		for(i=0; i < 3; i++){
			total++;
			pprime.info[i] = data[total];}
		for(i=0;i < 5; i++){
			total++;
			pprime.stat[i] = std::stoi(data[total]);}
		for(i=0; i < 4; i++){
			total++;
			pprime.xp[i] = std::stoi(data[total]);}
		for(i=0; i < 2; i++){
			total++;
			pprime.skill[i] = std::stoi(data[total]);}

		for(i=0; i < 3; i++){
			total++;
			pchen.info[i] = data[total];}
		for(i=0;i < 5; i++){
			total++;
			pchen.stat[i] = std::stoi(data[total]);}
		for(i=0; i < 4; i++){
			total++;
			pchen.xp[i] = std::stoi(data[total]);}
		for(i=0; i < 2; i++){
			total++;
			pchen.skill[i] = std::stoi(data[total]);}

		for(i=0; i < 3; i++){
			total++;
			pverne.info[i] = data[total];}
		for(i=0;i < 5; i++){
			total++;
			pverne.stat[i] = std::stoi(data[total]);}
		for(i=0; i < 4; i++){
			total++;
			pverne.xp[i] = std::stoi(data[total]);}
		for(i=0; i < 2; i++){
			total++;
			pverne.skill[i] = std::stoi(data[total]);}
	}
	file.close();
	queue.resize(0);
	queue.push_back("Game loaded!");
}

void keeppart() {
	//Backup partner stats
	if(part.info[0] == "Liberty Prime") {
		pprime = part;
	}
	if(part.info[0] == "Chen") {
		pchen = part;
	}
	if(part.info[0] == "Verne") {
		pverne = part;
	}
}

void mainm() {
	text.resize(0);
	text.push_back("Select a thing:");
	usr = cmenu(0, text);
	if(usr == 1) {
		battle();
	}
	if(usr == 2) {
		save();
		mainm();
	}
	if(usr == 3) {
		load();
		mainm();
	}
	if(usr == 4) {
		usr = cmenu(5, text);
		if(usr == 1) {
			text.resize(0);
			text.push_back("Are you sure?");
			usr = cmenu(9, text);
			if(usr == 1) {
				std::remove((dir + "Saves/savegame").c_str());
				prints("Restarting game...");
				main();
			} else {
				mainm();
			}
		}
		if(usr == 2) {
			keeppart();
			part = noally;
			mainm();
		}
		if(usr == 3) {
			mainm();
		}
	}
	if(usr == 5) {
		clean();
		delwin(wmenu);
		delwin(user);
		delwin(info);
		endwin();
		exit(EXIT_SUCCESS);
	}
	if(usr == 6) {
		text.resize(0);
		map();
	}
}

void battle() {
	plr.stat[1] = plr.stat[0];
	text.resize(0);
	text.push_back("Choose enemy:");
	usr = cmenu(1, text);
	if(usr == 1) {
		now = dummy;
		queue.push_back(("A " + now.info[1] + now.info[0] + " appears!"));
		prompt();
	}
	if(usr == 2) {
		clean();
		enerand = 1 + rand() % 2;
		if(enerand == 1) {
			now = wolf;
		}
		if(enerand == 2) {
			now = slime;
		}
		queue.push_back(("A " + now.info[1] + now.info[0] + " appears!"));
		prompt();
	}
	if(usr == 3) {
		now = bigslime;
		queue.push_back(("A " + now.info[1] + now.info[0] + " appears!"));
		queue.push_back("The giant slime gives itself regen!");
		prompt();
	}
	if(usr == 4) {
		clean();
		text.resize(0);
		mainm();
	}
}

void printu(std::string text) {
	//mvwprintw(user, ulines + 1, 1, text.c_str());
	mvwprintw(user, ulines + 1, 1, text.c_str());
	++ulines;
}
void printp(std::string text) {
	mvwprintw(wpart, plines + 1, 1, text.c_str());
	++plines;
}
void prints(std::string text) {
	mvwprintw(info, lines + 1, 1, text.c_str());
	++lines;
}
void printi(std::string text) {
	mvwprintw(info, lines + 1, 1, text.c_str());
	++lines;
}

void makeitems(int set) {
	clean();
	//Note to self: change how n_choices works here
	n_choices = 5;
	for(i = 0; i < n_choices; ++i)
		items[i] = new_item(inventory[page][i].c_str(), inventory[page+5][i].c_str());
	items[n_choices] = (ITEM *)NULL;
	menu = new_menu((ITEM **)items);
	cmenu(set, text);
}

void inv() {
	wmove(wmenu, 0, 0); wclrtoeol(wmenu);
	refresh();
	text.resize(0);
	text.push_back("Select item:");
	int usr = cmenu(100, text);
	if(usr == -1) {
		prompt();
	}
	if(usr == 1) {
		queue.push_back("You read the book...");
		queue.push_back("You suddenly feel smarter!");
		plr.buff[intup] = true;
		plr.times[intup] = 3;
	}
	if(usr == 4) {
		keeppart();
		if(pprime.info[0] != "") {
			part = pprime;
		} else {
			part = prime;
		}
		queue.push_back("*THUD* *THUD* *THUD* ... Must defeat Communism...");
		haspart = 1;
	}
	if(usr == 6) {
		keeppart();
		if(pchen.info[0] != "") {
			part = pchen;
		} else {
			part = chen;
		}
		queue.push_back("Out of seemingly nowhere, Chen appears!");
		haspart = 1;
	}
	if(usr == 14) {
		keeppart();
		if(pverne.info[0] != "") {
			part = pverne;
		} else {
			part = verne;
		}
		queue.push_back("Verne global when?");
		haspart = 1;
	}
	if(usr == 21) {
		if(e0.info[0] != "") {
			keeppart();
			part = e0;
			queue.push_back(e0.info[1]);
			haspart = 1;
		}
	}
	if(usr == 22) {
		if(e1.info[0] != "") {
			keeppart();
			part = e1;
			queue.push_back(e1.info[1]);
			haspart = 1;
		}
	}
	if(usr == 22) {
		if(e2.info[0] != "") {
			keeppart();
			part = e2;
			queue.push_back(e2.info[1]);
			haspart = 1;
		}
	}
	if(usr == 23) {
		if(e3.info[0] != "") {
			keeppart();
			part = e3;
			queue.push_back(e3.info[1]);
			haspart = 1;
		}
	}
	if(usr == 24) {
		if(e4.info[0] != "") {
			keeppart();
			part = e4;
			queue.push_back(e4.info[1]);
			haspart = 1;
		}
	}
	//queue.push_back("Not yet implemented!");
	prompt();
}

void prompt() {
	dam = plr.stat[2];
	agil = plr.stat[4];
	if(plr.stat[1] <= 0) {
		death();
	}
	for(i=0; i < queue.size(); i++) {
		std::string str = queue[i];
		prints(str);
	}
	text.resize(0);
	text.push_back("Enemy HP: " + std::to_string(now.stat[1]));
	plract(cmenu(2, text));
}

void enemyact() {
	dam = now.stat[2];
	int diff = dam * 20 / 100;
	dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
	agil = plr.stat[4];
	dodge = agil + rand() % (int)(10 - agil + 1);

	if(dodge == 10) {
		queue.push_back("You dodged!");
	} else if(dodge!=10) {
		//Reduce damage by player defense, but keep above 0
		dam = std::max(0, dam-plr.stat[2]);
		plr.stat[1] -= dam;
		queue.push_back("The " + now.info[0] + " dealt " + std::to_string(dam) + " damage to you!");
		if(plr.stat[1] <= 0) {
			death();
		}
	}
	if(plr.stat[1] <= 0) {
		death();
	}
	for(i=0; i < 10; i++) {
		if(plr.times[i] > 0)
			plr.times[i]--;
		if(plr.times[i] == 0)
			plr.buff[i] = false;
	}

	if(now.buff[0] == 1) {
		int hpg = now.stat[0] * 10 / 100;
		if(now.stat[1] + hpg >= now.stat[0]) {
			queue.push_back("The " + now.info[0] + "\'s HP is at max!");
			//Set HP to max
			now.stat[1] = now.stat[0];
		} else {
			queue.push_back("The " + now.info[0] + " healed " + std::to_string(hpg) + " HP!");
			now.stat[1] += hpg;
		}
	}
	prompt();
}

void glevel() {
	int diff = now.stat[0] * 20 / 100;
	int xp = now.stat[0] + diff;
	if(part.info[0] != "") {
		//Get value of excess xp
		if(xp + part.xp[0] >= part.xp[1]) {
			//Set xp equal to gained xp + current xp - needed xp (e.g- at level one, needed xp is 20: have 12xp, gain 12xp, subtract 20 = 4 left over)
			xp += part.xp[0] - part.xp[1];

			//Add a level
			part.xp[2]++;
			pup++;

			//Handling decimal values
			double temp = part.xp[2] / 2;
			part.xp[1] = 45 * temp;
			part.xp[0] = xp;

			//Partner stat changes will go here
			part.stat[0] += (plr.xp[2] * 2);
			part.stat[1] = part.stat[0];
			part.stat[2] += (plr.xp[2] * 2);
			part.stat[3] += (plr.xp[2] * 2);

			//Special stat increases
			if(part.info[0] == "Chen") {
				part.stat[4] = part.xp[2] / 2;
			}

		//Gained xp wasn't enough to level up
		} else if(xp + part.xp[0] < part.xp[1]) {
			part.xp[0] += xp;
		}
		if(xp + part.xp[0] >= part.xp[1]) {
			glevel();
		}
	}


	//Get value of excess xp
	if(xp + plr.xp[0] >= plr.xp[1]) {
		//Set xp equal to gained xp + current xp - needed xp (e.g- at level one, needed xp is 20: have 12xp, gain 12xp, subtract 20 = 4 left over)
		xp += plr.xp[0] - plr.xp[1];

		//Add a level
		plr.xp[2]++;
		uup++;

		//Handling decimal values
		double temp = plr.xp[2] / 2;
		plr.xp[1] = 45 * temp;
		plr.xp[0] = xp;

		//Player stat changes will go here
		plr.stat[0] += (plr.xp[2] * 0.8);
		plr.stat[2] += (plr.xp[2] * 0.8);
		plr.stat[3] += (plr.xp[2] * 0.8);
		plr.stat[5] += (plr.xp[2] * 0.8);
		plr.stat[6] += (plr.xp[2] * 0.8);

	//Gained xp wasn't enough to level up
	} else if(xp + plr.xp[0] < plr.xp[1]) {
		plr.xp[0] += xp;
	}
	if(xp + plr.xp[0] >= plr.xp[1]) {
		glevel();
	}
}

void enemydefeat() {
	queue.clear();
	queue.shrink_to_fit();
	queue.push_back(tempstr);
	queue.push_back("You defeated the " + now.info[0] + "!");

	//XP gained is 120% of the enemy's max HP
	int diff = now.stat[0] * 20 / 100;
	int xp = now.stat[0] + diff;
	if(part.info[0] != "") {
		queue.push_back(part.info[0] + " gained " + std::to_string(xp) + " xp!");
	}
	queue.push_back("You gained " + std::to_string(xp) + " xp!");
	pup = 0;
	uup = 0;
	//Call level up check
	glevel();
	//Pardon the weird formating
	if(pup > 0) { if(pup > 1) {
			queue.push_back(part.info[0] + " leveled up " + std::to_string(pup) + " times!");
	} else if(pup == 1) {
			queue.push_back(part.info[0] + " leveled up!");}}
	if(uup > 0) { if(uup > 1) {
			queue.push_back("You leveld up " + std::to_string(uup) + " times!");
			//lvlups = std::thread(play_sound,"Sounds/count.wav");
			Mix_PlayChannel(-1,lvlup,0);
	} else if(uup == 1) {
			queue.push_back("You leveled up!");
			//lvlups = std::thread(play_sound,"Sounds/count.wav");
			Mix_PlayChannel(-1,lvlup,0);
	}}
	werase(info);
	erase();
	for(i=0; i < queue.size(); i++) {
		std::string str = queue[i];
		printi(str);
	}
	box(info, 0, 0);
	refresh();
	wrefresh(info);
	getch();
	queue.clear();
	queue.shrink_to_fit();
	battle();
}

void plract(int usr) {
	if(usr == 1) {
		//Deals damage of random amount either 20% -/+ or equal to damage
		agil = now.stat[4];
		dodge = agil + rand() % (int)(10 - agil + 1);

		//Enemy dodged
		if(dodge == 10) {
			queue.push_back("The " + now.info[0] + " dodged!");

		//Enemy didn't dodge
		} else if(dodge != 10) {
			//Reduce damage by enemy defense, but keep above 0
			dam = std::max(-1, dam-now.stat[3]);
			now.stat[1] -= dam;
			clean();
			if(dam >= 1) {
				tempstr = "You dealt " + std::to_string(dam) + " damage to the " + now.info[0] + "!";
				queue.push_back("You dealt " + std::to_string(dam) + " damage to the " + now.info[0] + "!");
			} else {
				queue.push_back("Your attack did no damage!");
			}
		}

		if(now.stat[1] < 1) {
			enemydefeat();
		} else {
			if(part.info[0] != "") {
				allyact();
			} else {
				enemyact();
			}
		}
	}
	//Spells
	if(usr == 2) {
		uspell();
	}
	//Check enemy
	if(usr == 3) {
		queue.push_back("Enemy damage: " + std::to_string(now.stat[2]));
		queue.push_back("Enemy defense: " + std::to_string(now.stat[3]));
		queue.push_back("Enemy agility: " + std::to_string(now.stat[4]));
		//queue.push_back("Not yet implemented!");
		prompt();
	}
	//Items
	if(usr == 4) {
		inv();
	}
	//Run!
	if(usr == 5) {
		int spd = plr.stat[4];
		int run = spd + rand() % (int)(10 - spd + 1);
		if(run == 10) {
			queue.resize(0);
			queue.push_back("You ran away!");
			now = none;
			clean();
			battle();
			usr = 0;
		} else if(run != 10){
			queue.push_back("You couldn't get away!");
			enemyact();
		}
	}
}

void allyact() {
	dam = part.stat[2];
	int diff = dam * 20 / 100;
	dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
	text.resize(0);
	text.push_back("Select ally action");
	usr = cmenu(6, text);
	if(usr == 1) {
		//Deals damage of random amount either 20% -/+ or equal to damage
		agil = now.stat[3];
		dodge = agil + rand() % (int)(10 - agil + 1);

		//Enemy dodged
		if(dodge == 10) {
			queue.push_back("The " + now.info[0] + " dodged your ally's attack!");

		//Enemy didn't dodge
		} else if(dodge != 10) {
			//Reduce damage by enemy defense, but keep above 0
			dam = std::max(-1, dam-now.stat[2]);
			now.stat[1] -= dam;
			clean();
			if(dam >= 1) {
				tempstr = part.info[0] + " dealt " + std::to_string(dam) + " damage to the " + now.info[0] + "!";
				queue.push_back(part.info[0] + " " + std::to_string(dam) + " damage to the " + now.info[0] + "!");
			} else {
				queue.push_back(part.info[0] + "\'s attack did no damage!");
			}
		}

		if(now.stat[1] < 1) {
			enemydefeat();
		} else {
			enemyact();
		}
	}
	if(usr == 2) {
		queue.push_back("Not implemented yet");
		allyact();
	}
}

void uspell() {
	text.push_back("Select spell:");
	usr = cmenu(3, text);
	int didSpell = 0;
	if(usr == 1 && plr.spell[0] == 1) {
		//Deals damage of random amount either 20% -/+ or equal to damage
		dam = plr.stat[5];
		int diff = dam * 20 / 100;
		dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
		if(plr.buff[intup] == true)
			//Int buff adds extra 40% to magic attacks
			dam += (2*diff);
		now.stat[1] -= dam;
		tempstr = "You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!";
		queue.push_back("You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!");
		didSpell = 1;
	} else if(usr == 1) {
		queue.push_back("You don't have that spell!");
	}

	if(usr == 2 && plr.spell[1] == 1) {
		//Deals damage of random amount either 20% -/+ or equal to damage
		dam = plr.stat[5];
		int diff = dam * 20 / 100;
		dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
		if(plr.buff[intup] == true)
			//Int buff adds extra 40% to magic attacks
			dam += (2*diff);
		now.stat[1] -= dam;
		tempstr = "You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!";
		queue.push_back("You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!");
		didSpell = 1;
	} else if(usr == 2)  {
		queue.push_back("You don't have that spell!");
	}

	if(usr == 3 && plr.spell[2] == 1) {
		//Deals damage of random amount either 20% -/+ or equal to damage
		dam = plr.stat[5];
		int diff = dam * 20 / 100;
		dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
		if(plr.buff[intup] == true)
			//Int buff adds extra 40% to magic attacks
			dam += (2*diff);
		now.stat[1] -= dam;
		tempstr = "You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!";
		queue.push_back("You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!");
		didSpell = 1;
	} else if(usr == 3) {
		queue.push_back("You don't have that spell!");
	}

	if(usr == 4 && plr.spell[3] == 1) {
		//Deals damage of random amount either 20% -/+ or equal to damage
		dam = plr.stat[5];
		int diff = dam * 20 / 100;
		dam = (dam-diff) + rand() % (int)((dam+diff)-(dam-diff)+1);
		if(plr.buff[intup] == true)
			//Int buff adds extra 40% to magic attacks
			dam += (2*diff);
		now.stat[1] -= dam;
		tempstr = "You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!";
		queue.push_back("You dealt " + std::to_string(dam) + " to the " + now.info[0] + "!");
		didSpell = 1;
	} else if(usr == 4) {
		queue.push_back("You don't have that spell!");
	}

	if(usr == 5 && plr.spell[4] == 1) {
	} else if(usr == 5) {
		queue.push_back("You don't have that spell!");
	}

	if(usr == 6 && plr.spell[5] == 1) {
	} else if(usr == 6) {
		queue.push_back("You don't have that spell!");
	}

	if(usr == 7 && plr.spell[6] == 1) {
	} else if(usr == 7) {
		queue.push_back("You don't have that spell!");
	}

	if(now.stat[1] < 1) {
		enemydefeat();
	} else {
		if(didSpell == 1) {
			if(part.info[0] != "") {
				allyact();
			} else {
				enemyact();
			}
		} else {
			prompt();
		}
}

}

void death() {
	prints("You have died!");
	for(i=0; i < n_choices + 1; i++) {
		free_item(items[i]);
	}
	wgetch(wmenu);
	free_menu(menu);
	clean();
	delwin(wmenu);
	delwin(user);
	delwin(info);
	endwin();
	exit(EXIT_SUCCESS);
}

int cmenu(int set, std::vector<std::string> text) {
	clean();
	highlight = 1;
	ch = 0;
	if(scrn==false) {
		initscr();
		scrn=true;
		cbreak();
		noecho();
		refresh();
	}
	arrows = newwin(2, 25, 9, 0);
	wmenu = newwin(7, 100, 11, 0);
	info = newwin(9, 70, 0, 0);
	user = newwin(11, 25, 1, (col * 4 / 5));
	wpart = newwin(10, 25, 1, (col * 3 / 5));
	pname = newwin(1, 20, 0, (col * 3 / 5));
	refresh();
	keypad(wmenu, TRUE);
	//Isn't inventory call
	if(set != 100) {
		n_choices = c_choices[set];
		items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
		for(i = 0; i < n_choices; ++i)
			items[i] = new_item(choices[set][i], choices[set+10][i]);
		items[n_choices] = (ITEM *)NULL;
		menu = new_menu((ITEM **)items);
	}

	//Is inventory call
	if(set == 100) {
		fmenu = 5;
		n_choices = 5;
		items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
		for(i = 0; i < n_choices; ++i)
			items[i] = new_item(inventory[page][i].c_str(), inventory[page+5][i].c_str());
		items[n_choices] = (ITEM *)NULL;
		menu = new_menu((ITEM **)items);
	}
	if(set == 42) {
		//"North", "South", "East", "West"
		n_choices = 4;
		items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
		items[0] = new_item("North", "");
		items[1] = new_item("South", "");
		items[2] = new_item("East", "");
		items[3] = new_item("West", "");

		items[n_choices] = (ITEM *)NULL;
		menu = new_menu((ITEM **)items);
	}
	if(set == 0)
		fmenu = 1;
	if(set == 1)
		fmenu = 2;
	if(set == 2)
		fmenu = 3;
	if(set == 3)
		fmenu = 4;
	set_menu_win(menu, wmenu);
	set_menu_sub(menu, derwin(wmenu, 5, 70, 5, 0));
	set_menu_mark(menu, " > ");

	for(i=0; i < text.size(); i++) {
		std::string str = text[i];
		prints(str);
	}
	for(i=0; i < queue.size(); i++) {
		std::string str = queue[i];
		prints(str);
	}
	box(info, 0, 0);
	if(part.info[0] != "") {
		box(wpart, 0, 0);
		wprintw(pname, (part.info[0] + ":").c_str());
		printp("HP: " + std::to_string(part.stat[1]));
		printp("Attack: " + std::to_string(part.stat[2]));
		printp("Defense: " + std::to_string(part.stat[3]));
		printp("Agility: " + std::to_string(part.stat[4]));
		printp("");
		printp("Level: " + std::to_string(part.xp[2]));
		printp("EXP: " + std::to_string(part.xp[0]) + "/" + std::to_string(part.xp[1]));
	}
	if(noi == 0) {
		box(user, 0, 0);
		mvprintw(0, (col * 4 / 5) + 1, (plr.name[0] + ":").c_str());
		printu("HP: " + std::to_string(plr.stat[1]));
		printu("Mana: " + std::to_string(plr.stat[5]));
		printu("Attack: " + std::to_string(plr.stat[2]));
		printu("Intelligence: " + std::to_string(plr.stat[6]));
		printu("Defense: " + std::to_string(plr.stat[3]));
		printu("Agility: " + std::to_string(plr.stat[4]));
		printu("");
		printu("Level: " + std::to_string(plr.xp[2]));
		printu("EXP: " + std::to_string(plr.xp[0]) + "/" + std::to_string(plr.xp[1]));
	}
	post_menu(menu);
	set_escdelay(25);
	//mvprintw(28, 1, "%d, %d", (highlight + (page*5)), page);
	mvprintw(28, 100, "Press \"?\" for help");
	mvprintw(26, 1, "Currently Playing:");
	mvprintw(27, 1, "Volume:");
	if(set == 100) {
		werase(arrows);
		if(page == 0) {
			mvwprintw(arrows, 0, 20, "--->");
		} else if(page == pages) {
			mvwprintw(arrows, 0, 3, "<---");
		} else {
			mvwprintw(arrows, 0, 3, "<---             --->");
		}
	}
	refresh();
	wrefresh(wmenu);
	wrefresh(arrows);
	wrefresh(info);
	wrefresh(user);
	wrefresh(wpart);
	wrefresh(pname);
	move(27, 8); clrtoeol();
	mvprintw(27, 8, "%d%%", (vol * 100 / 128));
	move(26, 20); clrtoeol();
	int lks = loops % songs.size();
	std::string nowsong = songs[lks];
	nowsong = nowsong.substr(0, nowsong.size()-4);
	mvprintw(26, 20, nowsong.c_str());
	refresh();
	while((c = wgetch(wmenu))){
		switch(c) {
			//If user presses up arrow
			case KEY_UP:
				if(highlight == 1)
					highlight=1;
				else
					--highlight;
				menu_driver(menu, REQ_UP_ITEM);
				//mvprintw(28, 1, "%d, %d", (highlight + (page*5)), page);
				refresh();
				wrefresh(wmenu);
				break;
			//If user presses down arrow
			case KEY_DOWN:
				if(highlight == n_choices) {
					highlight=n_choices;
				} else {
					++highlight;
				}
				//mvprintw(28, 1, "%d, %d", (highlight + (page*5)), page);
				menu_driver(menu, REQ_DOWN_ITEM);
				refresh();
				wrefresh(wmenu);
				break;
			case KEY_RIGHT:
				if(set == 100) {
					if(page < pages) {
						++page;
						highlight = (page * 5) + 1;
					} else {
						page = pages;
					}
					makeitems(100);
				}
				break;
			case KEY_LEFT:
				if(set == 100) {
					if(page > 0) {
						--page;
						highlight = (page * 5) + 1;
					} else {
						page = 0;
					}
					makeitems(100);
				}
				break;
			//If user presses escape key in the menu
			case 27:
				if(set == 100)
					ch = -1;
				if(set == 42)
					ch = -1;
				break;
			//If user presses enter key
			case 10:
				ch = highlight;
				if(set==100)
					ch = highlight + (page*5);
				page = 0;
				if(set == 42) {
					test = item_name(current_item(menu));
				}
				break;
			case 60:
				loops -= 2;
				musicFinished();
				break;
			case 112:
				int paused;
				paused = 0;
				if(Mix_PausedMusic() == 1) {
					paused = 1;
				}
				if(paused == 0) {
					Mix_PauseMusic();
				}
				if(paused == 1) {
					Mix_ResumeMusic();
				}
				break;
			//">" key
			case 62:
				musicFinished();
				break;
			//User presses "?" key
			case 63:
				help();
				break;
			case 83:
				save();
				prints("Game Saved!");
				refresh();
				wrefresh(info);
				break;
			case 43:
				if(vol < 128) {
					vol++;vol++;
				} else if(vol == 128) {
					vol = 128; }
				Mix_VolumeMusic(vol);
				move(27, 8); clrtoeol();
				mvprintw(27, 8, "%d%%", (vol * 100 / 128));
				refresh();
				break;
			case 45:
				if(vol > 0) {
					vol--;vol--;
				} else if(vol == 0) {
					vol = 0; }
				Mix_VolumeMusic(vol);
				move(27, 8); clrtoeol();
				mvprintw(27, 8, "%d%%", (vol * 100 / 128));
				refresh();
				break;
			//If user presses control key
			case BUTTON_CTRL:
				//This is beautiful, works like a charm :)
				while((t = wgetch(wmenu))) {
					switch(t) {
						case 99:
							system("clear");
							free_menu(menu);
							clean();
							delwin(wmenu);
							endwin();
							exit(EXIT_SUCCESS);
							break;
						default:
							refresh();
							wrefresh(wmenu);
							break;
					}
					refresh();
					wrefresh(wmenu);
				}
				if(t != 0) {
					break;
				}
				break;
			default:
				move(27, 8); clrtoeol();
				mvprintw(27, 8, "%d%%", (vol * 100 / 128));
				refresh();
				wrefresh(wmenu);
				break;
		}
		refresh();
		wrefresh(wmenu);
	if(ch != 0)
		break;
	}
	unpost_menu(menu);
	for(i=0; i < item_count(menu); i++) {
		free_item(items[i]);
	}
	text.clear();
	text.shrink_to_fit();
	queue.clear();
	queue.shrink_to_fit();
	//free_menu(menu);
	clean();
	//isBattle = false;
	noi = 0;
	return(ch);
}

void map() {
	text.resize(0);
	text.push_back(mapa[ypos][xpos]);
	text.push_back("");
	text.push_back("North- " + mapinfo[ypos+1][xpos]);
	text.push_back("South- " + mapinfo[ypos-1][xpos]);
	text.push_back("East- " + mapinfo[ypos][xpos-1]);
	text.push_back("West- " + mapinfo[ypos][xpos+1]);
	usr = cmenu(42, text);
	if(usr == -1) {
		mainm();
	}

	if(test == "North") {
		if(mapa[ypos+1][xpos] != "") {
			ypos++;
		} else {
			queue.push_back("You can't go that way!");
		}
	}
	if(test == "South") {
		if(mapa[ypos-1][xpos] != "") {
			ypos--;
		} else {
			queue.push_back("You can't go that way!");
		}
	}
	if(test == "East") {
		if(mapa[ypos][xpos-1] != "") {
			xpos--;
		} else {
			queue.push_back("You can't go that way!");
		}
	}
	if(test == "West") {
		if(mapa[ypos][xpos+1] != "") {
			xpos++;
		} else {
			queue.push_back("You can't go that way!");
		}
	}

	map();
}

void clean() {
	//TO-DO: figure out a better alternative to this
	if(scrn==true) {
		for(i = 0; i < lines; i++) {
			wmove(info, lines, 0); clrtoeol();
		}
		for(i = 0; i < ulines; i++) {
			wmove(user, ulines, 0); clrtoeol();
		}
		for(i = 0; i < plines; i++) {
			wmove(wpart, plines, 0); clrtoeol();
		}
	}
	lines = 0;
	ulines = 0;
	plines = 0;
}

void modcheck() {
	//Need to fix this- will always be true
	if (FILE *file = fopen((dir + "Mods/extra").c_str(), "r")) {
		fclose(file);
		text.resize(0);
		text.push_back("Would you like to enable your mods?");
		noi = 1;
		usr = cmenu(9, text);
		if(usr == 1) {
			modon = true;
		}
		if(usr == 2) {
			modon = false;
		}
		noi = 0;
	}
	if(modon == true) {
		int total = -1;
		std::string line;
		std::ifstream file;
		file.open((dir + "Mods/extra").c_str());

		if(!file.is_open()) {
			perror("Error opening mod");
		}
		std::string data[1000];
		int r = 0;
		while(getline(file, line, ',')) {
			data[r] = line;
			r++;
		}
		for(i=0; i < 3; i++){
			total++;
			e0.info[i] = data[total];}
		for(i=0;i < 5; i++){
			total++;
			e0.stat[i] = std::stoi(data[total]);}
		for(i=0; i < 4; i++){
			total++;
			e0.xp[i] = std::stoi(data[total]);}
		for(i=0; i < 2; i++){
			total++;
			e0.skill[i] = std::stoi(data[total]);}
		if(r > 20) {
			for(i=0; i < 3; i++){
				total++;
				e1.info[i] = data[total];}
			for(i=0;i < 5; i++){
				total++;
				e1.stat[i] = std::stoi(data[total]);}
			for(i=0; i < 4; i++){
				total++;
				e1.xp[i] = std::stoi(data[total]);}
			for(i=0; i < 2; i++){
				total++;
				e1.skill[i] = std::stoi(data[total]);}
		}
		file.close();
	}
	usr = 0;
	modchecked = 1;
}

void getname() {
	if(newg == 1) {
		erase();
		system("clear");
		endwin();
		pprime = prime;
		pchen = chen;
		pverne = verne;
		cout << "What is your name?(Max 16 characters)\n";
		std::string in;
		getline(cin, in);
		if(in == "llama") {
			exit(EXIT_SUCCESS);}
		if(in == "Llama") {
			exit(EXIT_SUCCESS);}
		if(in.length() > 16) {
			cout << "Name too long!\n";
			getname();
		} else if(in != "") {
			plr.name[0] = in;
			savechecked1 = 1;
		}
	}
}

void savecheck() {
	if (FILE *file = fopen((dir + "Saves/savegame").c_str(), "r")) {
		fclose(file);
		text.resize(0);
		text.push_back("Would you like to load your save?");
		noi = 1;
		usr = cmenu(9, text);
		if(usr == 1) {
			load();
		}
		if(usr == 2) {
			newg = 1;
		}
	} else {
		newg = 1;
	}
	if(newg == 1) {
		if(savechecked1 == 0) {
			getname();}
	}
	savechecked = 1;
}

void getdir() {
	if(os == 2) {
		char path[1024];
		uint32_t size = sizeof(path);
		if (_NSGetExecutablePath(path, &size) == 0)
			dir = path;
	}
	dir = dir.substr(0, dir.size()-13);
}
