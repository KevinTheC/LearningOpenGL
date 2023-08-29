#ifndef RENDERABLE_H
#define RENDERABLE_H
#include "UniqueID.h"
#include "unordered_map"
class Renderable {
protected:
	static UniqueID IDgen;
	static std::unordered_map<long int, Renderable*> map;
	long int ID;
public:
	const static void drawAll()
	{
		for (std::pair<long int, Renderable*> r : map)
			r.second->draw();
	}
	const long int getID()
	{
		return ID;
	}
	static const bool existID(long int num)
	{
		return IDgen.existsID(num);
	}
	Renderable() {
		ID = IDgen.genID();
		EnableRendering(true);
	}
	void EnableRendering(bool b)
	{
		if (b)
			map.emplace(ID, this);
		else
		{
			if (map.find(ID) != map.end())
				map.erase(map.find(ID));
		}	
	}
	~Renderable() {
		EnableRendering(false);
		IDgen.freeID(ID);
	}
	virtual void draw() = 0;
};
#endif