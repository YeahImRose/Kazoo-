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
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
//Planning on eventually adding sounds
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>

//Weapon list: 0 hands, 1 branch, 2 dagger, 3 dull sword, 4 Blade Staff, 5 sharp spear, 6 polished axe, 7 The Blade of Honking
int weapon;
//Armor list: 0 Cloth shirt, 1 Leather Breastplate, 2 Chainmail Breastplate, 3 Scale Breastplate, 4 Crystal Breastplate, 5 Cloak of Shadows, 6 Magic Shield, 7 Kazoo Shield of Death
int armor;
//Game variables
int points, difficulty, classsc, usr, tut_finished;
//Battle variables
int agil, dodge, dam, enerand, haspart = 0;
//Menu variables + some
int highlight, n_choices, i, c, ch, t, noi, fmenu, xpos, ypos, pages, row, col;
int ulines = 0, plines = 0, lines = 0;
int page = 0;
bool scrn = false;

std::string triggers[25];
/*std::string yes_words [] = {"yes", "y", "true", "indeed", "yeah", "afirmative"};
std::string lights_words [] = {"switch", "lights", "light", "torch"};
std::string spellbook_words [] = {"spellbook", "book", "runebook"};
std::string take_words [] = {"take", "grab", "pick", "get", "aquire", "nab", "steal"};
std::string use_words[] = {"use", "eat", "read", "drink", "flip", "turn", "hit"};*/
std::string kills[100];
std::string tempstr;
std::string lastdir;
std::vector<std::string> text, queue;

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
} part, pchen, pprime, pverne;

struct enemy {
	std::string info[2]; //Name, info
	int stat[5]; //Max HP, current HP, damage, defense, agility
	bool buff[10]; //Regen, def up, atk up, agi up, int up, poison, def down, atk down, agi down, int down, paralysis/frozen
	int times[10]; //Buff timers for regen, def up, atk up, agi up, int up, poison, def down, atk down, agi down, int down, paralysis/frozen
} now;

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

enum allies {
	aprime=0,
	anep,
	averne,
	achen
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
void uspell();
void clean();
void battle();
void prompt();
void death();
void plract(int);
void enemyact();
void allyact();
void printu(std::string);
void printp(std::string);
void prints(std::string);
void printi(std::string);
void enemydefeat();
void mainm();
void keeppart();
void save();
void load();
void resize();
void help();
void inv();
void makeitems(int);
void map();

#endif /* MAIN_H_ */
