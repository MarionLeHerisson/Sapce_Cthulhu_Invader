#pragma once

enum EntityType
{
	player,
	weapon,
	enemyWeapon,
	enemyMasterWeapon,
	enemy,
	wizard,
	baby,
	fish,
	block
};

class Entity
{
public:
	Entity() { };
	~Entity() { };

public:
	sf::Sprite m_sprite;
	sf::Vector2u m_size;
	sf::Vector2f m_position;
	EntityType m_type;
	std::string movingDirection;
	bool m_enabled = true;
	bool isTentacle = false;
	std::string positionSpawn;

	// Enemy only
	bool m_bLeftToRight = true;
	int m_times = 0;
	std::string getDirection();
};

