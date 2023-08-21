#pragma once
#include "stdlib.h"
#include "set"
class UniqueID {
private:
	std::set<long int> IDs;
public:
	long int genID();
	bool freeID(long int);
	bool existsID(long int);
};