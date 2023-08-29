#include "Renderable.h"
UniqueID Renderable::IDgen;
std::unordered_map<long int, Renderable*> Renderable::map;