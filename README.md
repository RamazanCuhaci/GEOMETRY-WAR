# GEOMETRY-WAR
Basic SFML game project

Player: 
- The player is represented by a shape which is defined in the config file - The player must spawn in the center of the screen at the beginning of the game, and after it dies (collides with an enemy)
- The player moves by a speed read from the config file in these directions:
  Up: W key, Left: A key, Down: S key, Right: D key
- The player is confined to move only within the bounds of the window
- The player will shoot a bullet toward the mouse pointer when the left mouse button is clicked. The speed, size, and lifespan of the bullets are read from the config file.

Special Ability
- Multiple entities (bullets etc) spawned by special weapon
- Entities have some unique graphic associate with them
- A unique game mechanic is introduced via a new component
- A 'cooldown timer must be implemented for the special weapon 
The properties of the special move are not in the config file.

Enemy (s):
- Enemies will spawn in a random location on the screen every X frames, where X is defined in the configuration file.
- Enemies must not overlap the sides of the screen at the time of spawn.
- Enemies shapes have random number of vertices, between a given minimum and maximum number, which is specified in the config file.
- Enemy shape radius will be specified in the configuration file.
- Enemies will be given a random color upon spawning.
- Enemies will be given a random speed upon spawning, between a minimum and maximum value specified in the config file. - When an enemy reaches the edge of the window, it should bounce off in
the opposite direction at the same speed.
- When (large) enemies collide with a bullet or player, they are destroyed, and N small enemies spawn in its place, where N is the number of vertices of the original enemy. Each small enemy must have the same number of vertices and color of the original enemy. These small entities travel outward at angles at a fixed intervals equal to (360 / vertices).
For example, if the original enemy had 6 sides, the 6 smaller enemies will travel outward in intervals of (360/6)= 60 degrees.

Score:
- Each time an enemy spawns, it is given a score component of N*100, where N is the number of vertices it has. Small enemies get double this value.
- If a player bullet kills an enemy, the game score is increased by the score component of the enemy killed.
- The score should be displayed with the font specified by the config file in the top-left corner of the screen.

Drawing:
- In the render system, all entities should be given a slow rotation, which makes the game look a little nicer.
- Any Entity with a lifespan is currently alive, it should have its Color alpha channel set to a ratio depending on how long it has left to live. For example, if an Entity has a 100 frame life span, and it has been alive for 50 frames, its alpha value should be set to 0.5* 255. The alpha should go from 255 when it is first spawned, to 0 on the last frame it is alive.


Enemy Specification:
- Enemy SR CR SMIN SMAX OR OG OB OT VMIN VMAX L SP
- Shape Radius		SR 		int
- Collision Radius	CR		int
- Min/Max Speed		SMIN, SMAX	float, float
- Outline Color		OR, OG, OB	int, int, int
- Outline Thickness	OT		int
- Min/Max Vertices	VMIN, VMAX	int, int
- Small Lifespan		L		int
- Spawn Interval		SP		int

Fon Specification:
- Font FS RGB
- This lines defines the font which is to be used to draw text for this program. The format of the line is as follows:
- Font File	F 	std::string (it will have no spaces)
- Font Size	S	int
- RGB Color	(R,G,B) int, int, int

Player Specification:
- Player SR CR FR FG FB OR OG OB OT V
- Shape Radius		SR		int
- Collision Radius	CR		int
- Speed			S		float
- Fill Color		FR, FG, FB	int, int, int
- Outline Color		OR, OG, OB	int, int, int
- Outline Thickness 	OT		int
- Shape Vertices		V		int

Bullet Specification:
- Bullet SR CR S FR FG FB OR OG OB OT V L
- Shape Radius		SR		int
- Collision Radius	CR		int
- Speed			S		float
- Fill Color		FR, FG, FB	int, int, int
- Outline Color		OR, OG, OB 	int, int, int
- Outline Thickness	OT		int
- Shape Vertices		V		int
-Lifespan		L		int
