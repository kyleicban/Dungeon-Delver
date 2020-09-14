#include <iostream>
#include <string>
#include <cmath>
#include "Monster.h"
#include "Dungeon.h"
#include "Weapon.h"
#include "Player.h"
#include "utilities.h"
using namespace std;

string Monster::doSomething()
{
	if (m_sleep > 0) // if its asleep
	{
		m_sleep--;
		return "";
	}
	if (withinAttackingDistance()) return attack(); //if its right next to the player
	if (!withinPursuingDistance()) return ""; //if its too far away
	move(chooseDirection()); //otherwise move to the player
	return "";
}

void Monster::move(char dir)
{
	switch (dir)
	{
	case ARROW_UP: if (canMove(m_row - 1, m_col)) m_row--; break;
	case ARROW_DOWN: if (canMove(m_row + 1, m_col)) m_row++; break;
	case ARROW_LEFT: if (canMove(m_row, m_col - 1)) m_col--; break;
	case ARROW_RIGHT: if (canMove(m_row, m_col + 1)) m_col++; break;
	default: break;
	}
}

char Monster::chooseDirection() //monsters will travel vertically before horizontally
{
	if (!withinPursuingDistance()) return ' ';
	if (m_player->row() < m_row && canMove(m_row - 1, m_col)) return ARROW_UP; //if the Player is above it
	else if (m_player->row() > m_row && canMove(m_row + 1, m_col)) return ARROW_DOWN; //below
	else if (m_player->col() < m_col && canMove(m_row, m_col - 1)) return ARROW_LEFT; //left
	else if (m_player->col() > m_col && canMove(m_row, m_col + 1)) return ARROW_RIGHT; //right
	else return ' ';
}

string Monster::attack() //attach the player
{
	if (m_sleep > 0) { m_sleep--; return ""; }
	string msg = "";
	if ((m_player->row() == row() + 1 && m_player->col() == col()) ||
		(m_player->row() == row() - 1 && m_player->col() == col()) ||
		(m_player->row() == row() && m_player->col() == col() + 1) ||
		(m_player->row() == row() && m_player->col() == col() - 1))
	{
		int attackerDexterity = dexterity() + m_weapon->dexterity(); //determines if the attack will hit
		int defenderDexterity = m_player->dexterity() +
			(m_player->weapon())->dexterity();


		if (randInt(1, attackerDexterity) >= randInt(1, defenderDexterity)) //return the appropriate message
		{
			int damage = m_strength + m_weapon->damage();
			m_player->takeDamage(damage);
			if (m_player->isDead())
			{
				msg += "The " + name() + " " + m_weapon->action() + " at the " + m_player->name() + " dealing a lethal blow";
			}
			else msg += "The " + name() + " " + m_weapon->action() + " at the " + m_player->name() + " and hits";

			if (!(m_player->isDead()) && dynamic_cast<MagicFangs*>(m_weapon) != nullptr) //if you're weilding fangs
			{
				if (randInt(1, 5) == 1)
				{
					m_player->goToSleep();
					msg += " causing it to sleep";
				}
			}
		}
		else msg += "The " + name() + " " + m_weapon->action() + " at the " + m_player->name() + " and missed";
	}
	return msg;
}

bool Monster::withinPursuingDistance() const //ignoring walls
{
	int verDist = abs(m_row - m_player->row());
	int horDist = abs(m_col - m_player->col());
	return m_smellingDistance >= verDist + horDist;
}

bool Monster::withinAttackingDistance() const
{
	int verDist = abs(m_row - m_player->row());
	int horDist = abs(m_col - m_player->col());
	return verDist + horDist == 1;
}

Bogeyman::Bogeyman(Dungeon* dungeon, int r, int c, Player* player, int smellingDistance) : Monster(dungeon, r, c, player, 5)
{
	m_hp = randInt(5, 10);
	mx_hp = 10;
	m_weapon = new ShortSword();
	m_strength = randInt(2, 3);
	m_dexterity = randInt(2, 3);
	m_armor = 2;
}

Bogeyman::~Bogeyman()
{
	if (dungeon()->getCellStatus(row(), col()) == EMPTY)
	{
		int chance = randInt(1, 10);
		if (chance == 1) dungeon()->setCellStatus(4, row(), col());
	}
}

Snakewoman::Snakewoman(Dungeon* dungeon, int r, int c, Player* player, int smellingDistance) : Monster(dungeon, r, c, player, 3)
{
	m_hp = randInt(3, 6);
	mx_hp = 6;
	m_weapon = new MagicFangs();
	m_strength = 2;
	m_dexterity = 3;
	m_armor = 3;
}

Snakewoman::~Snakewoman()
{
	if (dungeon()->getCellStatus(row(), col()) == EMPTY)
	{
		int chance = randInt(1, 3);
		if (chance == 1) dungeon()->setCellStatus(5, row(), col());
	}
}

Dragon::Dragon(Dungeon* dungeon, int r, int c, Player* player, int smellingDistance) : Monster(dungeon, r, c, player, 0)
{
	m_hp = randInt(20, 25);
	mx_hp = 25;
	m_weapon = new LongSword();
	m_strength = 4;
	m_dexterity = 4;
	m_armor = 4;
}

Dragon::~Dragon()
{
	if (dungeon()->getCellStatus(row(), col()) == EMPTY)
	{
		dungeon()->setCellStatus(randInt(6, 10), row(), col());
	}
}

string Dragon::doSomething() //dragons can heal...
{
	if (m_sleep > 0)
	{
		m_sleep--;
		return "";
	}
	int heal = randInt(1, 10);
	if (m_hp < mx_hp && heal == 1) m_hp++;
	if (withinAttackingDistance()) return attack();
	return "";
}

Goblin::Goblin(Dungeon* dungeon, int r, int c, Player* player, int smellingDistance) : Monster(dungeon, r, c, player, 15)
{
	m_hp = randInt(15, 20);
	mx_hp = 20;
	m_weapon = new ShortSword();
	m_strength = 2;
	m_dexterity = 1;
	m_armor = 1;
}

Goblin::~Goblin()
{
	if (dungeon()->getCellStatus(row(), col()) == EMPTY)
	{
		int chance = randInt(1, 3);
		if (chance == 1) dungeon()->setCellStatus(randInt(4, 5), row(), col());
	}
}

char Goblin::chooseDirection(int dummy) //uses recursion to choose optimal pathway
{
	if(!withinPursuingDistance()) return ' ';
	char pathGrid[MAXROWS][MAXCOLS];
	for (int i = 0; i < MAXROWS; i++)
		for (int j = 0; j < MAXCOLS; j++)
			pathGrid[i][j] = m_dungeon->getCellStatus(i, j);
	if (pathExists(pathGrid, m_row, m_col, m_player->row(), m_player->col(), 8))
	{
		if (pathGrid[m_row - 2][m_col - 1] == 'x') return ARROW_UP;
		else if (pathGrid[m_row][m_col - 1] == 'x') return ARROW_DOWN;
		else if (pathGrid[m_row - 1][m_col - 2] == 'x') return ARROW_LEFT;
		else if (pathGrid[m_row - 1][m_col] == 'x') return ARROW_RIGHT;
	}
	return ' ';
}

bool Goblin::pathExists(char grid[][MAXCOLS], int sr, int sc, int er, int ec, int iterations)
{
	//if a path exists record it on grid[][] with an 'x'
	//follow the x's
	if (iterations <= 0) return false;
	if (sr == er && sc == ec) return true;
	if (canMove(sr - 1, sc) && grid[sr - 2][sc - 1] != 'x') //for example, if you can move up...
	{
		grid[sr - 1][sc - 1] = 'x'; //set your current spot to x
									//if there is not a path from here, make it EMPTY again
		if (!pathExists(grid, sr - 1, sc, er, ec, iterations - 1)) grid[sr - 1][sc - 1] = EMPTY; 
		else return true;
	}
	if (canMove(sr + 1, sc) && grid[sr][sc - 1] != 'x')
	{
		grid[sr - 1][sc - 1] = 'x';
		if (!pathExists(grid, sr + 1, sc, er, ec, iterations - 1)) grid[sr - 1][sc - 1] = EMPTY;
		else return true;
	}
	if (canMove(sr, sc - 1) && grid[sr - 1][sc - 2] != 'x')
	{
		grid[sr - 1][sc - 1] = 'x';
		if (!pathExists(grid, sr, sc - 1, er, ec, iterations - 1)) grid[sr - 1][sc - 1] = EMPTY;
		else return true;
	}
	if (canMove(sr, sc + 1) && grid[sr - 1][sc] != 'x')
	{
		grid[sr - 1][sc - 1] = 'x';
		if (!pathExists(grid, sr, sc + 1, er, ec, iterations - 1)) grid[sr - 1][sc - 1] = EMPTY;
		else return true;

	}
	return false;
}