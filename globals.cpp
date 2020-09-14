#include <iostream>
#include "globals.h"
#include "Weapon.h"
#include "Scroll.h"
using namespace std;

bool isInBounds(int r, int c) { return r >= 1 && r <= MAXROWS && c >= 1 && c <= MAXCOLS; }