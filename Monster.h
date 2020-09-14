#ifndef MONSTER_H
#define MONSTER_H

#include "Actor.h"
#include "globals.h"

class Player;

class Monster : public Actor
{
public:
	Monster(Dungeon* dungeon, int r, int c, Player* player, int smellingDistance) 
		: Actor(dungeon, r, c), m_player(player), m_smellingDistance(smellingDistance) {}
	virtual ~Monster() { delete m_weapon; }

	virtual string name() const = 0;

	virtual string doSomething();
	virtual void move(char dir);
	virtual char chooseDirection();
	bool withinPursuingDistance() const;
	bool withinAttackingDistance() const;

	int smellingDistance() const { return m_smellingDistance; }

	string attack();

protected:
	Player* m_player;

private:
	int m_smellingDistance;
};

class Bogeyman : public Monster
{
public:
	Bogeyman(Dungeon* dungeon, int r, int c, Player* player, int smellingDistance);
	virtual ~Bogeyman();

	virtual string name() const { return "Bogeyman"; }
};

class Snakewoman : public Monster
{
public:
	Snakewoman(Dungeon* dungeon, int r, int c, Player* player, int smellingDistance);
	virtual ~Snakewoman();

	virtual string name() const { return "Snakewoman"; }
};

class Dragon : public Monster
{
public:
	Dragon(Dungeon* dungeon, int r, int c, Player* player, int smellingDistance);
	virtual ~Dragon();

	virtual string doSomething();

	virtual string name() const { return "Dragon"; }
};

class Goblin : public Monster
{
public:
	Goblin(Dungeon* dungeon, int r, int c, Player* player, int smellingDistance);
	virtual ~Goblin();

	virtual char chooseDirection(int dummy);
	virtual string name() const { return "Goblin"; }
private:
	bool pathExists(char grid[][MAXCOLS], int sr, int sc, int er, int ec, int iterations);
};

#endif // !MONSTER_H
