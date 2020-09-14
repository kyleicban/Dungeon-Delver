#include <iostream>
#include <string>
#include "Dungeon.h"
#include "Player.h"
#include "Monster.h"
#include "utilities.h"
using namespace std;

Dungeon::Dungeon(int level) : m_level(level)
{
	m_player = nullptr;
	n_monsters = 0;
}

Dungeon::~Dungeon()
{
	for (int i = 0; i < n_monsters; i++) delete m_monster[i];
	delete m_player;
}

void Dungeon::display(char ch)
{
	//Actors do something
	string playerAction = m_player->doSomething(ch);
	string monstersActions[MAXMONSTERS];
	for (int i = 0; i < n_monsters; i++)
		monstersActions[i] = m_monster[i]->doSomething();

	//Copy the Dungeon grid //Print Weapons and Scrolls
	char displayGrid[MAXROWS][MAXCOLS];
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			if (m_grid[i][j] >= 1 && m_grid[i][j] <= 5) displayGrid[i][j] = WEAPON;
			else if (m_grid[i][j] >= 6 && m_grid[i][j] <= 10) displayGrid[i][j] = SCROLL;
			else displayGrid[i][j] = m_grid[i][j];
		}
	}

	//Print Player
	if (m_player != nullptr) displayGrid[m_player->row() - 1][m_player->col() - 1] = '@';

	//Print Monsters
	for (int i = 0; i < n_monsters; i++)
	{
		if (dynamic_cast<Goblin*>(m_monster[i]) != nullptr) 
			displayGrid[m_monster[i]->row() - 1][m_monster[i]->col() - 1] = 'G';
		else if (dynamic_cast<Snakewoman*>(m_monster[i]) != nullptr)
			displayGrid[m_monster[i]->row() - 1][m_monster[i]->col() - 1] = 'S';
		else if (dynamic_cast<Bogeyman*>(m_monster[i]) != nullptr)
			displayGrid[m_monster[i]->row() - 1][m_monster[i]->col() - 1] = 'B';
		else if (dynamic_cast<Dragon*>(m_monster[i]) != nullptr)
			displayGrid[m_monster[i]->row() - 1][m_monster[i]->col() - 1] = 'D';
	}

	//Cout
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++) cout << displayGrid[i][j];
		cout << endl;
	}

	//Stats bar
	if (m_player != nullptr)
	{
		if (m_player->isDead()) m_player->setHealthZero();
		cout << "Dungeon Level: " << m_level << ", Hit points: " << m_player->hp()
			<< ", Armor: " << m_player->armor() << ", Strength: " << m_player->strength()
			<< ", Dexterity: " << m_player->dexterity() << endl << endl;
	}

	//Action Dialogue
	if (playerAction != "") cout << playerAction << endl;
	for (int i = 0; i < n_monsters; i++)
		if (monstersActions[i] != "")	cout << monstersActions[i] << endl;
}

void Dungeon::setGrid() //create the initial display for new dungeon levels
{
	//Delete the previous monsters to reset the grid
	for (int i = 0; i < n_monsters; i++) delete m_monster[i];
	n_monsters = 0;

	int r, c;

	//fill grid with walls
	for (r = 0; r < MAXROWS; r++)
		for (c = 0; c < MAXCOLS; c++)
			m_grid[r][c] = WALL;

	//create rooms
	int numOfRooms = randInt(4, 7);
	createNRooms(numOfRooms);

	//add player if not already, otherwise move the same player to a different part of the new dungeon
	if (m_player == nullptr)
	{
		do
		{
			r = randInt(1, MAXROWS);
			c = randInt(1, MAXCOLS);
		} while (!addPlayer(r, c));
	}
	else
	{
		do
		{
			r = randInt(1, MAXROWS);
			c = randInt(1, MAXCOLS);
		} while (!(m_player->teleport(r, c)));
	}

	//determine what monsters to include
	int rangeOfMonsters = SNAKEWOMAN;
	if (m_level == 3) rangeOfMonsters = BOGEYMAN;
	if (m_level > 4) rangeOfMonsters = DRAGON;

	//adding monsters
	int numOfMonsters = randInt(2, 5 * (m_level + 2) + 1);
	for (int i = 0; i < numOfMonsters; i++)
	{
		int ID = randInt(GOBLIN, rangeOfMonsters);
		do
		{
			r = randInt(1, MAXROWS);
			c = randInt(1, MAXCOLS);
		} while (!addMonster(ID, r, c));
	}

	//place stairs or idol
	do
	{
		r = randInt(1, MAXROWS);
		c = randInt(1, MAXCOLS);
	} while (m_grid[r - 1][c - 1] != EMPTY);
	if (m_level < 4) m_grid[r - 1][c - 1] = STAIRS;
	else m_grid[r - 1][c - 1] = IDOL;

	//place the objects
	int n_objects = randInt(2, 3);
	for (int i = 0; i < n_objects; i++)
	{
		do
		{
			r = randInt(1, MAXROWS);
			c = randInt(1, MAXCOLS);
		} while (m_grid[r - 1][c - 1] != EMPTY);
		int objectype = randInt(1, 2);
		if (objectype == 1) 
			m_grid[r - 1][c - 1] = randInt(1, 3);
		else m_grid[r - 1][c - 1] = randInt(7, 10);
	}
}

bool Dungeon::setCellStatus(char status, int r, int c) //used to pick up and place objects
{
	if (!isInBounds(r, c)) return false;

	if (!isValidCellSatus(getCellStatus(r, c)) || !isValidCellSatus(status)) return false;
	
	if (status == getCellStatus(r, c)) return false;

	m_grid[r - 1][c - 1] = status;
	return true;
}

bool Dungeon::addPlayer(int r, int c)
{
	if (!isInBounds(r, c)) return false; //within the grid

	if (m_grid[r - 1][c - 1] == WALL) return false; //not in a wall

	if (m_player != nullptr) return false; //not already exist

	for (int i = 0; i < n_monsters; i++) //not on a monster
		if (m_monster[i]->row() == r && m_monster[i]->col() == c) return false;

	m_player = new Player(this, r, c);
	return true;
}

bool Dungeon::addMonster(int ID, int r, int c)
{
	if (ID < GOBLIN || ID > DRAGON) return false; //not an unknown monster

	if (!isInBounds(r, c)) return false; //...

	if (m_grid[r - 1][c - 1] == WALL) return false;

	if (m_player->row() == r && m_player->col() == c) return false; //not on the player

	for (int i = 0; i < n_monsters; i++)
		if (m_monster[i]->row() == r && m_monster[i]->col() == c) return false;

	m_monster[n_monsters] = monsterID(ID, r, c); //ID represents the unique monster //see globals.h
	n_monsters++;
	return true;
}

void Dungeon::deleteMonster(int i)
{
	if (i >= 0 && i < n_monsters)
	{
		delete m_monster[i];
		m_monster[i] = m_monster[n_monsters - 1]; //order not necessary
		n_monsters--;
	}
}

bool Dungeon::isValidRoom(int r, int c, int dr, int dc) //helper to create the dungeon rooms
{
	if (r <= 1 || c <= 1) return false;
	if (r + dr > MAXROWS || c + dc > MAXCOLS) return false;
	for (int i = r - 1; i <= r + dr; i++)
		if (m_grid[i][c - 1] != WALL || m_grid[i][c + dc] != WALL) return false;
	for (int i = c - 1; i <= c + dc; i++)
		if (m_grid[r - 1][i] != WALL || m_grid[r + dr][i] != WALL) return false;
	return true;
}

bool Dungeon::closeEnough(int r, int c, int dr, int dc, char dir) //makes sure the rooms corridors aren't too long
{
	for (int i = 1; i <= 5; i++)
	{
		switch (dir)
		{
		case ARROW_UP:
			if (r - dr - i > 1 && m_grid[r - dr - i - 1][c - 1] == EMPTY) return true;
			break;
		case ARROW_DOWN:
			if (r + dr + i < MAXROWS && m_grid[r + dr + i - 1][c - 1] == EMPTY)	return true;
			break;
		case ARROW_LEFT:
			if (c - dc - i > 1 && m_grid[r - 1][c - dc - i - 1] == EMPTY)	return true;
			break;
		case ARROW_RIGHT:
			if (c + dc + i < MAXCOLS && m_grid[r - 1][c + dc + i - 1] == EMPTY)	return true;
			break;
		default:
			break;
		}
	}
	return false;
}

void Dungeon::createRoom(int r, int c, int dr, int dc) //creates the room
{
	if (isValidRoom(r, c, dr, dc))
	{
		for (int i = r; i < r + dr; i++)
			for (int j = c; j < c + dc; j++)
				m_grid[i - 1][j - 1] = EMPTY;
	}
}

void Dungeon::createNRooms(int n) //creates multiple rooms
{
	if (n <= 0) return;
	int r, c, dr, dc;
	if (n == 1)
	{
		do
		{
			r = randInt(2, MAXROWS - 1);
			c = randInt(2, MAXCOLS - 1);
			dr = randInt(5, 10);
			dc = randInt(7, 13);
		} while (!isValidRoom(r, c, dr, dc));
		createRoom(r, c, dr, dc);
		return;
	}
	createNRooms(n - 1);
	int h_r, h_c, m_r, m_c;
	do
	{
		r = randInt(2, MAXROWS - 1);
		c = randInt(2, MAXCOLS - 1);
		dr = randInt(5, 10);
		dc = randInt(7, 13);
		h_r = dr / 2;
		h_c = dc / 2;
		m_r = r + h_r;
		m_c = c + h_c;
	} while (!(isValidRoom(r, c, dr, dc) && 
		(closeEnough(m_r, m_c, h_r, h_c, ARROW_UP) || closeEnough(m_r, m_c, h_r, h_c, ARROW_DOWN) ||
			closeEnough(m_r, m_c, h_r, h_c, ARROW_LEFT) || closeEnough(m_r, m_c, h_r, h_c, ARROW_RIGHT))));

	createRoom(r, c, dr, dc);

	if (closeEnough(m_r, m_c, h_r, h_c, ARROW_UP))
		for (int i = 1; i <= 5; i++)
			if (m_grid[m_r - h_r - i - 1][m_c - 1] != EMPTY) m_grid[m_r - h_r - i - 1][m_c - 1] = EMPTY;
	if (closeEnough(m_r, m_c, h_r, h_c, ARROW_DOWN)) 
		for (int i = 0; i <= 5; i++) 
			if (m_grid[m_r + h_r + i - 1][m_c - 1] != EMPTY) m_grid[m_r + h_r + i - 1][m_c - 1] = EMPTY;
	if (closeEnough(m_r, m_c, h_r, h_c, ARROW_LEFT)) 
		for (int i = 1; i <= 5; i++) 
			if (m_grid[m_r - 1][m_c - h_c - i - 1] != EMPTY) m_grid[m_r - 1][m_c - h_c - i - 1] = EMPTY;
	if (closeEnough(m_r, m_c, h_r, h_c, ARROW_RIGHT)) 
		for (int i = 0; i <= 5; i++) 
			if (m_grid[m_r - 1][m_c + h_c + i - 1] != EMPTY) m_grid[m_r - 1][m_c + h_c + i - 1] = EMPTY;
}

bool Dungeon::isValidCellSatus(char ch)
{
	return ch == EMPTY || (ch >= 1 && ch <= 10);
}

Monster* Dungeon::monsterID(int ID, int r, int c) //transforms numbers into monsters
{
	switch (ID)
	{
	case GOBLIN: return new Goblin(this, r, c, m_player, 10);
	case SNAKEWOMAN: return new Snakewoman(this, r, c, m_player, 3);
	case BOGEYMAN: return new Bogeyman(this, r, c, m_player, 5);
	case DRAGON: return new Dragon(this, r, c, m_player, 0);
	default: return nullptr;
	}
	return nullptr;
}