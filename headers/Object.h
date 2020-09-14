#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include "globals.h"
using namespace std;

class Object 
{
public:
	Object() {};
	virtual ~Object() {}

	virtual string name() = 0;
};

#endif // !OBJECT_H
