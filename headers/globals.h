#ifndef GLOBALS_H
#define GLOBALS_H

class Object;

const int MAXROWS = 18;
const int MAXCOLS = 70;

const int MAXMONSTERS = 31;

const char WALL = '#';
const char EMPTY = ' ';
const char WEAPON = ')';
const char SCROLL = '?';
const char GRAB = 'g';
const char WIELD = 'w';
const char READ = 'r';
const char INVENTORY = 'i';
const char STAIRS = '>';
const char IDOL = '&';
const char CHEAT = 'c';

//unique number for objects and monsters //used for randomization
const int MACE = 1;
const int SHSW = 2;
const int LGSW = 3;
const int AXE = 4;
const int FANGS = 5;
const int TELEPORTATION = 6;
const int ARMOR = 7;
const int STRENGTH = 8;
const int HEALTH = 9;
const int DEXTERITY = 10;

const int GOBLIN = 11;
const int SNAKEWOMAN = 12;
const int BOGEYMAN = 13;
const int DRAGON = 14;

bool isInBounds(int r, int c);

#endif // !GLOBALS_H
