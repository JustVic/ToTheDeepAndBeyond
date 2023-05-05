#include "maze.h"

#include "vector.h"

#define max(x,y) (((x) >= (y)) ? (x) : (y))
#define min(x,y) (((x) <= (y)) ? (x) : (y))

void Carve(int x, int y, int level, struct Building* build, struct Map* map) {

	int w = map->widthInCells;
	int h = map->heightInCells;

	build->buildMap[(level*w*h)+(x*h+y)] = FLOOR;

	if((x<w) && (x>0))
	{
		if((y<h) && (y>0))
		{
			struct Block* bk = malloc(sizeof(struct Block));
			bk->x = x;
			bk->y = y;
			bk->z = level;

			bk->region = build->_currentRegion;

			build->freeMap.pfVectorAdd(&build->freeMap, bk);
		}
	}
}

unsigned int arrangeEnemies(int level, struct Building* build, struct Map* map, vector* vEnemies) {
	printf("singleplayer arrange enemies\n");

	for (int n = 0; n < level+4; n++)
	{
		int total = build->freeMap.pfVectorTotal(&build->freeMap);
		//if(total > 0)
		//{
			printf("TOTAL:%d i:%d\n", total, n);
			int r =0;
			r = rand()%total;

			struct Block bk = *(struct Block*)build->freeMap.pfVectorGet(&build->freeMap, r);

			//free(build.freeMap.pfVectorGet(&build.freeMap, r));
			//build.freeMap.pfVectorDelete(&build.freeMap, r);

			Enemy* enemy_block = malloc(sizeof(Enemy));

			int r1 = rand()%4;
			switch(r1)
			{
				case 0:
					enemy_block->direction = 0;
					break;
				case 1:
					enemy_block->direction = 1;
					break;

				case 2:
					enemy_block->direction = 2;
					break;

				case 3:
					enemy_block->direction = 3;
					break;
			}

			enemy_block->coord.x = bk.x*map->cellWidth;
			enemy_block->coord.y = bk.y*map->cellWidth;

			enemy_block->level = bk.z;

			vEnemies->pfVectorAdd(vEnemies, enemy_block);
		//}

	}

	return 0;
}


void _addJunction(int x, int y, int level, struct Building* build, struct Map* map) {
	printf("Add Junction\n");
	Carve(x, y, level, build, map);
}

void _startRegion(struct Building* build) {
	build->_currentRegion++;
}

void resetArray(int* cells, struct Map* map)
{
	int w = map->widthInCells;
  	int h = map->heightInCells;

	for(int i=0; i<w; i++)
	{
		for(int j=0; j<h; j++)
		{
			cells[i*h+j] = 0;
		}
	}
}

bool CanCarve(int x, int y, int direction, int level, struct Map* map) {

	int xtmp;
	int ytmp;
	int xtmp2;
	int ytmp2;
	int xtmp3;
	int ytmp3;

	switch(direction)
	{
		case north:
			xtmp=x;
			ytmp=y-1;
			xtmp2=x;
			ytmp2=y-2;
			xtmp3=x;
			ytmp3=y-3;

			break;
		case south:
			xtmp=x;
			ytmp=y+1;
			xtmp2=x;
			ytmp2=y+2;
			xtmp3=x;
			ytmp3=y+3;

			break;
		case west:
			xtmp=x-1;
			ytmp=y;

			xtmp2=x-2;
			ytmp2=y;
			xtmp3=x-3;
			ytmp3=y;

			break;
		case east:
			xtmp=x+1;
			ytmp=y;
			xtmp2=x+2;
			ytmp2=y;
			xtmp3=x+3;
			ytmp3=y;

			break;

	}
	
	if ((xtmp3>=map->widthInCells-1) || (ytmp3>=map->heightInCells-1) || (xtmp3<=0) || (ytmp3<=0)) return false;

	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;
	bool flag4 = false;
	bool flag5 = false;

	bool flag6 = false;
	bool flag7 = false;
	bool flag8 = false;
	bool flag9 = false;
	bool flag10 = false;

	if(CheckTileWall(xtmp2, ytmp2, level))
	{
		flag6 = true;
	}

	if(CheckTileWall(xtmp3, ytmp3, level))
	{
		flag7 = true;
	}

	if((flag6)&&(flag7))
		return true;

	return false;
}

void GrowMaze(int x, int y, int level, struct Building* build, struct Map* map)
{
	VECTOR_INIT(cells);
	
    	SDL_Rect lastDir;
    	SDL_Rect lastLastDir;
	SDL_Rect tmp = {-1,-1,0,0};
	lastDir=tmp;
	lastLastDir=tmp;
	int lDir = -1;

	int windingPercent = 30;
	build->mazeSize = 0;
    
	_startRegion(build);
	Carve(x, y, level, build, map);
	build->mazeSize++;

	SDL_Rect* start = malloc(sizeof(SDL_Rect));
	start->x = x;
        start->y = y;
	start->w = 0;
	start->h = 0;

	cells.pfVectorAdd(&cells, start);

    	SDL_Rect cell;
	VECTOR_INIT(unmadeCells);

	while (cells.pfVectorTotal(&cells)>0) 
	{
		cell = *(SDL_Rect*)cells.pfVectorGet(&cells, cells.pfVectorTotal(&cells)-1);
  
		while(unmadeCells.pfVectorTotal(&unmadeCells)>0)
	  	{
			int i = unmadeCells.pfVectorTotal(&unmadeCells)-1;
			free(unmadeCells.pfVectorGet(&unmadeCells, i));
			unmadeCells.pfVectorDelete(&unmadeCells, i);
	  	}

	  	for (int dir1 = north; dir1 <= east; dir1++)
	  	{

			//printf("while cxells%d\n", build.mazeSize);

			if (CanCarve(cell.x, cell.y, dir1, level, map))
			{
				SDL_Rect* ucell = malloc(sizeof(SDL_Rect));
				ucell->x =cell.x;
			        ucell->y =cell.y;

				switch(dir1)
				{
					case north:
						ucell->y-=1;	
						//std::cout << "NORTH!!" << std::endl;
						printf("NORTH");
						unmadeCells.pfVectorAdd(&unmadeCells, ucell);
						break;
					case south:
						ucell->y+=1;
						//std::cout << "SOUTH!!" << std::endl;
						printf("SOUTH");
						unmadeCells.pfVectorAdd(&unmadeCells, ucell);
						break;
					case west:
						ucell->x-=1;
						//std::cout << "WEST!!" << std::endl;
						printf("WEST");
						unmadeCells.pfVectorAdd(&unmadeCells, ucell);
						break;
					case east:
						ucell->x+=1;
						//std::cout << "EAST!!" << std::endl;
						printf("EAST");
						unmadeCells.pfVectorAdd(&unmadeCells, ucell);
						break;
				}
			}
	  	}

      		if (unmadeCells.pfVectorTotal(&unmadeCells) > 0) 
      		{
        		// Based on how "windy" passages are, try to prefer carving in the
        		// same direction.
        		SDL_Rect dir;
			bool isflag = false;
			int tx;
			int ty;

			for(int i=0; i<unmadeCells.pfVectorTotal(&unmadeCells); i++)
			{
				SDL_Rect it = *(SDL_Rect*)unmadeCells.pfVectorGet(&unmadeCells, i);
				switch(lDir)
				{
					case 0:
						tx = cell.x;
						ty = cell.y - 1;
						break;
					case 1:
						tx = cell.x;
						ty = cell.y + 1;
						break;
					case 2:
						tx = cell.x - 1;
						ty = cell.y;
						break;
					case 3:
						tx = cell.x + 1;
						ty = cell.y;
						break;
					default:
						tx = -999;
						ty = -999;
						break;

				}

				if ((it.x == tx) && (it.y == ty))
				{
					isflag = true;
					break;
				}

			}
			if((isflag) && (rand()%100 > windingPercent)) {
				dir.x = tx;
				dir.y = ty;
			}
			else
			{
				int i = rand()%(unmadeCells.pfVectorTotal(&unmadeCells));
				dir = *(SDL_Rect*)unmadeCells.pfVectorGet(&unmadeCells, i);
			}

			int k = dir.x - cell.x;
			int l = dir.y - cell.y;

			if(k > 0)
				lDir = 3;

			if(k < 0)
				lDir = 2;

			if(l > 0)
				lDir = 1;

			if(l < 0)
				lDir = 0;

        		Carve(dir.x, dir.y, level, build, map);
			build->mazeSize++;

			SDL_Rect dir2;
			dir2.x = dir.x + k;
			dir2.y = dir.y + l;

        		Carve(dir2.x, dir2.y, level, build, map);
			build->mazeSize++;

			SDL_Rect* temp2 = malloc(sizeof(SDL_Rect));
			temp2->x = dir2.x;
			temp2->y = dir2.y;
			temp2->w = 0;
			temp2->h = 0;
			cells.pfVectorAdd(&cells, temp2);

        		lastDir = dir;
      		} else {
        		// No adjacent uncarved cells.
			if(cells.pfVectorTotal(&cells)>0)
			{
        			cells.pfVectorDelete(&cells, cells.pfVectorTotal(&cells)-1);
			}
	
        		// This path has ended.
        		lastDir = tmp;
			lDir = -1;
		}
	}

	unmadeCells.pfVectorFree(&unmadeCells);
	cells.pfVectorFree(&cells);
}

void GenerateDoors(int level, struct Building* build, struct Map* map)
{
	int d = rand()%2;//8

	for(int j = 0; j<build->_rooms.pfVectorTotal(&build->_rooms);j++)
      	{

	SDL_Rect room = *(SDL_Rect*)build->_rooms.pfVectorGet(&build->_rooms, j);
	int x = room.x-1;
	int x2 = room.x+room.w;
	int y = room.y-1;
	int y2 = room.y+room.h;

	bool flag = false;
	int count = 0;

	while((!flag) && (count<10))
	{
		int r1 = rand()%4;
		int r2 = 0;
		if(r1 == 0)
		{
			r2 = rand()%room.h;
			if(!CheckTileWall(x-1, room.y+r2, level))
			{
				if(x!=0)
				{
					_addJunction(x, room.y+r2, level, build, map);
					flag = true;
				}
			}
		}
		if(r1 == 1)
		{
			r2 = rand()%room.w;
			if(!CheckTileWall(room.x+r2, y-1, level))
			{
				if(y!=0)
				{
					_addJunction(room.x+r2, y, level, build, map);
					flag = true;
				}
			}
		}
		if(r1 == 2)
		{
			r2 = rand()%room.h;
			if(!CheckTileWall(x2+1, room.y+r2, level))
			{
				if(x2!=map->widthInCells-1)
				{
					_addJunction(x2, room.y+r2, level, build, map);
					flag = true;
				}
			}
		}
		if(r1 == 3)
		{
			r2 = rand()%room.w;
			if(!CheckTileWall(room.x+r2, y2+1, level))
			{
				if(y2!=map->heightInCells-1)
				{
					_addJunction(room.x+r2, y2, level, build, map);
					flag = true;
				}
			}
		}
		count++;
	}

	}

	int w = map->widthInCells;
	int h = map->heightInCells;
	

//	d = 8;
	
	while(d>0)
	{
	int d2 = rand()%((w-1)*(h-1));

  	for (int i = 0; i < w; i++) 
  	{
        	for (int j = 0; j < h; j++)
        	{		

		int count2 = 0;
		if(!CheckTileWall(i, j-1, level))
		{
			count2++;
		}

		if(!CheckTileWall(i-1, j, level))
		{
			count2++;
		}

		if(!CheckTileWall(i+1, j, level))
		{
			count2++;
		}

		if(!CheckTileWall(i, j+1, level))
		{
			count2++;
		}

		if(CheckTileWall(i, j-1, level))
		{
			count2--;
		}

		if(CheckTileWall(i-1, j, level))
		{
			count2--;
		}

		if(CheckTileWall(i+1, j, level))
		{
			count2--;
		}

		if(CheckTileWall(i, j+1, level))
		{
			count2--;
		}

		if(CheckTileWall(i, j, level))
		{

			if((count2==0)&&(d>0)&&(d2<(i*h+j)))
			{
				_addJunction(i,j, level, build, map);
				d--;
				d2 = 9999;
			}
		}



		}
	}
	}

}

void GenerateNextRound(int level, struct Building* build, struct Map* map)
{
	int numRoomTries = 10;
	if(map->widthInCells>map->heightInCells)
		numRoomTries = map->widthInCells;
	else
		numRoomTries = map->heightInCells;

	int roomExtraSize = 2;

	SDL_Rect bounds = {0, 0, map->widthInCells-1, map->heightInCells-1};

	build->_rooms.pfVectorFree(&build->_rooms);

	vector_init(&build->_rooms);

	for (int i = 0; i < numRoomTries; i++) {

		int size = (rand()%((1, 3 + roomExtraSize))) * 2 + 1;
		int rectangularity = (rand()%((0, 1 + size / 2))) * 2;
		int width = size;
		int height = size;

		if (rand()%2) {
			width += rectangularity;
		} else {
			height += rectangularity;
		}

		if((width<2)||(height<2))
		{
			continue;
		}

		int x = (rand()%(bounds.w - width)/2) *2 +1;
		int y = (rand()%(bounds.h - height)/2) *2 +1;

		SDL_Rect* room = malloc(sizeof(SDL_Rect));
		room->x = x;
		room->y = y;
		room->w = width;
		room->h = height;
		SDL_Rect room2 = {x-1, y-1, width+2, height+2};

		bool overlaps = false;

		for(int j = 0; j<build->_rooms.pfVectorTotal(&build->_rooms);j++)
		{
			SDL_Rect other = *(SDL_Rect*)build->_rooms.pfVectorGet(&build->_rooms, j);
			SDL_Rect room3 = {other.x-1, other.y-1, other.w+2, other.h+2};

			if(SDL_HasIntersection(&room2, &room3)==true)
			{
          			overlaps = true;
          			break;
        		}
      		}

		if (overlaps) continue;

		build->_rooms.pfVectorAdd(&build->_rooms, room);

      		_startRegion(build);

		for (int xtmp = x; xtmp < (x+width); xtmp++) {

			for(int ytmp = y; ytmp < (y+height); ytmp++)
			{
				Carve(xtmp, ytmp, level, build, map);
			}
		}   
	}
}

int GenerateNewLevel(int n, struct Building* build, struct Map* map, vector* vEnemies) {

	int w = map->widthInCells;
	int h = map->heightInCells;

	int level = n;

	build->mazeSize = 0;

	build->_currentRegion = 0;

	while(build->mazeSize < 50)
  	{
		int total = build->freeMap.pfVectorTotal(&build->freeMap);
		for(int i=0; i<total; i++)
		{
			//free(build->freeMap.pfVectorGet(&build->freeMap, i));
		}
		
		build->freeMap.pfVectorFree(&build->freeMap);
		vector_init(&build->freeMap);


  		for(int i=0; i<(w*h); i++)
  		{
			build->buildMap[(n*w*h)+i] = WALL;
  		}

		GenerateNextRound(level, build, map);
		
  		GrowMaze(1, 1, level, build, map);
  	}

	GenerateDoors(level, build, map);

	arrangeEnemies(n, build, map, vEnemies);
}

struct pricedTile{
	SDL_Point point;
	int price;
};

struct pricedTile* FindPricedTileInVec(vector* vec, SDL_Point pointp)
{

	int temp = vec->pfVectorTotal(vec);

	for(int i = 0; i < temp; i++)
        {
		struct pricedTile* node = (struct pricedTile*)vec->pfVectorGet(vec, i);

		if((node->point.x == pointp.x) && (node->point.y == pointp.y))
			return node;
	}

	return NULL;
}

struct SDL_Point* FindPointInVec(vector* vec, SDL_Point pointp)
{

	int temp = vec->pfVectorTotal(vec);

	for(int i = 0; i < temp; i++)
        {
		struct pricedTile* node = (struct pricedTile*)vec->pfVectorGet(vec, i);

		if((node->point.x == pointp.x) && (node->point.y == pointp.y))
			return node;
	}

	return NULL;
}

bool VecPointContains(vector* vec, SDL_Point pointp)
{

	int size = vec->pfVectorTotal(vec);

	for(int i = 0; i < size; i++)
        {
		SDL_Point* node = (struct SDL_Point*)vec->pfVectorGet(vec, i);

		if((node->x == pointp.x) && (node->y == pointp.y))
			return true;
	}

	return false;
}

bool VecDoublePointContains(vector* vec, SDL_Point pointp)
{

	int size = vec->pfVectorTotal(vec);

	for(int i = 0; i < size; i++)
        {
		SDL_Point* node = (struct SDL_Point*)vec->pfVectorGet(vec, i);

		if((node->x == pointp.x) && (node->y == pointp.y))
			return true;
	}

	return false;
}

int Distance(SDL_Point start, SDL_Point dest)
{

   int x_1 = start.x;
   int x_2 = dest.x;
   int y_1 = start.y;
   int y_2 = dest.y;

   int dx = x_2 - x_1 - floor(y_2/2) + floor(y_1/2);
   int dy = y_2 - y_1;

    if(((dx > 0) - (dx < 0)) == ((dy > 0) - (dy < 0)))
     return abs(dx + dy);
    else
     return max(abs(dx), abs(dy));
}

int VecPointFindIndex(vector* vec, SDL_Point pointp)
{

	int size = vec->pfVectorTotal(vec);

	for(int i = 0; i < size; i++)
        {
		SDL_Point* node = (SDL_Point*)vec->pfVectorGet(vec, i);

		if((node->x == pointp.x) && (node->y == pointp.y))
			return i;
	}

	return -1;
}

struct DoublePoint {
	SDL_Point point0;
	SDL_Point point1;
};

struct DoublePoint* FindPointInDoublePointVec(vector* vec, SDL_Point* pointp)
{
	int size = vec->pfVectorTotal(vec);

	for(int i = 0; i < size; i++)
        {
		struct DoublePoint* node = (struct DoublePoint*)vec->pfVectorGet(vec, i);

		if((node->point1.x == pointp->x) && (node->point0.y == pointp->y))
			return node;
	}

	return NULL;
}

bool ComparePoints(SDL_Point* point0, SDL_Point* point1)
{
	if((point0->x == point1->x) && (point0->y == point1->y))
	{
		return true;
	}

	return false;
}

struct Entity {
	vec3 pos;
	int entity_id;
	int type;
	int direction;
	bool fire;
	int fire_duration;
};



bool IsFree(vector vMap, int x, int y, int z, int mapWidth, int mapHeight)
{
	int temp = vMap.pfVectorTotal(&vMap);
        for(int i = 0; i < temp; i++)
        {
		struct Entity* ent0 = (struct Entity*)vMap.pfVectorGet(&vMap, i);

		if(ent0->pos.x == x)
		{
			if(ent0->pos.y == y)
			{
				if(ent0->pos.z == z)
				{
					if(ent0->type == WALL)
					{
						return false;	
					}
				}
			}
		}
	}

	return true;

	if(!(x>=0&&y>=0&&x<=(mapWidth-1)&&y<=(mapHeight-1)))
	{
        	return false;
	}

    	return true;
}


vector FreeNeighbours(vector* vMap, int x, int y, int z, int mapWidth, int mapHeight)
{
	vector vZone;

	if(IsFree(*vMap, x-1,y,z, mapWidth, mapHeight))
	{
		SDL_Point point0;
		point0.x = x-1;
		point0.y = y;
		vZone.pfVectorAdd(&vZone, &point0);
	}
	if(IsFree(*vMap, x+1,y,z, mapWidth, mapHeight))
       	{
		SDL_Point point0;
		point0.x = x+1;
		point0.y = y;
		vZone.pfVectorAdd(&vZone, &point0);
	}
       	if(IsFree(*vMap, x,y+1,z, mapWidth, mapHeight))
       	{
        	SDL_Point point0;
		point0.x = x;
		point0.y = y+1;
		vZone.pfVectorAdd(&vZone, &point0);
       	}
       	if(IsFree(*vMap, x,y-1,z, mapWidth, mapHeight))
       	{
         	SDL_Point point0;
		point0.x = x;
		point0.y = y-1;
		vZone.pfVectorAdd(&vZone, &point0);
       	}


    return vZone;
}

vector loki(vector* Map)
{	
	//VECTOR_INIT(v);

	vector v2;
	//vector_init(&v2);
	printf("ADDRESS%d", *(*Map).pfVectorTotal);
	//int tempZ = Map->pfVectorTotal(Map);

	return v2;
}

vector AStarSearch(vector* vMap, SDL_Point startp, SDL_Point finishp, int z, int mapWidth, int mapHeight)
{
	int tempZ = vMap->pfVectorTotal(vMap);

	vector vG;
	vector vF;

	vector vCameFrom;
	vector vClosed;
	vector vOpen;
	
	vector_init(&vG);
    	vector_init(&vF);
    	vector_init(&vCameFrom);
	vector_init(&vClosed);
	vector_init(&vOpen);

		
	SDL_Point* point0 = malloc(sizeof(SDL_Point));
	*point0 = startp;
	
	vOpen.pfVectorAdd(&vOpen, point0);//??????????????????????

	//vector vResultPath;
	vector vNeighbours;

	//vector_init(&vResultPath);
	VECTOR_INIT(vResultPath);

	vector_init(&vNeighbours);

	SDL_Point current;

	vector vCurrentPath;
	vector_init(&vCurrentPath);

	
	SDL_Point* node = (SDL_Point*)vOpen.pfVectorGet(&vOpen, vOpen.pfVectorTotal(&vOpen)-1);

	struct pricedTile* tempPT = malloc(sizeof(struct pricedTile*));
	tempPT->point = *node;
	tempPT->price = 0;
	
	vG.pfVectorAdd(&vG, tempPT);
	SDL_Point start;
	SDL_Point end;

	start = startp;
	end =  finishp;

       	while((vOpen.pfVectorTotal(&vOpen) > 0))
	{
		node = (SDL_Point*)vOpen.pfVectorGet(&vOpen, vOpen.pfVectorTotal(&vOpen)-1);

		//find lowest score in open
		int size0 = vOpen.pfVectorTotal(&vOpen);
        	for(int i = 0; i < size0; i++)
        	{
			SDL_Point* nodeTemp = (SDL_Point*)vOpen.pfVectorGet(&vOpen, i);

			struct pricedTile* tempp0 = FindPricedTileInVec(&vF, *node);
			struct pricedTile* tempp1 = FindPricedTileInVec(&vF, *node);

			int score_1;
			int score_2;

			if(tempp0 != NULL)
           			score_1 = tempp0->price;
			else
				score_1 = 0;
			if(tempp0 != NULL)
           			score_2 = tempp1->price;
			else
				score_2 = 0;


           		if(score_1 < score_2)
           		{
                		node = nodeTemp;
           		}
        	}

		        //if current == end
        	if(ComparePoints(node, &end))
        	{
			vResultPath.pfVectorAdd(&vResultPath, node);

            		bool flag = true;

            		SDL_Point* point3 = node;

			while(flag)
			{
				if(VecDoublePointContains(&vCameFrom, *point3))
				{
					SDL_Point endT = *(SDL_Point*)vOpen.pfVectorGet(&vOpen, 0);
		
					if(!ComparePoints(FindPointInDoublePointVec(&vCameFrom, point3), &endT))
					{
						*point3 = FindPointInDoublePointVec(&vCameFrom, point3)->point0;

						vResultPath.pfVectorAdd(&vResultPath, &point3);
					}
					else
					{
						flag=false;
						break;
					}
				}
				else
				{
					flag = false;
					break;
				}
			}

			return vResultPath;
		}

		int ind = VecPointFindIndex(&vOpen, *node);
		vOpen.pfVectorDelete(&vOpen, ind); //erase(std::find(open.begin(), open.end(), node));

        	vClosed.pfVectorAdd(&vClosed, node);

        	vNeighbours = FreeNeighbours(vMap, node->x, node->y, z, mapWidth, mapHeight);


		int size1 = vNeighbours.pfVectorTotal(&vNeighbours);

		for(int i =0; i<size1; i++)
        	{
			SDL_Point* nodeTemp = (SDL_Point*)vNeighbours.pfVectorGet(&vNeighbours, i);

			if(FindPointInVec(&vClosed, *nodeTemp) != NULL)
				continue;

            		bool neighborInOpenSet = VecPointContains(&vOpen, *nodeTemp);

            		int tentative_g_score = FindPricedTileInVec(&vG,* node)->price+Distance(*node, *nodeTemp);

            		if((!neighborInOpenSet) || (tentative_g_score < FindPricedTileInVec(&vG, *node)->price))
            		{
				struct DoublePoint* tempDP = malloc(sizeof(struct DoublePoint));
				tempDP->point0 = *nodeTemp;
				tempDP->point1 = *node;

                		vCameFrom.pfVectorAdd(&vCameFrom, tempDP);
				if(vCameFrom.pfVectorTotal(&vCameFrom)>30)
				{
					vResultPath.pfVectorFree(&vResultPath);
					return vResultPath;
				}

				struct pricedTile* tempPT0 = malloc(sizeof(struct pricedTile));
				tempPT0->point = *node;
				tempPT0->price = tentative_g_score;

				vG.pfVectorAdd(&vG, tempPT0);
		
				struct pricedTile* tempPT1 = malloc(sizeof(struct pricedTile));
				tempPT1->point = *node;
				tempPT1->price = tentative_g_score+Distance(*nodeTemp, end);

				vG.pfVectorAdd(&vG, tempPT1);

                		if(!neighborInOpenSet)
					vOpen.pfVectorAdd(&vOpen, nodeTemp);
            		}
        	}
	}

	return vResultPath;
}
