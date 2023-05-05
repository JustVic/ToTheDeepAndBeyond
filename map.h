#ifndef map_h
#define map_h

#include "gamestate.h"
#include <time.h>
#include <map>
#include "enemy.h"

//#include <stdio.h>
//#include <vector>
//#include <iostream>
#include <utility>
#include <functional>

enum bulletType { SIMPLE, ARMOR, ROCKET };
enum type { WALL, TELEPORT, FLOOR, NOM_NOM, CONNECTOR };

typedef struct { float x, y, z; } vec3;
typedef struct { int16_t x, y, z; } veci3;

enum Direction { north, south, west, east };

struct Block
{
	int x;
	int y;
	int w;
	int h;
	int z;
	type tp;

	int region;

	bool operator ==( const Block& p ) const
    	{
        	return (x == p.x)&&(y == p.y);
	}

	bool operator <( const Block& p ) const
    	{
        	if (x == p.x)
		{
			return y < p.y;
		}

		return x < p.x;
    	}
};

Block CreateBlock(int x, int y, int w, int h, int z);

struct Connector
{
    int x;
    int y;
	int w;
	int h;
    int z;
    type tp;

	int region;
	std::vector<int> regions;
};

struct Bullet
{
	int x;
    	int y;
	int w;
   	int h;
    	int z;
	int direction;
	float distance;
	SDL_Point startPoint;

    bulletType tp;
};

typedef struct {
  SDL_FRect rect;
  int health;
  int direction;
  int old_direction;
  float hitTimer;
  int sprite;
  double hitAnimTimer;
  double speed;
  SDL_FRect moveSpeed;
} Player;

class Map
{
    private:
		std::vector<SDL_Rect> currentRooms;
		std::vector<SDL_Rect> nextRooms;

		double mapCenterX;
		double mapCenterY;

		int mazeSize;

		int level;
		
	    	std::vector<SDL_Rect> walls;

		std::vector<SDL_Rect> obstacles;

		std::vector<Bullet> pBullets;

		std::vector<SDL_Rect> eBullets;

	    	std::vector<SDL_Rect> moving_obstacles;

	    	std::vector<Enemy0> enemies;

		Block exit;

		std::vector<SDL_Rect> currentFreeMap;
		std::vector<SDL_Rect> nextFreeMap;

	    	double Speed;
		//standart monsters speed;
		double mSpeed;

		int* currentMap;
		int* nextLevelMap;

		int width;
		int height;
		int widthInCells;
		int heightInCells;

		double cellWidth;

		//int _currentRegion;

		std::vector<Block> _regions;

	    	std::shared_ptr<SDL_Texture> blue_texture;

	    	std::shared_ptr<SDL_Texture> door_texture;

	    	std::shared_ptr<SDL_Texture> orange_texture;

	    	std::shared_ptr<SDL_Texture> green_texture;

	    	std::shared_ptr<SDL_Texture> goblin_texture;


	   	std::shared_ptr<RenderManager> rend;
    public:
		Map(GameEngine* game, Player* player);
		~Map();

		unsigned int ArrangeEnemies(GameEngine* game, std::vector<SDL_Rect>* freeMap, std::vector<Enemy0>* enemies);
		unsigned int ArrangeBlock(std::vector<SDL_Rect>* freeMap, Block* block);
		bool IsFree(int x, int y, int*map);

		void ClearLines(int x, int y);

		std::vector<Block> FreeNeighbours(int x, int y, int* map);

		int Distance(Block start, Block dest);

		int Distance(SDL_Rect start, SDL_Rect dest);

		std::vector<Block> FindPath(int start_x, int start_y, int end_x, int end_y, int* map);

		Block GetNearestMapTile(int x, int y);

		void UpdateMapCenterX(double x);                                         
                void UpdateMapCenterY(double y);                                         
                int GetMapCenterX();                                                  
                int GetMapCenterY();  

		void UpdateEnemies(Player* player, float dt);

	    	void Update(GameEngine* game, Player* player, float dt);

		void Carve(int x, int y, std::vector<SDL_Rect>* freeMap, int* map);

		bool CheckTileWall(int x, int y, int* map); 

		void AddJunction(int x, int y, std::vector<SDL_Rect>* freeMap, int* map);

		//void StartRegion();

		//int GetRegion(int x, int y);

		//void _connectRegions(std::vector<SDL_Rect>* freeMap, int* map);

		bool CanCarve(int x, int y, int direction, int* map); 
		bool CheckOverlaps(SDL_Rect room1, SDL_Rect room2);

		void GrowMaze(int x, int y, std::vector<SDL_Rect>* freeMap, int* map);

		void GenerateNextRound(std::vector<SDL_Rect>* _rooms, std::vector<SDL_Rect>* freeMap, int* map);

		int GenerateNewLevel(GameEngine* game, int n, std::vector<SDL_Rect>* freeMap, int* map, std::vector<SDL_Rect>* _rooms);

		void GenerateDoors(std::vector<SDL_Rect>* freeMap, std::vector<SDL_Rect>* _rooms, int* map);

		void Shoot(Player* player, float press_duration);

		bool IntersectFRect(const SDL_FRect * r1, const SDL_FRect * r2);
		bool IntersectRect(const SDL_Rect * r1, const SDL_Rect * r2);
		
		void MoveToNextCell(double pX, double pY, Enemy0* monster);
		void MoveToLastCell(double pX, double pY, Enemy0* monster);

		bool RectInside(SDL_Rect* rect0, SDL_Rect* rect1);

		bool RectInside(SDL_FRect* rect0, SDL_FRect* rect1);
		
		bool CheckBlockCollision(Block* block, Player* player);

		bool CheckBulletEnemiesCollision(Bullet* bullet);

		bool CheckBulletMonstersCollision(Bullet* bullet);

		bool CheckMonsterCollision(double x, double y, double w, double h, Enemy0* monster);

		//void DrawPlayer(std::shared_ptr<RenderManager>& rend, double x, double y);

		void DrawObstacles(std::shared_ptr<RenderManager>& rend, double x, double y,
							double w, double h);
		void DrawSimpleEnemy(std::shared_ptr<RenderManager>& rend, double x, double y,
							double w, double h);
		void DrawGoblin(std::shared_ptr<RenderManager>& rend, double x, double y,
							double w, double h);
		void DrawExit(std::shared_ptr<RenderManager>& rend, double x, double y,
							double w, double h);

		/*void PlayerMoveDown();
		void PlayerMoveUp();
		void PlayerMoveLeft();
		void PlayerMoveRight();*/
		/*void PlayerStopY();
		void PlayerStopX();*/
		void MonsterHit(Player* player, float dt, int power);
		//bool PlayerDie(Player* player);
		//int GetPlayerHealth();


		bool BulletMapCollision(int level);

		bool Collision();
		bool MonsterCollision(float px, float py, int pw, int ph);
		bool SimpleEnemyCollision(float px, float py, int pw, int ph);

		bool WallCollision(float px, float py, int pw, int ph);

		void Draw(std::shared_ptr<RenderManager>& rend, Player* player);
};

#endif
