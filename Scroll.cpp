#include <iostream>
#include <string>
#include "Scroll.h"
#include "Player.h"
#include "utilities.h"
using namespace std;

string Teleportation::performMagic()
{
	int r, c;
	do
	{
		r = randInt(1, MAXROWS);
		c = randInt(1, MAXCOLS);
	} while (!isInBounds(r, c));
	player()->teleport(r, c);
	return "You teleported away.";
}

string Armor::performMagic()
{
	int i = randInt(1, 3);
	player()->increaseArmor(i);
	return "You become more formidable.";
}

string Strength::performMagic()
{
	int i = randInt(1, 3);
	player()->increaseStrength(i);
	return "Your muscles buldge.";
}

string Health::performMagic()
{
	int i = randInt(3, 8);
	player()->increaseHealth(i);
	return "Your heart pumps stronger.";
}

string Dexterity::performMagic()
{
	player()->increaseDexterity(1);
	return "Your lungs expand further.";
}