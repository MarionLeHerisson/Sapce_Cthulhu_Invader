#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"

const float Game::PlayerSpeed = 100.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(840, 600), "Space Invaders 1978", sf::Style::Close)
	//, mTexture()
	, mPlayer()
	, mFont()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mIsLookingUp(false)
	, mIsLookingDown(false)
	, mIsLookingRight(false)
	, mIsLookingLeft(false)
	, mIsAttacking(false)
	, mIsEating(false)
	, mIsBlocking(false)
{
	mWindow.setFramerateLimit(160);

	_TextureFish.loadFromFile("Media/Assets/fish-angler-Sheet.png", sf::IntRect(0, 0, 32, 32));
	_TextureLookingUp.loadFromFile("Media/Assets/cthulhu.png", sf::IntRect(0, 192, 64, 256));
	_TextureLookingDown.loadFromFile("Media/Assets/cthulhu.png", sf::IntRect(0, 0, 64, 64));
	_TextureLookingRight.loadFromFile("Media/Assets/cthulhu.png", sf::IntRect(0, 128, 64, 192));
	_TextureLookingLeft.loadFromFile("Media/Assets/cthulhu.png", sf::IntRect(0, 64, 64, 128));
	_TextureWeapon.loadFromFile("Media/Textures/SI_WeaponGreen.png");
	_TextureWeaponEnemy.loadFromFile("Media/Textures/SI_WeaponYellow.png");
	//mTexture.loadFromFile("Media/Textures/SI_Player.png");
	_TextureEnemy.loadFromFile("Media/Textures/SI_Enemy.png");
	_TextureBlock.loadFromFile("Media/Textures/SI_Block.png");
	mFont.loadFromFile("Media/Sansation.ttf");

	InitSprites();
}

void Game::ResetSprites()
{
	_IsGameOver = false;
	_IsEnemyWeaponFired = false;
	_IsPlayerWeaponFired = false;

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		entity->m_enabled = true;
	}
}

void Game::InitSprites()
{
	_lives = 3;
	_power = 0;
	_score = 0;
	_IsGameOver = false;
	_IsEnemyWeaponFired = false;
	_IsPlayerWeaponFired = false;

	//
	// Player
	//

	mPlayer.setTexture(_TextureLookingDown);
	mPlayer.setPosition(400.f, 300.f);//set to the middle of the screen
	std::shared_ptr<Entity> player = std::make_shared<Entity>();
	player->m_sprite = mPlayer;
	player->m_type = EntityType::player;
	player->m_size = _TextureLookingDown.getSize();
	player->m_position = mPlayer.getPosition();
	EntityManager::m_Entities.push_back(player);

	//
	// Enemies
	//

	//UP
	posSpawnUp[1]=400;
	posSpawnUp[2]=20;
	//DOWN
	posSpawnDown[1] = 400;
	posSpawnDown[2] = 550;
	//LEFT
	posSpawnLeft[1] = 20;
	posSpawnLeft[2] = 300;
	//RIGHT
	posSpawnRight[1] = 750;
	posSpawnRight[2] = 300;

	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	//
	// Lives
	//

	_LivesText.setFillColor(sf::Color::Green);
	_LivesText.setFont(mFont);
	_LivesText.setPosition(10.f, 50.f);
	_LivesText.setCharacterSize(20);
	_LivesText.setString(std::to_string(_lives));

	//
	// Power
	//

	_PowerText.setFillColor(sf::Color::Green);
	_PowerText.setFont(mFont);
	_PowerText.setPosition(10.f, 150.f);
	_PowerText.setCharacterSize(20);
	_PowerText.setString(std::to_string(_power));

	//
	// Text
	//

	_ScoreText.setFillColor(sf::Color::Green);
	_ScoreText.setFont(mFont);
	_ScoreText.setPosition(10.f, 100.f);
	_ScoreText.setCharacterSize(20);
	_ScoreText.setString(std::to_string(_score));
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{
	sf::Vector2f movement(0.f, 0.f);
	if (mIsLookingUp) {
		mPlayer.setTexture(_TextureLookingUp);
	}
	if (mIsLookingDown)
		mPlayer.setTexture(_TextureLookingDown);
	if (mIsLookingLeft)
		mPlayer.setTexture(_TextureLookingLeft);
	if (mIsLookingRight) {
		mPlayer.setTexture(_TextureLookingRight);
	}

	// TODO

	std::shared_ptr<Entity> player = std::make_shared<Entity>();
	player->m_sprite = mPlayer;
	player->m_type = EntityType::player;
	player->m_size = _TextureLookingUp.getSize();
	player->m_position = mPlayer.getPosition();
	EntityManager::m_Entities.push_back(player);


	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::player)
		{
			continue;
		}

		entity->m_sprite.move(movement * elapsedTime.asSeconds());
	}
}

void Game::render()
{
	mWindow.clear();

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		mWindow.draw(entity->m_sprite);
	}

	mWindow.draw(mStatisticsText);
	mWindow.draw(mText);
	mWindow.draw(_LivesText);
	mWindow.draw(_ScoreText);
	mWindow.draw(_PowerText);
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}

	//
	// Handle collisions
	//

	if (mStatisticsUpdateTime >= sf::seconds(0.050f))
	{
		if (_IsGameOver == true)
			return;

		HandleTexts();
		HandleGameOver();
		HandleCollisionWeaponEnemy();
		HandleCollisionWeaponPlayer();
		HandleCollisionWeaponBlock();
		HandleCollisionEnemyWeaponBlock();
		HandleCollisionBlockEnemy();
		HanldeWeaponMoves();
		HanldeEnemyWeaponMoves();
		HandleEnemyMoves();
		HandleEnemyWeaponFiring();
	}
}

void Game::HandleTexts()
{
	std::string lives = "Lives: " + std::to_string(_lives);
	_LivesText.setString(lives);

	std::string power = "Power: " + std::to_string(_power);
	_PowerText.setString(power);

	std::string score = "Score: " + std::to_string(_score);
	_ScoreText.setString(score);
	return;
}

void Game::HandleCollisionEnemyWeaponBlock()
{
	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::enemyWeapon)
		{
			continue;
		}

		for (std::shared_ptr<Entity> block : EntityManager::m_Entities)
		{
			if (block->m_type != EntityType::block)
			{
				continue;
			}

			if (block->m_enabled == false)
			{
				continue;
			}

			sf::FloatRect boundWeapon;
			boundWeapon = weapon->m_sprite.getGlobalBounds();

			sf::FloatRect boundBlock;
			boundBlock = block->m_sprite.getGlobalBounds();

			if (boundWeapon.intersects(boundBlock) == true)
			{
				weapon->m_enabled = false;
				_IsEnemyWeaponFired = false;
				//break;
				goto end2;
			}
		}
	}

end2:
	//nop
	return;
}

void Game::HandleCollisionWeaponPlayer()
{
	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::enemyWeapon)
		{
			continue;
		}

		sf::FloatRect boundWeapon;
		boundWeapon = weapon->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundWeapon.intersects(boundPlayer) == true)
		{
			weapon->m_enabled = false;
			_IsEnemyWeaponFired = false;
			_lives--;
			goto end;
		}
	}

end:
	//nop
	return;
}

void Game::HanldeEnemyWeaponMoves()
{
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemyWeapon)
		{
			continue;
		}

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		y += 1.0f;

		if (y >= 600)
		{
			entity->m_enabled = false;
			_IsEnemyWeaponFired = false;
		}
		else
		{
			entity->m_sprite.setPosition(x, y);
		}
	}
}

void Game::HandleEnemyWeaponFiring()
{
	if (_IsEnemyWeaponFired == true)
		return;

	std::vector<std::shared_ptr<Entity>>::reverse_iterator rit = EntityManager::m_Entities.rbegin();
	for (; rit != EntityManager::m_Entities.rend(); rit++)
	{
		std::shared_ptr<Entity> entity = *rit;

		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemy)
		{
			continue;
		}

		// a little random...
		int r = rand() % 20;
		if (r != 10)
			continue;

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		y--;

		std::shared_ptr<Entity> sw = std::make_shared<Entity>();
		sw->m_sprite.setTexture(_TextureWeaponEnemy);
		sw->m_sprite.setPosition(
			x + _TextureWeaponEnemy.getSize().x / 2,
			y + _TextureWeaponEnemy.getSize().y);

		sw->m_sprite.setPosition(
			entity->m_sprite.getPosition().x + _TextureEnemy.getSize().x / 2,
			entity->m_sprite.getPosition().y - 10);

		sw->m_type = EntityType::enemyWeapon;
		sw->m_size = _TextureWeaponEnemy.getSize();
		EntityManager::m_Entities.push_back(sw);

		_IsEnemyWeaponFired = true;
		break;
	}
}

void Game::HandleCollisionBlockEnemy()
{
	// Handle collision ennemy blocks

	for (std::shared_ptr<Entity> enemy : EntityManager::m_Entities)
	{
		if (enemy->m_enabled == false)
		{
			continue;
		}

		if (enemy->m_type != EntityType::enemy)
		{
			continue;
		}

		for (std::shared_ptr<Entity> block : EntityManager::m_Entities)
		{
			if (block->m_type != EntityType::block)
			{
				continue;
			}

			if (block->m_enabled == false)
			{
				continue;
			}

			sf::FloatRect boundEnemy;
			boundEnemy = enemy->m_sprite.getGlobalBounds();

			sf::FloatRect boundBlock;
			boundBlock = block->m_sprite.getGlobalBounds();

			if (boundEnemy.intersects(boundBlock) == true)
			{
				EntityManager::GetPlayer()->m_enabled = false;
				goto end3;
			}
		}
	}

end3:
	//nop
	return;
}

void Game::HandleEnemyMoves()
{
	//
	// Handle Enemy moves
	//

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemy)
		{
			continue;
		}

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;

		if (entity->m_bLeftToRight == true)
			x++;
		else
			x--;
		entity->m_times++;

		if (entity->m_times >= 100) //0)
		{
			if (entity->m_bLeftToRight == true)
			{
				entity->m_bLeftToRight = false;
				entity->m_times = 0;
			}
			else
			{
				entity->m_bLeftToRight = true;
				entity->m_times = 0;
				y += 1;
			}
		}

		entity->m_sprite.setPosition(x, y);
	}
}

void Game::HanldeWeaponMoves()
{
	//
	// Handle Weapon moves
	//

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::weapon)
		{
			continue;
		}

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		y--;

		if (y <= 0)
		{
			entity->m_enabled = false;
			_IsPlayerWeaponFired = false;
		}

		entity->m_sprite.setPosition(x, y);
	}
}

void Game::HandleCollisionWeaponBlock()
{
	// Handle collision weapon blocks

	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::weapon)
		{
			continue;
		}

		for (std::shared_ptr<Entity> block : EntityManager::m_Entities)
		{
			if (block->m_type != EntityType::block)
			{
				continue;
			}

			if (block->m_enabled == false)
			{
				continue;
			}

			sf::FloatRect boundWeapon;
			boundWeapon = weapon->m_sprite.getGlobalBounds();

			sf::FloatRect boundBlock;
			boundBlock = block->m_sprite.getGlobalBounds();

			if (boundWeapon.intersects(boundBlock) == true)
			{
				weapon->m_enabled = false;
				_IsPlayerWeaponFired = false;
				//break;
				goto end2;
			}
		}
	}

end2:
	//nop
	return;
}

void Game::HandleCollisionWeaponEnemy()
{
	// Handle collision weapon enemies

	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::weapon)
		{
			continue;
		}

		for (std::shared_ptr<Entity> enemy : EntityManager::m_Entities)
		{
			if (enemy->m_type != EntityType::enemy)
			{
				continue;
			}

			if (enemy->m_enabled == false)
			{
				continue;
			}

			sf::FloatRect boundWeapon;
			boundWeapon = weapon->m_sprite.getGlobalBounds();

			sf::FloatRect boundEnemy;
			boundEnemy = enemy->m_sprite.getGlobalBounds();

			if (boundWeapon.intersects(boundEnemy) == true)
			{
				enemy->m_enabled = false;
				weapon->m_enabled = false;
				_IsPlayerWeaponFired = false;
				_score += 10;
				//break;
				goto end;
			}
		}
	}

end:
	//nop
	return;
}

void Game::HandleGameOver()
{


	if (EntityManager::GetPlayer()->m_enabled == false)
	{
		DisplayGameOver();
	}

	if (_lives == 0)
	{
		DisplayGameOver();
	}
}

void Game::DisplayGameOver()
{
	if (_lives == 0)
	{
		mText.setFillColor(sf::Color::Green);
		mText.setFont(mFont);
		mText.setPosition(200.f, 200.f);
		mText.setCharacterSize(80);

		mText.setString("GAME OVER");

		_IsGameOver = true;
	}
	else
	{
		ResetSprites();
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::Up) {
		mIsLookingUp = isPressed;
	}
	else if (key == sf::Keyboard::Down) {
		mIsLookingDown = isPressed;
	}
	else if (key == sf::Keyboard::Left) {
		mIsLookingLeft = isPressed;
	}
	else if (key == sf::Keyboard::Right) {
		mIsLookingRight = isPressed;
	}
	else if (key == sf::Keyboard::A) {
		mIsAttacking = isPressed;
	}
	else if (key == sf::Keyboard::Z) {
		mIsEating = isPressed;
	}
	else if (key == sf::Keyboard::E) {
		mIsBlocking = isPressed;
	}

	if (key == sf::Keyboard::Space)
	{
		if (isPressed == false)
		{
			return;
		}

		if (_IsPlayerWeaponFired == true)
		{
			return;
		}

		std::shared_ptr<Entity> sw = std::make_shared<Entity>();
		sw->m_sprite.setTexture(_TextureFish);
		sw->m_sprite.setPosition(
			EntityManager::GetPlayer()->m_sprite.getPosition().x + EntityManager::GetPlayer()->m_sprite.getTexture()->getSize().x / 2,
			EntityManager::GetPlayer()->m_sprite.getPosition().y - 10);
		sw->m_type = EntityType::weapon;
		sw->m_size = _TextureFish.getSize();
		EntityManager::m_Entities.push_back(sw);

		_IsPlayerWeaponFired = true;
	}
}
