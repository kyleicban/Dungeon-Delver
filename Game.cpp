// Game.cpp

#include "Game.h"
#include "Player.h"
#include "utilities.h"
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game()
{
    m_dungeon = new Dungeon(0);
    m_dungeon->setGrid();
}

void Game::play()
{
    char ch;
    m_dungeon->display('0');
    do
    {
        ch = getCharacter();
        clearScreen();
        m_dungeon->display(ch);

    } while (!((m_dungeon->player())->isDead()) && !(m_dungeon->player())->hasIdol() && ch != 'q');
}

// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.
