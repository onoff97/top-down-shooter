#ifndef ENTITY_H
#define ENTITY_H

#include "cocos2d.h"

using namespace cocos2d;

class Entity : Node
{
public:
	virtual void update(float dt) = 0;
	float x, y;
};
#endif