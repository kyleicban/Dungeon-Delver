# Dungeon-Delver
Welcome to the Dungeon MUAHAHAHA

In this turn based c++ based game, delve through different rooms, fighting your way through 
Goblins, Snakewomen, Bogeymen, and Dragons in your effort to retrieve the coveted GOLDEN AMULET (woo woo!)


DIRECTIONS

- Fight your way through 5 dungeon levels starting on "Dungeon Level: 0"
  On each level, lies a staircase, '>' , that will lead you deeper into the dungeon.
  On the final level, "Dungeon Level: 4" , there will lie your goal, the GOLDEN AMULET, '&'
  Collect the AMULET before dying to win the game!
- You, the player , '@' , start off with basic gear and a less than stellar weapon.
  Fight monsters to advance in the dungeon and collect items to increase your stats
- Monsters:
  - Goblins, 'G'
  - Snakewomen, 'S' (they weild the 'magic fangs of sleep' which can put you to sleep)
  - Bogeymen, 'B'
  - Dragons, 'D'
- Items:
  - Items, ')' , can be found randomly in rooms or dropped by monsters
    They can be either weapons to increase your damage output or scrolls to increase your stats


CONTROLS

**The player acts first everytime. Each input counts as a turn for the player AND the monsters 
  EVEN IF the player fails to pick a valid move (e.g. moving into a wall, picking up an object not 
  on the ground, pressing an incorrect key). Be sure to count your moves carefully...

- Use the arrow keys (or h,j,k,l as left,down,up,right) to navigate the dungeon floors
  - Navigate in the direction of a monster to attack them
- Press 'g' to pick up items on the floor
- Press 'i' to access your inventory (VIEW ONLY)
- Press 'w' to weild a new weapon
  - Also the reason why I couldn't use w,a,s,d for alternate navigation :(
- Press 'r' to read a scroll to boost your stats
- Press '>' to descend further into the dungeon
- Press '&' to pick up the GOLDEN AMULET and BEAT THE GAME
- Press 'c' to cheat, instantly healing yourself to MAX_HEALTH and giving yourself MAX_STRENGTH
  - Sometimes playing legit is annoying, don't worry I get it :)
