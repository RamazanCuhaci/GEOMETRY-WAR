#pragma once

#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "Entity.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <cmath>


struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig  { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN,SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
	sf::RenderWindow		m_window;				//the window we we will draw to
	EntityManager			m_entities;				//vector of entities to maintain
	sf::Font				m_font;					//the font we will use to draw
	sf::Text				m_text;					//the score text to be drawn to the screen
	PlayerConfig			m_playerconfig;
	EnemyConfig				m_enemyConfig;
	BulletConfig			m_bulletConfig;
	int						mScore = 0;
	int						m_currentFrame = 0;
	int						m_lastEnemySpawnTime = 0;
	int						m_lastSpecialWeaponUseTime = 0;
	bool					m_paused = false;		//whether we update game logic
	bool					m_running = true;		//whether the game is running

	long double				PI = 3.141592653589793238462643383279502884;

	std::shared_ptr<Entity> m_player;

	void init(const std::string& path);				//initialize the GameState with a config file
	//void setPaused(bool pause);						//pause the game

	void sMovement();								//System: Entity position / movement update
	void sUserInput();								//System: User Input
	void sLifespan();								//System: Lifespan
	void sRender();									//System: Render / Drawing
	void sEnemySpawner();							//System: Spawns Enemies
	void sCollision();								//System: Collisions

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & target);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:

	Game(const std::string& config); //constructor, takes in game config

	void run();



};