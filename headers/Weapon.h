#ifndef WEAPON_H
#define WEAPON_H
#include "Object.h"

class Actor;

class Weapon : public Object
{
public:
	Weapon(int dex, int dam) : m_dexterity(dex), m_damage(dam) {}

	int dexterity() const { return m_dexterity; }
	int damage() const { return m_damage; }
	virtual string name() = 0;
	virtual string action() = 0;

private:
	int m_dexterity;
	int m_damage;
};

class Mace : public Weapon
{
public:
	Mace() : Weapon(0, 2) {}

	virtual string name() { return "mace"; }
	virtual string action() { return "swings mace"; }
};

class ShortSword : public Weapon
{
public:
	ShortSword() : Weapon(0, 2) {}

	virtual string name() { return "short sword"; }
	virtual string action() { return "slashes short sword"; }
};

class LongSword : public Weapon
{
public:
	LongSword() : Weapon(2, 4) {}

	virtual string name() { return "long sword"; }
	virtual string action() { return "slashes long sword"; }
};

class MagicAxe : public Weapon
{
public:
	MagicAxe() : Weapon(5, 5) {}

	virtual string name() { return "magic axe"; }
	virtual string action() { return "chops magic axe"; }
};

class MagicFangs : public Weapon
{
public:
	MagicFangs() : Weapon(3, 2) {}

	virtual string name() { return "magic fangs of sleep"; }
	virtual string action() { return "strikes magic fangs of sleep"; }
};

#endif // !WEAPON_H
