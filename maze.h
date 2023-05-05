#include "vector.h"
#include <SDL2/SDL.h>
#include "enemy.h"
#include "types.h"

struct Building {
	int mazeSize;
	int _currentRegion;
	vector _rooms;
	vector freeMap;
	vector _regions;

	int *buildMap;
};

struct Map {
	int cellWidth;
	int widthInCells;
	int heightInCells;
};

void Carve(int x, int y, int level, struct Building* build, struct Map* map);
unsigned int arrangeEnemies(int level, struct Building* build, struct Map* map, vector* vEnemies);
void _addJunction(int x, int y, int level, struct Building* build, struct Map* map);
void _startRegion(struct Building* build);
void resetArray(int* cells, struct Map* map);
bool CanCarve(int x, int y, int direction, int level, struct Map* map);
void GrowMaze(int x, int y, int level, struct Building* build, struct Map* map);
void GenerateDoors(int level, struct Building* build, struct Map* map);
void GenerateNextRound(int level, struct Building* build, struct Map* map);
int GenerateNewLevel(int n, struct Building* build, struct Map* map, vector* vEnemies);

void loki(vector* vec);
