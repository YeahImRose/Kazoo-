/*
 * allies.h
 *
 *  Created on: May 3, 2016
 *      Author: 1800887
 */

#ifndef ALLIES_H_
#define ALLIES_H_

/*
 * struct ally
	std::string info[3]; Name, info, skill
	int stat[4]; Max HP, current HP, damage, defense, agility
	int xp[4]; Current xp, xp needed for next level, current level, max level
	int skill[2]; Has skill, skill level
	<Buffs/timers>
 */

int pcount = 3;

ally prime = {{"Liberty Prime", "The most patriotic robot ever", "Nukes"}, {40, 40, 10, 1, 0}, {0, 25, 1, 15}};
ally chen = {{"Chen", "Nyaaa~", "Spellcasting"}, {25, 25, 20, 2, 6}, {0, 25, 1, 15}};
ally verne = {{"Verne", "Verne", "Verne-ness"}, {30, 30, 10, 1, 1}, {0, 25, 1, 15}};


#endif /* ALLIES_H_ */
