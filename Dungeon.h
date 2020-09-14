#ifndef DUNGEON_H
#define DUNGEON_H

#include "globals.h"

class Player;
class Monster;

class Dungeon
{
public:
	Dungeon(int level);
	~Dungeon();

	//Accessors
	int level() const { return m_level; }
	Player* player() const { return m_player; }
	char getCellStatus(int r, int c) const { return m_grid[r - 1][c - 1]; }
	int numMonsters() const { return n_monsters; }
	Monster* ithMonster(int i) const { if (i >= 0 && i < n_monsters) return m_monster[i]; return nullptr; }

	void display(char ch);
	void setGrid();
	bool setCellStatus(char status, int r, int c);

	void nextLevel() { m_level++; }
	bool addPlayer(int r, int c);
	bool addMonster(int ID, int r, int c);	
	void deleteMonster(int i);

private:
	char m_grid[MAXROWS][MAXCOLS];
	int m_level;
	Player* m_player;
	Monster* m_monster[MAXMONSTERS];
	int n_monsters;
	
	bool isValidRoom(int r, int c, int dr, int dc);
	bool closeEnough(int r, int c, int dr, int dc, char dir);
	void createRoom(int r, int c, int dr, int dc);
	void createNRooms(int n);
	bool isValidCellSatus(char ch);
	Monster* monsterID(int ID, int r, int c);
};

#endif // !DUNGEON_H
