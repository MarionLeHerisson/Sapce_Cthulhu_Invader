#pragma once
#include "Weapon.h"
#include <iostream>

#define SPRITE_COUNT_X 11
#define SPRITE_COUNT_Y 5
#define BLOCK_COUNT 4

class Game
{
public:
	Game();
	~Game() { };
	void run();

private:
	void processEvents();
	void update(sf::Time elapsedTime);
	void render();

	void InitSprites();
	void ResetSprites();

	void updateStatistics(sf::Time elapsedTime);
	void HandleTexts();
	void HandleCollisionEnemyWeaponBlock();
	void HandleCollisionWeaponPlayer();
	void HanldeEnemyWeaponMoves();
	void HandleEntitiesApparing();
	void HandleEnemyWeaponFiring();
	void HandleCollisionBlockEnemy();
	//void HandleEnemyMoves();
	void HandleEnemiesSwitching();
	void HanldeWeaponMoves();
	void HandleCollisionWeaponBlock();
	void HandleCollisionWeaponEnemy();
	void HandleGameOver();
	void DisplayGameOver();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void SpawnEntities();


private:
	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
	sf::Sprite	mPlayer;
	sf::Font	mFont;
	sf::Text	mStatisticsText;
	sf::Time	mStatisticsUpdateTime;
	sf::Text	mText;
	sf::Text	_LivesText;
	sf::Text	_PowerText;
	int _lives = 3;
	int _power = 0;
	sf::Text	_ScoreText;
	int _score = 0;
	int posSpawnUp[2];
	int posSpawnRight[2];
	int posSpawnDown[2];
	int posSpawnLeft[2];
	int typeEnemy;
	int isGenerated;
	int posGenerate;
	sf::Sprite	_Enemies[4];

	std::size_t	mStatisticsNumFrames;
	bool mIsLookingUp;
	bool mIsLookingDown;
	bool mIsLookingRight;
	bool mIsLookingLeft;
	bool mRightIsHere;
	bool mLeftIsHere;
	bool mUpIsHere;
	bool mDownIsHere;
	std::string mdirectionLooking;

	bool _IsGameOver = false;
	bool _IsEnemyWeaponFired = false;
	bool _IsPlayerWeaponFired = false;
	bool _PlayerWeaponFiredToTop = false;
	bool _PlayerWeaponFiredToBottom = false;
	bool _PlayerWeaponFiredToRight = false;
	bool _PlayerWeaponFiredToleft = false;
	bool _IsPlayerTentacleFired = false;
	bool _PlayerTentacleFiredToTop = false;
	bool _PlayerTentacleFiredToBottom = false;
	bool _PlayerTentacleFiredToRight = false;
	bool _PlayerTentacleFiredToleft = false;

	sf::Texture _TextureLookingUp;
	sf::Texture _TextureLookingDown;
	sf::Texture _TextureLookingRight;
	sf::Texture _TextureLookingLeft;
	sf::Texture	_TextureBubbleGreen;
	sf::Texture	_TextureBubbleRed;
	sf::Texture	_TextureFish;
	sf::Texture _TextureBaby;

	sf::Texture	_TextureEnemy;
	sf::Sprite	_Enemy[4];
	sf::Texture	_TextureBlock;
	sf::Sprite	_Block[BLOCK_COUNT];
	sf::Texture	_TextureWeapon;
	sf::Texture	_TextureWeaponEnemy;
	sf::Texture	_TextureWeaponEnemyMaster;
	sf::Sprite	_Weapon;
	sf::Texture	_TextureEnemyMaster;
};

