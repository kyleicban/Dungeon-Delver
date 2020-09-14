#ifndef SCROLL_H
#define SCROLL_H
#include "Object.h"

class Player;

class Scroll : public Object
{
public:
	Scroll(Player* player) : Object(), m_player(player) {}
	
	Player* player() { return m_player; }

	virtual string performMagic() = 0;
	virtual string name() = 0;

private:
	Player* m_player;
};

class Teleportation : public Scroll
{
public:
	Teleportation(Player* player) : Scroll(player) {}

	virtual string performMagic();
	virtual string name() { return "a scroll of teleportation"; }
};

class Armor : public Scroll
{
public:
	Armor(Player* player) : Scroll(player) {}

	virtual string performMagic();
	virtual string name() { return "a scroll of improve armor"; }
};

class Strength : public Scroll
{
public:
	Strength(Player* player) : Scroll(player) {}

	virtual string performMagic();
	virtual string name() { return "a scroll of raise strength"; }
};

class Health : public Scroll
{
public:
	Health(Player* player) : Scroll(player) {}

	virtual string performMagic();
	virtual string name() { return "a scroll of enhance health"; }
};

class Dexterity : public Scroll
{
public:
	Dexterity(Player* player) : Scroll(player) {}

	virtual string performMagic();
	virtual string name() { return "a scroll of enhance dexterity"; }
};

#endif // !SCROLL_H
