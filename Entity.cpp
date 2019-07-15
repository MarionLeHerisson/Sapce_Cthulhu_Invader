#include "pch.h"
#include "Entity.h"


std::string Entity::getDirection() {
	return movingDirection;
}

int Entity::getSpawn() {
	return positionSpawn;
}