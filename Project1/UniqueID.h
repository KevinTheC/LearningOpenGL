#pragma once
#include "stdlib.h"
#include "set"
#include "iostream"
class UniqueID {
private:
	std::set<long int> IDs;
public:
	long int genID()
	{
		long int ID = 0;
		while (true) {
			ID = ((long int)rand() + 31427) * (rand()) + 31427;
			if (IDs.find(ID) == IDs.end())
				break;
		}
		IDs.emplace(ID);
		return ID;
	}
	bool freeID(long int ID) {
		auto itr = IDs.find(ID);
		if (itr == IDs.end())
			return false;
		IDs.erase(itr);
		return true;
	}
	bool existsID(long int ID)
	{
		return IDs.find(ID) != IDs.end();
	}
};