#ifndef ENEMIES_H_
#define ENEMIES_H_
#include "main.h"

//Name, info, ||||| Max HP, current HP, damage, defense, agility ||||| Regen, def up, atk up, agi up, counter, stealth(dodge up),
//poison, def down, atk down, agi down, paralysis/frozen, dodge down ||||| Buff timers for respective buff
enemy none = {{ "", "" }, {0, 0, 0, 0, 0}};
enemy dummy = {{ "dummy", "slightly angry" }, {1000, 1000, 1, 1, 0}};

enemy wolf = {{ "wolf", "fodder " }, {15, 15, 3, 2, 1}};
enemy slime = {{"slime", "*boing*'ing "}, {10, 10, 2, 3, 1}};

enemy bigslime = {{"giant slime", "*BOING BOING*'ing"}, {50, 50, 5, 5, 2}, {1}};


#endif /* ENEMIES_H_ */
