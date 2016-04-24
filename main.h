/*
 * main.h
 *
 *  Created on: Apr 21, 2016
 *      Author: Lunar Dust
 */

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#ifndef MAIN_H_
#define MAIN_H_

//A lot of variables
int twait;
int weapon;
//Weapon list: 0 hands, 1 branch, 2 dagger, 3 dull sword, 4 Blade Staff, 5 sharp spear, 6 polished axe, 7 The Blade of Honking
int armor;
//Armor list: 0 Cloth shirt, 1 Leather Breastplate, 2 Chainmail Breastplate, 3 Scale Breastplate, 4 Crystal Breastplate, 5 Cloak of Shadows, 6 Magic Shield, 7 Kazoo Shield of Death
int min_dam;
int max_dam;
int dodges;
int dodge_act;
int damage;
int max_hp;
int max_mana;
int level;
int max_level;
std::string skills;
std::string skills_thing;
int skill_energy;
int max_energy;
std::string spells;
std::string spells_thing;
int exp;
int evolve_count;
int points;
int game_diff;
int classsc;
int boss;
int bossattackchoice;
std::string triggers;
std::string inventory;
//Words that will be checked for later
std::string n_words [] = {"n", "north"};
std::string s_words [] = {"s", "south"};
std::string e_words [] = {"e", "east"};
std::string w_words [] = {"w", "west"};
std::string u_words [] = {"u", "up"};
std::string d_words [] = {"d", "down"};
std::string yes_words [] = {"yes", "y", "true", "indeed", "yeah", "afirmative"};
std::string lights_words [] = {"switch", "lights", "light", "torch"};
std::string spellbook_words [] = {"spellbook", "book", "runebook"};
std::string take_words [] = {"take", "grab", "pick", "get", "aquire", "nab", "steal"};
std::string use_words[] = {"use", "eat", "read", "drink", "flip", "turn", "hit"};
int stop;
int enemy_set;
//Time removed in v0.1.4 (Re-implementation being tested in v0.3) (Completely broken, but I decided to leave the code in)
//time 0
int encounter_time;
int skip;
std::string enemy_type;
int enemy_dam;
int enemy_dodge;
std::string enemy_buffs;
std::string enemy_debuffs;
int enemy_buff_timer;
int enemy_debuff_timer;
std::string enemy_info;
std::string enemy_dam_info;
int hp;
int def;
int mana;
int var_set;
std::string player_buffs;
std::string player_debuffs;
int player_buff_timer;
int player_debuff_timer;
int firebolt_level;
int frost_level;
int poison_level;
int lifesteal_level;
int bolt_level;
int quake_level;
int heal_level;
int stun_level;
int exp_limit;
std::string kills;
int encounter;
std::string history;
int tut_finished;
int loadyload;
std::string dothing;
int acted;

#endif /* MAIN_H_ */
