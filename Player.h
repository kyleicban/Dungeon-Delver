#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

class Object;

class Player : public Actor
{
public:
	Player(Dungeon* dungeon, int r, int c);
	virtual ~Player();

	//Accessors
	bool hasIdol() const { return m_idol; }
	virtual string name() const { return "Player"; }

	void setHealthZero() { m_hp = 0; }
	string doSomething(char ch);
	virtual void move(char dir);
	string attack(int r, int c);
	string pickUpObject();
	void descend();
	void takeIdol();
	string wieldWeapon(char ch);
	string readScroll(char ch);
	bool teleport(int r, int c);
	void increaseArmor(int i) { m_armor += i; }
	void increaseStrength(int i) { m_strength += i; }
	void increaseHealth(int i) { mx_hp += i; }
	void increaseDexterity(int i) { m_dexterity += i; }

private:
	bool m_idol;
	vector<Object*> m_inventory;
	int n_inventory;

	void viewInventory();
	Object* objectID(int ID);
};

#endif // !PLAYER_H
