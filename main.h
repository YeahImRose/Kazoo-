/*
 * main.h
 *
 *  Created on: Apr 21, 2016
 *      Author: Lunar Dust
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <cstdlib>
#include <cstdint>
//Planning on eventually adding sounds
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>

#define row 30
#define col 125

//Weapon list: 0 hands, 1 branch, 2 dagger, 3 dull sword, 4 Blade Staff, 5 sharp spear, 6 polished axe, 7 The Blade of Honking
int weapon;
//Armor list: 0 Cloth shirt, 1 Leather Breastplate, 2 Chainmail Breastplate, 3 Scale Breastplate, 4 Crystal Breastplate, 5 Cloak of Shadows, 6 Magic Shield, 7 Kazoo Shield of Death
int armor;
//Game variables
int points, difficulty, classsc, usr, tut_finished;
//Battle variables
int agil, dodge, dam, enerand;
//Menu variables + some
int highlight, n_choices, i, c, ch, t, noi, fmenu, xpos, ypos, pages, ynow, xnow;
int ulines = 0, plines = 0, lines = 0, page = 0, pup = 0, uup = 0, modchecked = 0, haspart = 0, newg = 0, savechecked = 0, savechecked1 = 0, loops=0;
int vol = 64;
int c_choices[10] = {6, 4, 5, 7, 4, 3, 2, 0, 0, 2};
bool scrn = false;
bool modon;

std::string dir;
std::string triggers[25];
std::string test;
std::string kills[100];
std::string tempstr;
std::vector<std::string> text, queue, songs;

struct player {
	std::string name[1];
	int stat[7]; //Max HP, current HP, attack, defense, agility, mana, intelligence
	int xp[4]; //Current xp, xp needed for next level, current level, max level
	int spell[16]; //0-7= Has: firebolt, frost, poison, lifesteal, bolt, quake, heal, stun/ 8-15=respective spell levels
	bool buff[10]; //Regen, def up, atk up, agi up, int up, poison, def down, atk down, agi down, int down, paralysis/frozen
	int times[10]; //Buff timers for regen, def up, atk up, agi up, int up, poison, def down, atk down, agi down, int down, paralysis/frozen
};

struct ally {
	std::string info[3]; //Name, info, skill
	int stat[5]; //Max HP, current HP, damage, defense, agility
	int xp[4]; //Current xp, xp needed for next level, current level, max level
	int skill[2]; //Has skill, skill level
	bool buff[10]; //Regen, def up, atk up, agi up, int up, poison, def down, atk down, agi down, int down, paralysis/frozen
	int times[10]; //Buff timers for regen, def up, atk up, agi up, int up, poison, def down, atk down, agi down, int down, paralysis/frozen
} part, pchen, pprime, pverne, e0, e1, e2, e3, e4;

struct enemy {
	std::string info[2]; //Name, info
	int stat[5]; //Max HP, current HP, damage, defense, agility
	bool buff[10]; //Regen, def up, atk up, agi up, int up, poison, def down, atk down, agi down, int down, paralysis/frozen
	int times[10]; //Buff timers for regen, def up, atk up, agi up, int up, poison, def down, atk down, agi down, int down, paralysis/frozen
} now;

typedef struct  WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         Subchunk2ID[4]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;

enum statusBuff {
	regen=0,
	defup=1,
	atkup=2,
	agiup=3,
	intup=4,
	psn=5,
	defdn=6,
	atkdn=7,
	agidn=8,
	intdwn=9,
	frzn=10,
};

enum spellEnum {
	firebolt=0,
	frost=1,
	bolt=2,
	quake=3,
	poison=4,
	lifesteal=5,
	heal=6,
	lfirebolt=7,
	lfrost=8,
	lbolt=9,
	lquake=10,
	lpoison=11,
	llifesteal=12,
	lheal=13
};

//Reminder to set colors for different OS's here

int cmenu(int, std::vector<std::string>);
void makeitems(int);
void mainm();
void clean();
void battle();
void inv();
void uspell();
void printu(std::string);
void printp(std::string);
void prints(std::string);
void printi(std::string);
void plract(int);
void enemyact();
void allyact();
void enemydefeat();
void keeppart();
void save();
void savecheck();
void getname();
void modcheck();
void getdir();
void load();
void resize();
void help();
void map();
void prompt();
void death();
void play_sound(std::string);
void checkaudio();
void loadbar();

#endif /* MAIN_H_ */
