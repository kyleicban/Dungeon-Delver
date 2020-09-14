#include <iostream>
using namespace std; 

#include "Actor.h"
#include "Player.h"
#include "Monster.h"
#include "Dungeon.h"
#include "globals.h"

void Actor::goToSleep() 
{ 
	int temp = randInt(2, 6); 	
	if (temp > m_sleep) m_sleep = temp; 
}

bool Actor::canMove(int r, int c)
{
	if (m_dungeon->getCellStatus(r, c) == WALL) return false;
	if ((m_dungeon->player())->row() == r && (m_dungeon->player())->col() == c) return false;
	for (int i = 0; i < m_dungeon->numMonsters(); i++)
		if ((m_dungeon->ithMonster(i))->row() == r && (m_dungeon->ithMonster(i))->col() == c) return false;
	return true;
}