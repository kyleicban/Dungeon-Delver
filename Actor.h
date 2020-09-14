#ifndef ACTOR_H
#define ACTOR_H
#include <string>
#include "utilities.h"
using namespace std;

class Dungeon;
class Weapon;

class Actor
{
public:
	//Constructor/Destructor
	Actor(Dungeon* dungeon, int r, int c) : m_dungeon(dungeon), m_row(r), m_col(c), m_sleep(0) {}
	virtual ~Actor() {}

	//Accessors
	int row() const { return m_row; }
	int col() const { return m_col; }
	Dungeon* dungeon() const { return m_dungeon; }
	int hp() const { return m_hp; }
	int mxhp() const { return mx_hp; }
	Weapon* weapon() const { return m_weapon; }
	int armor() const { return m_armor; }
	int strength() const { return m_strength; }
	int dexterity() const { return m_dexterity; }
	bool isDead() const { return m_hp <= 0; }

	virtual string name() const = 0;

	//Mutators
	virtual void move(char dir) = 0;
	void takeDamage(int damage) { m_hp -= damage; }
	void goToSleep();

protected:
	int m_row, m_col;
	Dungeon* m_dungeon;
	int	m_hp;
	int mx_hp;
	Weapon* m_weapon;
	int	m_armor;
	int	m_strength;
	int	m_dexterity;
	int m_sleep;

	bool canMove(int r, int c);
private:
};

#endif // !ACTOR_H
