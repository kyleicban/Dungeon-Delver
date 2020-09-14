#include <iostream>
#include <string>
#include "Dungeon.h"
#include "Player.h"
#include "Monster.h"
#include "Weapon.h"
#include "Scroll.h"
#include "utilities.h"
using namespace std;

Player::Player(Dungeon* dungeon, int r, int c) : Actor(dungeon, r, c)
{
	m_hp = 20;
	mx_hp = 20;
	m_armor = 2;
	m_strength = 2;
	m_dexterity = 2;
	m_inventory.push_back(new ShortSword());
	m_weapon = dynamic_cast<Weapon*> (m_inventory[0]);
	n_inventory = 1;
	m_idol = false;
}

Player::~Player()
{
	for (int i = 0; i < n_inventory; i++)
		delete m_inventory[i];
}

string Player::doSomething(char ch)
{
	int heal = randInt(1, 10); //chance to heal...
	if (m_hp < mx_hp && heal == 1) m_hp++;
	char s;
	if (ch == CHEAT) //hacks //prioritize cheating
	{
		mx_hp = 50;
		m_hp = 50;
		m_strength = 9;
	}
	if (m_sleep > 0)
	{
		m_sleep--;
		return "";
	}
	string msg = "";
	switch (ch) //what did you type?
	{
	case ARROW_UP: if (canMove(m_row - 1, m_col)) move(ch);
				 else msg += attack(m_row - 1, m_col); break;
	case ARROW_DOWN: if (canMove(m_row + 1, m_col)) move(ch);
				   else msg += attack(m_row + 1, m_col); break;
	case ARROW_LEFT: if (canMove(m_row, m_col - 1)) move(ch);
				   else msg += attack(m_row, m_col - 1); break;
	case ARROW_RIGHT: if (canMove(m_row, m_col + 1)) move(ch);
					else msg += attack(m_row, m_col + 1); break;
	case GRAB:
		msg += pickUpObject(); break;
	case WIELD:
		viewInventory();
		s = getCharacter();
		msg += wieldWeapon(s);
		clearScreen();
		break;
	case READ:
		viewInventory();
		s = getCharacter();
		msg += readScroll(s);
		clearScreen();
		break;
	case INVENTORY:
		viewInventory(); 
		getCharacter();
		clearScreen();
		break;
	case STAIRS:
		descend(); break;
	case IDOL:
		takeIdol(); break;
	default:
		break;
	}
	return msg;
}

void Player::move(char dir) //moves the player...
{
	switch (dir)
	{
	case ARROW_UP: if (canMove(m_row - 1, m_col)) m_row--; break;
	case ARROW_DOWN: if (canMove(m_row + 1, m_col))  m_row++; break;
	case ARROW_LEFT: if (canMove(m_row, m_col - 1)) m_col--; break;
	case ARROW_RIGHT: if (canMove(m_row, m_col + 1)) m_col++; break;
	default: break;
	}
}

string Player::attack(int r, int c) //call when you can move
{
	if (m_dungeon->getCellStatus(r, c) == WALL) return ""; //if your going into a wall do nothing
	int i;												   //otherwise its a monster
	for (i = 0; i < m_dungeon->numMonsters(); i++)		   //scan for the monster
		if ((m_dungeon->ithMonster(i))->row() == r && (m_dungeon->ithMonster(i))->col() == c) break;
	string msg = "";
	if (i != m_dungeon->numMonsters())
	{
		int attackerDexterity = dexterity() + m_weapon->dexterity(); //will your attack hit?
		int defenderDexterity = (m_dungeon->ithMonster(i))->dexterity() + 
								((m_dungeon->ithMonster(i))->weapon())->dexterity();

		 
		if (randInt(1, attackerDexterity) >= randInt(1, defenderDexterity)) //print the appropriate message
		{
			int damage = m_strength + m_weapon->damage();
			(m_dungeon->ithMonster(i))->takeDamage(damage);
			if ((m_dungeon->ithMonster(i))->isDead()) //if killed the monster
			{
				msg += "The " + name() + " " + m_weapon->action() + " at the " + (m_dungeon->ithMonster(i))->name() + " dealing a lethal blow";
				m_dungeon->deleteMonster(i);
			}
			//regular action
			else msg += "The " + name() + " " + m_weapon->action() + " at the " + (m_dungeon->ithMonster(i))->name() + " and hits";

			//if you're wielding the fangs
			if (!((m_dungeon->ithMonster(i))->isDead()) && dynamic_cast<MagicFangs*>(m_weapon) != nullptr)
			{ 
				if (randInt(1, 5) == 1)
				{
					(m_dungeon->ithMonster(i))->goToSleep();
					msg += " causing it to sleep";
				}
			}
		}
		//if you miss
		else msg += "The " + name() + " " + m_weapon->action() + " at the " + (m_dungeon->ithMonster(i))->name() + " and missed";
	}
	return msg;
}

string Player::pickUpObject()
{
	if (n_inventory >= 26) return "Your pockets are full.";

	if (m_dungeon->getCellStatus(m_row, m_col) >= 1 && m_dungeon->getCellStatus(m_row, m_col) <= 10)
	{
		n_inventory++;
		//objects on the grid are secretly coded as numbers
		//decode the number using objectID and pushback the inventory with the corresponding one
		m_inventory.push_back(objectID(m_dungeon->getCellStatus(m_row, m_col)));
		m_dungeon->setCellStatus(EMPTY, m_row, m_col);
		return "You picked up " + m_inventory[n_inventory - 1]->name();
	}
	return "";
}

void Player::descend()
{
	if (m_dungeon->getCellStatus(m_row, m_col) == STAIRS) //if the stairs are below you
	{
		m_dungeon->nextLevel(); //increase level
		m_dungeon->setGrid(); //reset grid
		int r, c; //transport to new spot
		do
		{
			r = randInt(1, MAXROWS);
			c = randInt(1, MAXCOLS);
		} while (!teleport(r, c));
	}
}

void Player::takeIdol() //win the game
{
	if (m_dungeon->getCellStatus(m_row, m_col) == IDOL) m_idol = true;
}

string Player::wieldWeapon(char ch)
{
	if (!isalpha(ch)) return "";
	char lowercase = tolower(ch);
	int slot = lowercase - 97;
	if (slot < n_inventory)
	{
		if (dynamic_cast<Weapon*>(m_inventory[slot]) != nullptr) //if the selected slot is not a weapon
		{
			m_weapon = dynamic_cast<Weapon*>(m_inventory[slot]); //equip it
			return "You are wielding " + m_inventory[slot]->name();
		}
		else return "You cannot wield a scroll";
	}
	return "";
}

string Player::readScroll(char ch)
{
	if (!isalpha(ch)) return "";
	string msg = "";
	char lowercase = tolower(ch);
	int slot = lowercase - 97;
	if (slot < n_inventory)
	{
		if (dynamic_cast<Scroll*>(m_inventory[slot]) != nullptr) //if the selected slot is not a scroll
		{
			msg += dynamic_cast<Scroll*>(m_inventory[slot])->performMagic(); //perform the magic

			delete m_inventory[slot]; //first delete the spot
			m_inventory[slot] = nullptr; //reassign to the slot spot to prevent an error
			m_inventory.erase(m_inventory.begin() + slot); //erase the spot
			n_inventory--; //decrease the inventory size
		}
		else return "You cannot read a weapon";
	}
	return msg;
}

bool Player::teleport(int r, int c) //teleport to Coord r, c
{ 
	if (m_dungeon->getCellStatus(r, c) == WALL) return false;
	
	for(int i = 0; i < m_dungeon->numMonsters(); i++)
		if ((m_dungeon->ithMonster(i))->row() == r && (m_dungeon->ithMonster(i))->col() == c) return false;
	m_row = r; 
	m_col = c; 
	return true;
}

void Player::viewInventory() //it is always called with wieldWeapon and readScroll
{
	clearScreen();
	cout << "Inventory:" << endl;
	for (int i = 0; i < n_inventory; i++)
	{
		char slot = i + 97;
		cout << " " << slot << ". " << m_inventory[i]->name() << endl;
	}
}

Object* Player::objectID(int ID) //transform numbers into Objects
{
	switch (ID)
	{
	case MACE: return new Mace();
	case SHSW: return new ShortSword();
	case LGSW: return new LongSword();
	case AXE: return new MagicAxe();
	case FANGS: return new MagicFangs();
	case TELEPORTATION: return new Teleportation(this);
	case ARMOR: return new Armor(this);
	case STRENGTH: return new Strength(this);
	case HEALTH: return new Health(this);
	case DEXTERITY: return new Dexterity(this);
	default: return nullptr;
	}
	return nullptr;
}