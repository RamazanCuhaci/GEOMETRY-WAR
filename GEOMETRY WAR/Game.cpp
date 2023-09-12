#include "Game.h"
#include <sstream>


Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{

	int width, height, frameRate, screenMode;
	std::string temp;
	
	std::ifstream fin(path);
	
	
	while (fin >> temp)
	{
		if (temp == "Window")
		{
			fin >> width >> height >> frameRate >> screenMode;

			
			if (screenMode == 0)
			{
				m_window.create(sf::VideoMode(width, height), "Geometry War", sf::Style::Fullscreen);
				m_window.setFramerateLimit(frameRate);
			}
			else
			{
				m_window.create(sf::VideoMode(width, height), "Geometry War");
				m_window.setFramerateLimit(frameRate);
			}
		}

		else if (temp == "Font")
		{
			std::string fontPath;
			int fontSize, red, green, blue;

			fin >> fontPath >> fontSize >> red >> green >> blue;
			

			if (!m_font.loadFromFile(fontPath))
			{
				std::cerr << "Could not load font!\n";
				exit(-1);
			}

			m_text.setFont(m_font);
			
			std::stringstream sstm;
			sstm << "SCORE : " << mScore;
			std::string result = sstm.str();
			m_text.setString(result);
			m_text.setCharacterSize(fontSize);
			m_text.setFillColor(sf::Color(red, blue, green));

		}

		else if (temp == "Player")
		{
			fin >> m_playerconfig.SR >> m_playerconfig.CR >> m_playerconfig.S >> m_playerconfig.FR >> m_playerconfig.FG
				>> m_playerconfig.FB >> m_playerconfig.OR >> m_playerconfig.OG >> m_playerconfig.OB >> m_playerconfig.OT >> m_playerconfig.V;

		}
		else if (temp == "Enemy")
		{
			fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR
				>> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
		}

		else if (temp == "Bullet")
		{
			fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG
				>> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
		}
		
	}

	spawnPlayer();
}

void Game::run()
{

	while (m_running)
	{
		m_entities.update();

		if (!m_paused)
		{
			sEnemySpawner();
			sMovement();
			sCollision();
			sLifespan();
			sUserInput();
		}
		sRender();
		
		//If player dead , respawn it
		if (!m_player->isActive()) 
		{
			spawnPlayer();
		}
		
		m_currentFrame++;
	}

}

/*
void Game::setPaused(bool pause)
{
	m_paused = !m_paused;
}
*/

void Game::sMovement()
{


	m_player->cTransform->velocity = { 0,0 };

	
	if (m_player->cInput->up)
	{
		m_player->cTransform->velocity.y = -5;
	}
	if (m_player->cInput->down)
	{
		m_player->cTransform->velocity.y = 5;
	}
	if (m_player->cInput->left)
	{
		m_player->cTransform->velocity.x = -5;
	}
	if (m_player->cInput->right)
	{
		m_player->cTransform->velocity.x = 5;
	}

	
	for (auto e : m_entities.getEntities("enemy"))
	{
		
		if (m_window.getSize().x <=( e->cShape->circle.getGlobalBounds().getPosition().x+ e->cShape->circle.getRadius())
			|| 0 >= (e->cShape->circle.getGlobalBounds().getPosition().x +  e->cShape->circle.getRadius()))
		{
			e->cTransform->velocity.x *= -1;
		}

		if (m_window.getSize().y <= (e->cShape->circle.getGlobalBounds().getPosition().y +  e->cShape->circle.getRadius())
			|| 0 >= (e->cShape->circle.getGlobalBounds().getPosition().y + e->cShape->circle.getRadius()))
		{

			e->cTransform->velocity.y *= -1;
		}

	}

	for (auto e : m_entities.getEntities())
	{
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;
	}


}

void Game::sLifespan()
{
	for (auto e : m_entities.getEntities("bullet"))
	{
		
		e->cLifespan->remaining -= 1;
		
		//Fading
		e->cShape->circle.setFillColor(sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB , 
			255*(e->cLifespan->remaining)/e->cLifespan->total));
		e->cShape->circle.setOutlineColor(sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB,
			255 * (e->cLifespan->remaining) / e->cLifespan->total));
		
	}

	for (auto e : m_entities.getEntities("smallEnemy"))
	{

		e->cLifespan->remaining -= 1;

		//Fading
		e->cShape->circle.setFillColor(sf::Color(e->cShape->circle.getFillColor().r, e->cShape->circle.getFillColor().g, e->cShape->circle.getFillColor().b,
			255 * (e->cLifespan->remaining) / e->cLifespan->total));
		e->cShape->circle.setOutlineColor(sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB,
			255 * (e->cLifespan->remaining) / e->cLifespan->total));

	}

	for (auto e : m_entities.getEntities("bullet"))
	{
		
		if (e->cLifespan->remaining==0)
		{	
			e->destroy();
		}
	}

	for (auto e : m_entities.getEntities("smallEnemy"))
	{

		if (e->cLifespan->remaining == 0)
		{
			e->destroy();
		}
	}

}

void Game::sRender()
{
	m_window.clear();

	for (auto e : m_entities.getEntities())
	{
		
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);
		m_window.draw(e->cShape->circle);
	}
	std::stringstream sstm;
	sstm << "SCORE : " << mScore;
	std::string result = sstm.str();
	m_text.setString(result);

	m_window.draw(m_text);
	m_window.display();

}

void Game::sEnemySpawner()
{
	int enemySpawnRate = 90;
	//  code which implements enemy spawning should go here
	// (use m_currentFrame - m_lastEnemySpawnTime) to determine
	// how long it has been since the last enemy spawned
	if ((m_currentFrame - m_lastEnemySpawnTime) % 90 == 0)
	{
		spawnEnemy();
	}

}

void Game::sCollision()
{
	
	for (auto b : m_entities.getEntities("bullet"))
	{
		float bulletRadius =  (b->cCollission->radius);
		for (auto e : m_entities.getEntities("enemy"))
		{
			//Use distSquare because sqrt function is expensive, we can do it same operation with distSquare

			float totalRadius = bulletRadius + (e->cCollission->radius);
			if ((totalRadius * totalRadius) >= (b->cTransform->pos.distSquare(e->cTransform->pos)))
			{
				e->destroy();
				mScore += e->cScore->score;
				spawnSmallEnemies(e);
				
				b->destroy();
			}
		}
		for (auto e : m_entities.getEntities("smallEnemy"))
		{
			//Use distSquare because sqrt function is expensive, we can do it same operation with distSquare

			float totalRadius = bulletRadius + (e->cCollission->radius);
			if ((totalRadius * totalRadius) >= (b->cTransform->pos.distSquare(e->cTransform->pos)))
			{
				e->destroy();
				mScore += e->cScore->score;
				b->destroy();
			}
		}

	}

	float playerRadius = (m_player->cCollission->radius);
	for (auto e : m_entities.getEntities("enemy"))
	{
		float totalRadius = playerRadius + (e->cCollission->radius);
		if ((totalRadius * totalRadius) >= (m_player->cTransform->pos.distSquare(e->cTransform->pos)))
		{
			e->destroy();
			spawnSmallEnemies(e);

			m_player->destroy();
		}
	}


}

//Respawn the player in the middle of the screen
void Game::spawnPlayer()
{
	//Adding all properties of the player with the correct values from the config

	//We create every enttity by calling EntityManager.addEntity(tag)
	//This returns a std::shared_ptr<Entity>, so we use "auto" to save typing
	auto entity = m_entities.addEntity("player");

	//Give this entity a Transform so it spawns at (200,200) with velocity (1,1) and angle 0
	float mx = m_window.getSize().x / 2;
	float my = m_window.getSize().y / 2;

	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0, 0), 0.0f);

	//The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 5
	entity->cShape = std::make_shared<CShape>(m_playerconfig.SR, m_playerconfig.V, sf::Color(m_playerconfig.FR, m_playerconfig.FG, m_playerconfig.FB),
		sf::Color(m_playerconfig.OR, m_playerconfig.OG, m_playerconfig.OB), m_playerconfig.OT);

	//Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();
	
	entity->cCollission = std::make_shared<CCollision>(m_playerconfig.CR);

	//Since we want this Entity to be our player, set our Game's player variable to be this Entity
	//This goes slightly against the EntityManager, but we use player so much it's worth it
	m_player = entity;

}

//Spawn an enemy at a random position
void Game::spawnEnemy()
{
	// make sure the enemy is spawned properly with the m_enemyConfig variable
	// the enemy must be spawned completely within the bounds of the window
	//

	auto entity = m_entities.addEntity("enemy");
	//srand(time(NULL));
	//Because shape origin we need modify random numbers for enemy spawning in the window
	float ex = m_enemyConfig.SR + (fmod(rand(), 1 + (m_window.getSize().x - m_enemyConfig.SR) - m_enemyConfig.SR));
	float ey = m_enemyConfig.SR + (fmod(rand(), 1 + (m_window.getSize().y - m_enemyConfig.SR) - m_enemyConfig.SR));
	;
	//Random speed within the range from config file
	float sx = m_enemyConfig.SMIN + (fmod(rand(), 1 + m_enemyConfig.SMAX - m_enemyConfig.SMIN));
	float sy = m_enemyConfig.SMIN + (fmod(rand(), 1 + m_enemyConfig.SMAX - m_enemyConfig.SMIN));
	
	//Random vertices
	int vertices = m_enemyConfig.VMIN + (rand() % (1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN));
	

	//Random fill color
	int red = fmod(rand(), 256);
	int blue = fmod(rand(), 256);
	int green = fmod(rand(), 256);

	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(sx, sy), 0.0f);

	//The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 5
	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, vertices, sf::Color(red, blue, green), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);

	entity->cCollission = std::make_shared<CCollision>(m_enemyConfig.CR);
	
	entity->cScore = std::make_shared<CScore>(entity->cShape->circle.getPointCount()*100);

	

	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;

}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	// Spawn small enemies at the location of the input enemy e
	

	//When we create the smaller enemy, we have to read the values of the original enemy
	//- spawn a number of small enemies equal to the vertices of the original enemy
	//- set each small enemy to the same color as the original, half the size
	//- small enemies are worth double points of the original enemy

	float angle = 360 / (entity->cShape->circle.getPointCount());
	
	for (size_t i = 0; i < entity->cShape->circle.getPointCount() ; i++)
	{
		
		auto e = m_entities.addEntity("smallEnemy");
		
		Vec2 speedVector = { entity->cTransform->velocity.length() * cosf((angle * (PI / 180)) * i),
			entity->cTransform->velocity.length() * sinf((angle * (PI / 180)) * i) };
		
		speedVector.normalize();
		
		e->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), speedVector ,0);
		e->cShape = std::make_shared<CShape>(entity->cShape->circle.getRadius() / 2, entity->cShape->circle.getPointCount(),
			entity->cShape->circle.getFillColor(), entity->cShape->circle.getOutlineColor(), entity->cShape->circle.getOutlineThickness());
		e->cCollission = std::make_shared<CCollision>(entity->cCollission->radius / 2);
		e->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
		e->cScore = std::make_shared<CScore>(entity->cScore->score * 2);
	}

}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	//  implement the spawning of a bullet which travels toward target
	// -bullet speed is given as a scalar speed
	// -you must set the velocity by using formula in notes

	auto bullet = m_entities.addEntity("bullet");

	Vec2 speedVector = { target.x - entity->cTransform->pos.x,target.y - entity->cTransform->pos.y };
	speedVector.normalize();
	speedVector *= m_bulletConfig.S;

	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, speedVector, 0);
	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), 
		sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	bullet->cCollission = std::make_shared<CCollision>(m_bulletConfig.CR);
	bullet->cLifespan = std::make_shared<CLifespan>(30);

}

void Game::sUserInput()
{
	// handle user input here
	// note that you should only be setting the player's input component variables here
	// you should not implement the player's movement logic here
	// the movement system will read the variables you set in this function

	sf::Event event;

	while (m_window.pollEvent(event))
	{
		//this event triggers when the windows is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}
		

		//this event is triggered when a key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = true;
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = false;
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{

			if (event.mouseButton.button == sf::Mouse::Left)
			{
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}
			if (event.mouseButton.button == sf::Mouse::Right)
			{

				spawnSpecialWeapon(m_player);
				
				
			}
		}

	}

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	int cooldownFrameTime = 240; // 60 frame per second

	float angle = 360 / (entity->cShape->circle.getPointCount());
	int specialBulletSpeed = 20;
	
	if ((m_currentFrame - m_lastSpecialWeaponUseTime) >= cooldownFrameTime)
	{
		for (size_t i = 0; i < entity->cShape->circle.getPointCount() * 4; i++)
		{

			auto e = m_entities.addEntity("bullet");

			Vec2 speedVector = { specialBulletSpeed * cosf((angle / 3 * (PI / 180)) * i),specialBulletSpeed * sinf((angle / 3 * (PI / 180)) * i) };

			speedVector.normalize();
			speedVector *= specialBulletSpeed;

			e->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), speedVector, 0);
			e->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V,
				sf::Color(m_playerconfig.FR, m_playerconfig.FG, m_playerconfig.FB),
				sf::Color(m_playerconfig.OR, m_playerconfig.OG, m_playerconfig.OB), m_playerconfig.OT);
			e->cCollission = std::make_shared<CCollision>(m_bulletConfig.CR);
			e->cLifespan = std::make_shared<CLifespan>(240);
		}

		m_lastSpecialWeaponUseTime = m_currentFrame;
	}
	
	

	

	
}




