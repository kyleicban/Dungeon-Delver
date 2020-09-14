// utilities.h

#ifndef UTILITIES_INCLUDED
#define UTILITIES_INCLUDED

const char ARROW_LEFT = 'a';
const char ARROW_RIGHT = 'd';
const char ARROW_UP = 'w';
const char ARROW_DOWN = 's';

char getCharacter();
void clearScreen();
int randInt(int lowest, int highest); // random int from lowest to highest
int randInt(int limit);               // random int from 0 to limit-1
bool trueWithProbability(double p);   // return true with probability p

#endif // UTILITIES_INCLUDED
