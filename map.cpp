#include "map.h"
#include "enemy.h"


Block CreateBlock(int x, int y, int w, int h, int z){
  Block bk;
  bk.x = x;
  bk.y = y;

  return bk;
};

Map::Map(GameEngine* game, Player* player)
{
	std::cout << "Map Initialization start" << std::endl;

	blue_texture = game->GetRend()->createTexture("data/quadb.bmp");

	door_texture = game->GetRend()->createTexture("data/door.bmp");

	orange_texture = game->GetRend()->createTexture("data/quado.bmp");

	green_texture = game->GetRend()->createTexture("data/quad.bmp");

	goblin_texture = game->GetRend()->createTexture("data/quadog.bmp");

	width = game->GetRend()->GetWidth();
	height = game->GetRend()->GetHeight();

	cellWidth = game->GetRend()->GetCellWidth();

	std::cout << "Width " << width << std::endl;
	std::cout << "Height " << height << std::endl;

//	int center = game->GetRend()->GetWidth()/2;
//	int centerW = game->GetRend()->GetWidth()/2;
//	int centerH = game->GetRend()->GetHeight()/2;

	int w = widthInCells = 20;
	int h = heightInCells = 20;

	std::cout << "Player Initialisation" << std::endl;

	SDL_FRect rec = {cellWidth+cellWidth/4, cellWidth+cellWidth/4, cellWidth/2, cellWidth/2};
	
	player->rect = rec;
	player->health = 10;
	player->sprite = 0;
	player->speed = ((double)cellWidth)/26;
	player->direction = 1;

	mapCenterX = width/2;
	mapCenterY = height/2;

	mazeSize = 0;

	std::cout << "Map Init" << std::endl;

	nextLevelMap = (int*)malloc((1)*(w * h) * sizeof(int));

	level = 0;

	Speed = ((double)cellWidth)/68;

	mSpeed = player->speed - player->speed/3;

//	_currentRegion = 0;

	std::cout << "Level Init start" << std::endl;

	int readyFlag = false;
	while(!readyFlag)
	{
		nextRooms.clear();
		nextFreeMap.clear();
		enemies.clear();

    		GenerateNewLevel(game, 0, &nextFreeMap, nextLevelMap, &nextRooms);

		ClearLines(1, 1);
		currentMap = nextLevelMap;
		currentFreeMap = nextFreeMap;
		currentRooms = nextRooms;

		Block bl0 = GetNearestMapTile(exit.x, exit.y);
		Block bl1 = GetNearestMapTile(player->rect.x, player->rect.y);

		//Just additional check
		std::vector<Block> path = FindPath(bl1.x, bl1.y, bl0.x, bl0.y, currentMap);
		if(path.size()>0)
			readyFlag = true;
	}


	std::cout << "Map Initialized" << std::endl;
}

Map::~Map()//+
{
	delete currentMap;
}

int Map::GenerateNewLevel(GameEngine* game, int n, std::vector<SDL_Rect>* freeMap, int* map, std::vector<SDL_Rect>* _rooms)//+
{
	std::cout << "New Level Generation " << std::endl;

	int w = widthInCells;
	int h = heightInCells;

	mazeSize = 0;
	
	//_currentRegion = 0;

	while(mazeSize < 20)
  	{
		freeMap->clear();

  		for(int i=0; i<(w*h); i++)
  		{
			map[(0*w*h)+i] = WALL;
  		}

		std::cout << "Generate Next Round " << std::endl;
		GenerateNextRound(_rooms, freeMap, map);
		
		std::cout << "GrowMaze " << std::endl;
  		GrowMaze(1, 1, freeMap, map);
  	}
	std::cout << "GenerateDoors " << std::endl;
	GenerateDoors(freeMap, _rooms, map);
	std::cout << "Finish level init " << std::endl;

	//_connectRegions(freeMap, map);

	ArrangeEnemies(game, freeMap, &enemies);
	ArrangeBlock(freeMap, &exit);
	//}
}

void Map::GenerateDoors(std::vector<SDL_Rect>* freeMap, std::vector<SDL_Rect>* _rooms, int* map)//+
{
	int d = 0;//rand()%2;

	std::cout << "Generate Doors." << std::endl;

	for(int k = 0; k<_rooms->size();k++)
      	{
		SDL_Rect room = _rooms->at(k);
		int x = room.x-1;
		int x2 = room.x+room.w;
		int y = room.y-1;
		int y2 = room.y+room.h;

		bool flag = false;
		int count = 0;

		while((!flag) && (count<20))//10
		{
			std::cout << "Generate Doors first loop" << std::endl;
				
			int r1 = rand()%4;
			int r2 = 0;
			if(r1 == 0)
			{
				r2 = rand()%room.h;
				if(!CheckTileWall(x-1, room.y+r2, map))
				{
					if(x!=0)
					{
						AddJunction(x, room.y+r2, freeMap, map);
						flag = true;
					}
				}
			}
			if(r1 == 1)
			{
				r2 = rand()%room.w;
				if(!CheckTileWall(room.x+r2, y-1, map))
				{
					if(y!=0)
					{
						AddJunction(room.x+r2, y, freeMap, map);
						flag = true;
					}
				}
			}
			if(r1 == 2)
			{
				r2 = rand()%room.h;
				if(!CheckTileWall(x2+1, room.y+r2, map))
				{
					if(x2!=widthInCells-1)
					{
						AddJunction(x2, room.y+r2, freeMap, map);
						flag = true;
					}
				}
			}
			if(r1 == 3)
			{
				r2 = rand()%room.w;
				if(!CheckTileWall(room.x+r2, y2+1, map))
				{
					if(y2!=heightInCells-1)
					{
						AddJunction(room.x+r2, y2, freeMap, map);
						flag = true;
					}
				}
			}
			count++;
		}
	}

	int w = widthInCells;
	int h = heightInCells;
	
	while(d>0)
	{
		std::cout << "Generate Doors second loop" << std::endl;

		int d2 = rand()%((w-1)*(h-1));

  		for (int i = 0; i < w; i++) 
  		{
        		for (int j = 0; j < h; j++)
        		{		
				int count2 = 0;
				if(!CheckTileWall(i, j-1, map))
				{
					count2++;
				}

				if(!CheckTileWall(i-1, j, map))
				{
					count2++;
				}

				if(!CheckTileWall(i+1, j, map))
				{
					count2++;
				}

				if(!CheckTileWall(i, j+1, map))
				{
					count2++;
				}

				if(CheckTileWall(i, j-1, map))
				{
					count2--;
				}

				if(CheckTileWall(i-1, j, map))
				{
					count2--;
				}

				if(CheckTileWall(i+1, j, map))
				{
					count2--;
				}

				if(CheckTileWall(i, j+1, map))
				{
					count2--;
				}

				if(CheckTileWall(i, j, map))
				{
					if(((count2==0)&&(d>0))&&(d2<(i*h+j)))
					{
						AddJunction(i,j, freeMap, map);
						d--;
						d2 = 9999;
					}
				}
			}
		}
	}
}

unsigned int Map::ArrangeBlock(std::vector<SDL_Rect>* freeMap, Block* block) {
	
	int total = freeMap->size();

	int r0 = 0;
	r0 = rand()%total;

	SDL_Rect rect0 = freeMap->at(r0);

	freeMap->erase(freeMap->begin() + r0);

	block->x = rect0.x*cellWidth;
	block->y = rect0.y*cellWidth;
	block->w = cellWidth;
	block->h = cellWidth;	

	return 0;
}

void Map::ClearLines(int x, int y) {

	/*for (auto it = monsters.begin();it!=monsters.end();)
	{
		if(((*it)->GetPositionX()/cellWidth == x)||((*it)->GetPositionY()/cellWidth == y))
			it = monsters.erase(it);
		else
			it++;
	}*/

	for(auto it=enemies.begin(); it!=enemies.end();)
	{
		if((it->GetPositionX()/cellWidth == x)||(it->GetPositionY()/cellWidth == y))
			it = enemies.erase(it);
		else
			it++;
	}
}

/*int Map::GetPlayerHealth(Player* player) {
	return player.health;
}*/

unsigned int Map::ArrangeEnemies(GameEngine* game, std::vector<SDL_Rect>* freeMap, std::vector<Enemy0>* enemies) {
	
	std::cout << "Arrange enemies" << std::endl;

	for (int n = 0; n < level+8; n++)
	{
		int total = freeMap->size();

		int r0 = 0;
		r0 = rand()%total;

		SDL_Rect rect1 = freeMap->at(r0);
		//freeMap->erase(freeMap->begin() + r0);

		int r1 = rand()%2;
		if(r1)
		{
			Enemy0 enem0;// = new Enemy0();

			int r1 = rand()%4;
			switch(r1)
			{
				case 0:
					enem0.SetDirection(0);
					break;
				case 1:
					enem0.SetDirection(1);
					break;

				case 2:
					enem0.SetDirection(2);
					break;

				case 3:
					enem0.SetDirection(3);
					break;
			}
			int x = rect1.x*cellWidth;
			int y = rect1.y*cellWidth;
			enem0.SetPositionX(x);
			enem0.SetPositionY(y);
			enem0.SetWidth(cellWidth);
			enem0.SetHeight(cellWidth);

			enem0.SetType(simple);
			enem0.SetMind(silly);

			enemies->push_back(enem0);
		}
		else{
			Enemy0 enem1;

			int x = rect1.x*cellWidth+cellWidth/4;
			int y = rect1.y*cellWidth+cellWidth/4;
			enem1.SetPositionX(x);
			enem1.SetPositionY(y);
			enem1.SetWidth(cellWidth/2);
			enem1.SetHeight(cellWidth/2);
			enem1.SetHitTimer(1.9);


			enem1.SetType(goblin);
			enem1.SetMind(common);

			enemies->push_back(enem1);
		}

	}
	return 0;
}

void Map::GenerateNextRound(std::vector<SDL_Rect>* _rooms, std::vector<SDL_Rect>* freeMap, int* map)//+
{
	int numRoomTries = 10;
	if(widthInCells > heightInCells)
		numRoomTries = widthInCells;
	else
		numRoomTries = heightInCells;

	int roomExtraSize = 0;//2

	SDL_Rect bounds = {0, 0, widthInCells-1, heightInCells-1};

	_rooms->clear();

	for (int i = 0; i < numRoomTries; i++) {

		int size = (rand()%((1, 3 + roomExtraSize))) * 2 + 1;
		int rectangularity = (rand()%((0, 1 + size / 2))) * 2;
		int widthr = size;
		int heightr = size;

		if (rand()%2) {
			widthr += rectangularity;
		} else {
			heightr += rectangularity;
		}

		if((widthr<2)||(heightr<2))
		{
			continue;
		}

		int x = (rand()%(bounds.w - widthr)/2) *2 +1;
		int y = (rand()%(bounds.h - heightr)/2) *2 +1;

		SDL_Rect room = {x, y, widthr, heightr};
		SDL_Rect room2 = {x-1, y-1, widthr+2, heightr+2};

		bool overlaps = false;

		for(int j = 0; j<_rooms->size();j++)
		{
			SDL_Rect other = _rooms->at(j);
			SDL_Rect room3 = {other.x-1, other.y-1, other.w+2, other.h+2};

			if(SDL_HasIntersection(&room2, &room3)==true)
			{
          			overlaps = true;
          			break;
        		}
      		}

		if (overlaps)
		       continue;

		_rooms->push_back(room);

      		//StartRegion();

		for (int xtmp = x; xtmp < (x+widthr); xtmp++) 
		{
			for(int ytmp = y; ytmp < (y+heightr); ytmp++)
			{
				Carve(xtmp, ytmp, freeMap, map);
			}
		}   
	}
}

bool Map::RectInside(SDL_Rect* rect0, SDL_Rect* rect1)	
{
	SDL_Point point0 = {rect0->x, rect0->y};
	SDL_Point point1 = {rect0->x+rect0->w, rect0->y};
	SDL_Point point2 = {rect0->x, rect0->y+rect0->h};
	SDL_Point point3 = {rect0->x+rect0->w, rect0->y+rect0->h};

	if(SDL_PointInRect(&point0, rect1) && SDL_PointInRect(&point1, rect1) && SDL_PointInRect(&point2, rect1) && SDL_PointInRect(&point3, rect1))
		return true;

	return false;
}

bool Map::RectInside(SDL_FRect* rect0, SDL_FRect* rect1)	
{
	SDL_FPoint point0 = {rect0->x, rect0->y};
	SDL_FPoint point1 = {rect0->x+rect0->w, rect0->y};
	SDL_FPoint point2 = {rect0->x, rect0->y+rect0->h};
	SDL_FPoint point3 = {rect0->x+rect0->w, rect0->y+rect0->h};

	if(SDL_PointInFRect(&point0, rect1) && SDL_PointInFRect(&point1, rect1) && SDL_PointInFRect(&point2, rect1) && SDL_PointInFRect(&point3, rect1))
		return true;

	return false;
}

bool Map::CheckBlockCollision(Block* block, Player* player)
{	
	SDL_Rect rect0;
	rect0.w = block->w;
	rect0.h = rect0.w;
	rect0.x = block->x;
	rect0.y = block->y;

	SDL_Rect rec1;
	rec1.x = player->rect.x;
	rec1.y = player->rect.y;
	rec1.w = cellWidth/2;
	rec1.h = cellWidth/2;

	if(SDL_HasIntersection(&rect0, &rec1))
	{
		return true;
	}
	return false;
}

bool Map::CheckBulletEnemiesCollision(Bullet* bullet)
{
		SDL_Rect rect0;
		rect0.w = bullet->w;
		rect0.h = rect0.w;
		rect0.x = bullet->x+mapCenterX;
		rect0.y = bullet->y+mapCenterY;

		bool flagRemove = false;
		for(auto it1=enemies.begin(); it1!=enemies.end();)
		{
			SDL_Rect rect1;
			rect1.w = cellWidth;
			rect1.h = rect1.w;
			rect1.x = it1->GetPositionX()+mapCenterX;//+mapCenterX;
			rect1.y = it1->GetPositionY()+mapCenterY;//j*cellWidth+mapCenterY;

			if(SDL_HasIntersection(&rect0, &rect1))
			{
				//pBullets.erase(bullet);
				enemies.erase(it1);
				flagRemove = true;

				break;
			}
			else
				it1++;

			if(flagRemove)
				break;
		}
}

bool Map::CheckBulletMonstersCollision(Bullet* bullet)
{
	SDL_Rect rect0;
	rect0.w = bullet->w;
	rect0.h = rect0.w;
	rect0.x = bullet->x+mapCenterX;
	rect0.y = bullet->y+mapCenterY;

	bool flagRemove = false;
	/*for(auto it1=monsters.begin(); it1!=monsters.end();)
	{
		SDL_Rect rect1;
		rect1.w = cellWidth/2;
		rect1.h = rect1.w;
		rect1.x = (*it1)->GetPositionX()+mapCenterX;//+mapCenterX;
		rect1.y = (*it1)->GetPositionY()+mapCenterY;//j*cellWidth+mapCenterY;

		if(SDL_HasIntersection(&rect0, &rect1))
		{
			monsters.erase(it1);
			flagRemove = true;

			break;
		}
		else
			it1++;

		if(flagRemove)
			break;
	}*/

	for(auto it1=enemies.begin(); it1!=enemies.end();)
	{
		SDL_Rect rect1;
		rect1.w = cellWidth/2;
		rect1.h = rect1.w;
		rect1.x = (*it1).GetPositionX()+mapCenterX;//+mapCenterX;
		rect1.y = (*it1).GetPositionY()+mapCenterY;//j*cellWidth+mapCenterY;

		if(SDL_HasIntersection(&rect0, &rect1))
		{
			enemies.erase(it1);
			flagRemove = true;

			break;
		}
		else
			it1++;

		if(flagRemove)
			break;
	}


	return false;
}

bool Map::BulletMapCollision(int level)
{
	int w = widthInCells;
	int h = heightInCells;

	for(auto it=pBullets.begin();
			it!=pBullets.end();)
	{
		SDL_Rect rect;
		rect.w = it->w;
		rect.h = rect.w;
		rect.x = it->x+mapCenterX;
		rect.y = it->y+mapCenterY;

		bool flagRemove = false;
		for(int i=0; i < w; i++)
		{
			for(int j=0; j<h; j++)
			{
				int it3 = currentMap[(0*w*h)+(i*h+j)];

				SDL_Rect rect1;
				rect1.w = cellWidth;
				rect1.h = rect1.w;
				rect1.x = i*cellWidth+mapCenterX;
				rect1.y = j*cellWidth+mapCenterY;

				if(it3 == WALL)
				{
					if(SDL_HasIntersection(&rect, &rect1))
					{
						it3 = FLOOR;
						it = pBullets.erase(it);
						flagRemove = true;

						break;
					}
				}
				it3++;
			}
			if(flagRemove)
				break;
		}
		if(!flagRemove)
			it++;
	}
}

bool Map::SimpleEnemyCollision(float px, float py, int pw, int ph)
{
	for(auto enemy0: enemies)
	{
		if(enemy0.GetType() == simple)
		{
			SDL_Rect rec0;
			rec0.x = enemy0.GetPositionX();
			rec0.y = enemy0.GetPositionY();
			rec0.w = cellWidth;
			rec0.h = cellWidth;	

			SDL_Rect rec1;
			rec1.x = px;
			rec1.y = py;
			rec1.w = pw;//cellWidth/2;
			rec1.h = ph;//cellWidth/2;

			if(SDL_HasIntersection(&rec0, &rec1))
			{
				return true;
			}
		}
	}
	return false;
}

bool Map::MonsterCollision(float px, float py, int pw, int ph)
{

	for(auto enem0: enemies)
	{
		if(enem0.GetMind() == common)
		{
			SDL_Rect rec0;
			rec0.x = enem0.GetPositionX();
			rec0.y = enem0.GetPositionY();
			rec0.w = cellWidth/2;
			rec0.h = cellWidth/2;	

			SDL_Rect rec1;
			rec1.x = px;
			rec1.y = py;
			rec1.w = pw;//cellWidth/2;
			rec1.h = ph;//cellWidth/2;

			if(SDL_HasIntersection(&rec0, &rec1))
			{
				enem0.AttackF(px, py, mapCenterX, mapCenterY);
			}
		}
	}


}

bool Map::WallCollision(float px, float py, int pw, int ph)//+
{
	int w = widthInCells;
	int h = heightInCells;

	for(int i=0; i<w; i++)
	{
		for(int j=0; j<h; j++)
		{
			int cell = currentMap[(0*w*h)+(i*h+j)];

			if(cell == WALL)
			{
				double tmpx = i*cellWidth;//+mapCenterX;
				double tmpy = j*cellWidth;//+mapCenterY;

				SDL_FRect rec;
			        rec.x = tmpx;
				rec.y = tmpy;
				rec.w = cellWidth;
				rec.h = cellWidth;	

				SDL_FRect rec1;
			        rec1.x = px;
				rec1.y = py;
				rec1.w = pw;//cellWidth/2;
				rec1.h = ph;//cellWidth/2;

				if(IntersectFRect(&rec, &rec1))
				{
					return true;
				}
			}
		}
	}
	return false;
}


void Map::Shoot(Player* player, float press_duration)
{
	Bullet bullet_block;

	bullet_block.x = player->rect.x+cellWidth/8;
	bullet_block.y = player->rect.y+cellWidth/8;
	bullet_block.startPoint.x = bullet_block.x;
	bullet_block.startPoint.y = bullet_block.y;

	bullet_block.w = cellWidth/4;
	bullet_block.h = cellWidth/4;
	bullet_block.direction = player->direction;

	bullet_block.distance =  (press_duration/8)*cellWidth/20;
	
	bullet_block.z = 0;

	switch(player->direction)
	{
		case 0:
			bullet_block.x -= cellWidth/8;
			break;
		case 1:
			bullet_block.x += cellWidth/8;
			break;
		case 2:
			bullet_block.y -= cellWidth/8;
			break;
		case 3:
			bullet_block.y += cellWidth/8;
			break;
	}

	pBullets.push_back(bullet_block);
}

bool Map::IsFree(int x, int y, int* map)//+
{
	int h = heightInCells;
        int w = widthInCells;

	if(!(x>=0&&y>=0&&x<=(w-1)&&y<=(h-1)))
	{
        	return false;
    	}

	if(map[(0*w*h)+(x*h+y)] != FLOOR)
    	{
        	return false;
    	}

	
	return true;
}

std::vector<Block> Map::FreeNeighbours(int x, int y, int* map)// level)//+-
{
	int w = cellWidth/2;
	int h = cellWidth/2;
	int z = 0;
    
	std::vector<Block> zone;

       if(this->IsFree(x-1,y,map))
           zone.push_back(CreateBlock(x-1,y, w, h, z));
       if(this->IsFree(x+1,y,map))
           zone.push_back(CreateBlock(x+1,y, w, h, z));
       if(this->IsFree(x,y+1,map))
           zone.push_back(CreateBlock(x,y+1, w, h, z));
       if(this->IsFree(x,y-1,map))
           zone.push_back(CreateBlock(x,y-1, w, h, z));

	return zone;
}

int Map::Distance(Block start, Block dest)//+
{
	int x_1 = start.x;
	int x_2 = dest.x;
	int y_1 = start.y;
	int y_2 = dest.y;

	int dx = x_2 - x_1 - floor(y_2/2) + floor(y_1/2);
	int dy = y_2 - y_1;

	if(((dx > 0) - (dx < 0)) == ((dy > 0) - (dy < 0)))
		return std::abs(dx + dy);
	else
		return std::max(abs(dx), abs(dy));

}

int Map::Distance(SDL_Rect start, SDL_Rect dest)//+
{
	int x_1 = start.x;
	int x_2 = dest.x;
	int y_1 = start.y;
	int y_2 = dest.y;

	int dx = x_2 - x_1 - floor(y_2/2) + floor(y_1/2);
	int dy = y_2 - y_1;

	if(((dx > 0) - (dx < 0)) == ((dy > 0) - (dy < 0)))
		return std::abs(dx + dy);
	else
		return std::max(abs(dx), abs(dy));
}


Block Map::GetNearestMapTile(int x, int y)//+
{
	int mapX = x/cellWidth;
	int mapY = y/cellWidth;

	Block bl = CreateBlock(mapX, mapY, cellWidth, cellWidth, 0);

	return bl;
}

void Map::UpdateEnemies(Player* player, float dt)
{
	for(int i = 0; i < enemies.size(); i++)
	{
		Enemy0* enem0 = &enemies[i];
		if(enem0->GetMind() == common)
		{
			Block bl0 = GetNearestMapTile(enem0->GetPositionX(), enem0->GetPositionY());
			Block bl1 = GetNearestMapTile(player->rect.x, player->rect.y);

			std::vector<Block> path = FindPath(bl0.x, bl0.y, bl1.x, bl1.y, currentMap);

			if(!path.empty())
			{
				if(path.size()>1)
				{
					Block bl = path.end()[-2];

					MoveToNextCell(bl.x*cellWidth+cellWidth/2+mapCenterX,
					   bl.y*cellWidth+cellWidth/2+mapCenterY, enem0);
				}
			
				if((path.size()==1))// or (path.size()==2))
				{
					//Block bl = path.begin()[1];

					MoveToLastCell(player->rect.x+cellWidth/4+mapCenterX,
					   player->rect.y+cellWidth/4+mapCenterY, enem0);
				}

			}
			else
			{
			//std::cout << "EMPTY !!!!!! - " << std::endl;
			}
		}

		if(enem0->GetMind() == silly)
		{

			int x = 0;
			int y = 0;

			switch(enem0->GetDirection())
			{
				case 0:
					x = (enem0->GetPositionX()-(dt*100*Speed))/cellWidth;
					y = (enem0->GetPositionY())/cellWidth;

					if(!CheckTileWall(x, y, currentMap))
					{
	 					enem0->SetPositionX(enem0->GetPositionX()-(dt*100*Speed));
					}
					else
						enem0->SetDirection(1);
					break;
				case 1:

					x = (enem0->GetPositionX()+(dt*100*Speed))/cellWidth;
					y = (enem0->GetPositionY())/cellWidth;
	
					if(!CheckTileWall(x+1, y, currentMap))
					{
						enem0->SetPositionX(enem0->GetPositionX()+(dt*100*Speed));
					}
					else
						enem0->SetDirection(0);
					break;
				case 2:
					x = (enem0->GetPositionX())/cellWidth;
					y = (enem0->GetPositionY()-(dt*100*Speed))/cellWidth;

					if(!CheckTileWall(x, y, currentMap))
					{
	 					enem0->SetPositionY(enem0->GetPositionY()-(dt*100*Speed));
					}
					else
						enem0->SetDirection(3);
					break;
				case 3:
					x = (enem0->GetPositionX())/cellWidth;
					y = (enem0->GetPositionY()+(dt*100*Speed))/cellWidth;

					if(!CheckTileWall(x, y+1, currentMap))
					{
	 					enem0->SetPositionY(enem0->GetPositionY()+(dt*100*Speed));
					}
					else
						enem0->SetDirection(2);
					break;
			}
		}	
	}
}

//A* algorithm
std::vector<Block> Map::FindPath(int start_x, int start_y, int end_x, int end_y, int* map)
{
    	int _mapWidth = widthInCells;
    	int _mapHeight = heightInCells;

    	std::map<Block, int> g;//g  from start to current

    	std::map<Block, int> f;//h from point to end

    	std::map<Block, Block> came_from;

    	std::vector<Block> closed;

    	std::vector<Block> open;

    	open.push_back(CreateBlock(start_x, start_y, cellWidth/2, cellWidth/2, 0));

    	std::vector<Block> result_path;

    	std::vector<Block> Neighbours;

    	Block current;

    	std::vector<Block> current_path;

    	Block node = open.front();

    	g.insert(std::make_pair(node, 0));

    	Block end = CreateBlock(end_x, end_y, cellWidth/2, cellWidth/2, 0);
    	Block start = CreateBlock(start_x, start_y, cellWidth/2, cellWidth/2, 0);

	//std::cout << "Finding Path !!! " << std::endl;

	while(!open.empty())
	{
		//std::cout << "notempty " << std::endl;
		node = open.front();
       		//find lowest score in open
        	for(auto point : open)
        	{
			//std::cout << "open block - " << point.x << ":" << point.y << std::endl;

           		int score_1 = f.find(point)->second;
           		int score_2 = f.find(node)->second;

           		if(score_1 < score_2)
           		{
                		node = point;
           		}
        	}

        	//if current == end
        	if(node == end)
        	{
			result_path.push_back(node);

            		bool flag = true;

            		Block point3 = node;

			while(flag)
			{
						if(came_from.contains(point3))
						{
								
							if(came_from.find(point3) != came_from.end())
							{
								//std::cout << came_from.find(point3)->first.x << ":" <<
								//	came_from.find(point3)->first.y <<
								//	std::endl;

								point3=came_from.find(point3)->second;

								//std::cout << "pointsecond " << point3.x << ":" << point3.y << std::endl;

								result_path.push_back(point3);

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
            return result_path;
        }

        open.erase(std::find(open.begin(), open.end(), node));

        closed.push_back(node);

        Neighbours = FreeNeighbours(node.x, node.y, map);
        for(auto point2 : Neighbours)
        {
		//std::cout << "neighbours loop " << std::endl;
            if(std::find(closed.begin(), closed.end(), point2) != closed.end())
            	continue;

            bool neighborInOpenSet = (std::find(open.begin(), open.end(), point2) != open.end());

            auto tentative_g_score= g.find(node)->second+this->Distance(node, point2);

            if((!neighborInOpenSet) || (tentative_g_score < g.find(node)->second))
            {

                came_from.insert(std::make_pair(point2, node));
				if(came_from.size()>30)
				{
			result_path.clear();
					return result_path;
				}

		g.insert(std::make_pair(point2, tentative_g_score));

                f.insert(std::make_pair(point2,
										tentative_g_score+this->Distance(point2,
																		 end)));
                if(!neighborInOpenSet)
                    open.push_back(point2);
            }
        }
    }

    return result_path;
}

bool Map::CheckMonsterCollision(double x, double y, double w, double h, Enemy0* mon) {

	SDL_FRect rect0;
	rect0.w = w+(double)cellWidth/10;
	rect0.h = rect0.w;
	rect0.x = x+mapCenterX-(double)cellWidth/10;
	rect0.y = y+mapCenterY-(double)cellWidth/10;
	
	int wc = widthInCells;
	int hc = heightInCells;

	for(int i=0; i < wc; i++)
	{
		for(int j =0; j < hc; j++)
		{
			int cell = currentMap[(0*wc*hc)+(i*hc+j)];

			if(cell != WALL)
				continue;

			SDL_FRect rect1;
			rect1.w = cellWidth;
			rect1.h = rect1.w;
			rect1.x = i*cellWidth+mapCenterX;
			rect1.y = j*cellWidth+mapCenterY;

			if(IntersectFRect(&rect0, &rect1))
			{
				return true;
			}
		}
	}

	for(int i = 0; i < enemies.size(); i++ )
	{
		Enemy0* enem0 = &enemies[i];
		if(enem0->GetMind() == common)
		{
			double mx = enem0->GetPositionX()-(double)cellWidth/10;
			double my = enem0->GetPositionY()-(double)cellWidth/10;

			if(mon != enem0)
			{
				SDL_FRect rect1;
				rect1.w = (double)cellWidth/2+(double)cellWidth/10;
				rect1.h = rect1.w;
				rect1.x = mx+mapCenterX;
				rect1.y = my+mapCenterY;
				if(IntersectFRect(&rect0, &rect1))
				{
					return true;
				}
			}
		}
	}

	/*SDL_FRect rect2;
	rect2.w = player->rect.w;
	rect2.h = rect2.w;
	rect2.x = player->rect.x+mapCenterX;
	rect2.y = player->rect.y+mapCenterY;
	if(IntersectFRect(&rect0, &rect2))
	{
		return true;
	}*/


	return false;
}

void Map::MoveToNextCell(double pX, double pY, Enemy0* monster)
{
	double x = monster->GetPositionX();
	double y = monster->GetPositionY();
	double w = cellWidth/2;
	double h = cellWidth/2;

	double centerX = (x+w/2)+mapCenterX;
	double centerY = (y+h/2)+mapCenterY;
	int x1 = centerX-pX;
	int y1 = centerY-pY;

	double m = 0;
	double n = 0;

	if(x1>0)
	{
		if(!CheckMonsterCollision(x-mSpeed,y,w,h,monster))
			n=-mSpeed;
	}
	if(x1<0)
	{
		if(!CheckMonsterCollision(x+mSpeed,y,w,h,monster))
			n=+mSpeed;
	}

	if(y1>0)
	{
		if(!CheckMonsterCollision(x,y-mSpeed,w,h,monster))
			m=-mSpeed;
	}
	if(y1<0)
	{
		if(!CheckMonsterCollision(x,y+mSpeed,w,h,monster))
			m=+mSpeed;
	}

	monster->Move(n, m);
}

void Map::MoveToLastCell(double pX, double pY, Enemy0* monster) {

	double x = monster->GetPositionX();
	double y = monster->GetPositionY();
	double w = cellWidth/2;
	double h = cellWidth/2;

	double centerX = (x+w/2)+mapCenterX;
	double centerY = (y+h/2)+mapCenterY;
	int x1 = centerX-pX;
	int y1 = centerY-pY;

	double lenX = sqrt((centerX-pX)*(centerX-pX));
	double lenY = sqrt((centerY-pY)*(centerY-pY));

	double m = 0;
	double n = 0;

	if((lenX > mSpeed) && (lenY > mSpeed))
	{

		if(x1>0)
		{
			if(!CheckMonsterCollision(x-mSpeed,y,w,h,monster))
				n=-mSpeed;
		}
		if(x1<0)
		{
			if(!CheckMonsterCollision(x+mSpeed,y,w,h,monster))
				n=+mSpeed;
		}

		if(y1>0)
		{
			if(!CheckMonsterCollision(x,y-mSpeed,w,h,monster))
				m=-mSpeed;
		}
		if(y1<0)
		{
			if(!CheckMonsterCollision(x,y+mSpeed,w,h,monster))
				m=+mSpeed;
		}

		monster->Move(n, m);
	}

}

/*bool Map::PlayerDie(Player* player)
{
	if(player->health <= 0)
	{
		std::cout << "YOU LOST" << std::endl;
		return true;
	}

	return false;
}*/

void Map::MonsterHit(Player* player, float dt, int power)
{
	if(player->hitAnimTimer>0)
		player->hitAnimTimer -= dt;

	for(int i = 0; i < enemies.size(); i++)
	{
		Enemy0* enem0 = &enemies[i];
		if(enem0->GetType() != simple)
		{
			int monx = enem0->GetPositionX();
			int mony = enem0->GetPositionY();

			float len = sqrt((monx-player->rect.x)*(monx-player->rect.x)+(mony-player->rect.y)*(mony-player->rect.y));

			if(len<(cellWidth*1.5))
			{
				enem0->IncreaseHitTimer(dt);

				if(enem0->GetHitTimer() > 2)
				{
					player->health -= power;
					enem0->ZeroHitTimer();
					player->hitAnimTimer = 0.4;
					//std::cout << "PLAYER HEALTH:" << player.health << std::endl;
				}
			}
		}
	}

	if(player->hitAnimTimer > 0)
	{
		player->sprite = 1;
	}
	else
		player->sprite = 0;
}

void Map::UpdateMapCenterX(double x)//, Player* player)
{
	mapCenterX = width/2 - x;
}

void Map::UpdateMapCenterY(double y)//, Player* player)
{
	mapCenterY = height/2 - y;
}

int Map::GetMapCenterX()//, Player* player)
{
	return mapCenterX;
}

int Map::GetMapCenterY()//, Player* player)
{
	return mapCenterY;
}



/*
void Map::PlayerStopX()
{
	moveSpeed.x = 0;
}

void Map::PlayerStopY()
{
	moveSpeed.y = 0;
}

void Map::PlayerMoveDown()
{
	player.direction = 3;
	moveSpeed.y = +player.speed;
}

void Map::PlayerMoveUp()
{
	player.direction = 2;
	moveSpeed.y = -player.speed;
}

void Map::PlayerMoveLeft()
{
	player.direction = 0;
	moveSpeed.x = -player.speed;
}

void Map::PlayerMoveRight()
{
	player.direction = 1;
	moveSpeed.x = +player.speed;
}
*/
bool Map::IntersectRect(const SDL_Rect * r1, const SDL_Rect * r2)
{
  return !(r2->x > (r1->x + r1->w) || 
           (r2->x + r2->w) < r1->x || 
           r2->y > (r1->y + r1->h) ||
           (r2->y + r2->h) < r1->y);
}

bool Map::IntersectFRect(const SDL_FRect * r1, const SDL_FRect * r2)
{
  return !(r2->x > (r1->x + r1->w) || 
           (r2->x + r2->w) < r1->x || 
           r2->y > (r1->y + r1->h) ||
           (r2->y + r2->h) < r1->y);
}


void Map::Update(GameEngine* game, Player* player, float dt)
{
	/*if(!WallCollision(player->rect.x+moveSpeed.x*(dt*100), player->rect.y, player->rect.w, player->rect.h))
	{
		player->rect.x += moveSpeed.x*(dt*100);
		mapCenterX = width/2 - player->rect.x;
	}

	if(!WallCollision(player->rect.x, player->rect.y+moveSpeed.y*(dt*100), player->rect.w, player->rect.h))
	{
		player->rect.y += moveSpeed.y*(dt*100);
		mapCenterY = height/2 - player->rect.y;
	}*/	

	for(int i=0; i<pBullets.size();
		i++)
	{
		Bullet* bullet0 = &pBullets.at(i);
		double tempSpeed = cellWidth/10;

		switch(bullet0->direction)
		{
			case 0:
				bullet0->x -= tempSpeed;
				break;
			case 1:
				bullet0->x += tempSpeed;
				break;
			case 2:
				bullet0->y -= tempSpeed;
				break;
			case 3:
				bullet0->y += tempSpeed;
				break;
		}

		if((bullet0->x >
			bullet0->startPoint.x+bullet0->distance)or(bullet0->x < bullet0->startPoint.x-bullet0->distance)or(bullet0->y < bullet0->startPoint.y-bullet0->distance)or(bullet0->y > bullet0->startPoint.y+bullet0->distance))
		{
			this->CheckBulletEnemiesCollision(bullet0);
			this->CheckBulletMonstersCollision(bullet0);

			pBullets.erase(pBullets.begin()+i);
			break;
		}
	}

	SDL_FRect rect0;
	rect0.w = cellWidth;
	rect0.h = rect0.w;
	rect0.x = exit.x;
	rect0.y = exit.y;

	if(RectInside(&player->rect, &rect0))
	{
		free(nextLevelMap);
		level+=1;
		widthInCells+=1;
		heightInCells+=1;
		nextLevelMap = (int*)malloc((1)*(widthInCells * heightInCells) * sizeof(int)); 
		nextRooms.clear();
		nextFreeMap.clear();
		enemies.clear();
		GenerateNewLevel(game, 0, &nextFreeMap, nextLevelMap, &nextRooms);

		int total = nextFreeMap.size();
		int r0 = 0;
		r0 = rand()%total;

		SDL_Rect rect0 = nextFreeMap.at(r0);

		ClearLines(rect0.x, rect0.y);

		player->rect.x = rect0.x*cellWidth+cellWidth/4;
		player->rect.y = rect0.y*cellWidth+cellWidth/4;
		mapCenterX = width/2 - player->rect.x;
	       	mapCenterY = height/2 - player->rect.y;
		
		nextFreeMap.erase(nextFreeMap.begin() + r0);

		currentMap = nextLevelMap;
		currentFreeMap = nextFreeMap;
		currentRooms = nextRooms;
		if(Speed<cellWidth/6)
			Speed += width/1000;
		game->Save(level, widthInCells, heightInCells);
	}
	
	UpdateEnemies(player, dt);

	this->MonsterHit(player, dt, 1);
	if(this->SimpleEnemyCollision(player->rect.x, player->rect.y, player->rect.w, player->rect.h))
		player->health = 0;
	//this->CheckBulletEnemiesCollision(level);
	this->BulletMapCollision(level);
}

bool Map::CheckOverlaps(SDL_Rect room1, SDL_Rect room2)
{

	if(((room1.y < room2.y+room2.h)and(room1.y > room2.y)) or ((room1.y+room1.h >
													   room2.y)and(room1.y+room1.h
																   < room2.y+room2.h)))
	{
		//return true;
		if((room1.x < room2.x+room2.w)and(room1.x > room2.x))
		{
			return true;
		}

		if((room1.x+room1.w < room2.x+room2.w)and(room1.x+room1.w > room2.x))
		{
			return true;
		}

		if((room2.x < room1.x+room1.w)and(room2.x > room1.x))
		{
			return true;
		}

		if((room2.x+room2.w < room1.x+room1.w)and(room2.x+room2.w > room1.x))
		{
			return true;
		}
	}

	if(((room2.y < room1.y+room1.h)and(room2.y > room1.y)) or ((room2.y+room2.h >
													   room1.y)and(room2.y+room2.h
																   <
																   room1.y+room1.h)))
	{
		if((room1.x < room2.x+room2.w)and(room1.x > room2.x))
		{
			return true;
		}

		if((room1.x+room1.w < room2.x+room2.w)and(room1.x+room1.w > room2.x))
		{
			return true;
		}

		if((room2.x < room1.x+room1.w)and(room2.x > room1.x))
		{
			return true;
		}

		if((room2.x+room2.w < room1.x+room1.w)and(room2.x+room2.w > room1.x))
		{
			return true;
		}
	}

	return false;
}

void Map::Carve(int x, int y, std::vector<SDL_Rect>* freeMap,  int* map)//+ 
{
	int h = heightInCells;
        int w = widthInCells;

	if((x<w) && (x>0))
	{
		if((y<h) && (y>0))
		{
		 	map[(0*w*h)+(x*h+y)] = FLOOR;
			mazeSize++;

			Block bk;
			bk.x = x;
			bk.y = y;
			//bk.region = _currentRegion;
			//_regions.push_back(bk);

			SDL_Rect rk;
			rk.x = x;
			rk.y = y;
			rk.w = cellWidth;
			rk.h = rk.w;

		//	_regions[x][y] = _currentRegion;
			freeMap->push_back(rk);
		}
	}
}


/*int Map::GetRegion(int x, int y) {//+

	for(auto reg : _regions)
	{
		if((reg.x==x)and(reg.y==y))
		{
			return reg.region;
		}
	}

	return -999;
}*/


bool Map::CheckTileWall(int x, int y, int* map) {

	int h = heightInCells;
	int w = widthInCells;

	if(map[(0*w*h)+(x*h+y)] == WALL)
		return true;
	if((x>=w)||(x<0)||(y>=h)||(y<0))
		return true;

	return false;
}

void Map::AddJunction(int x, int y, std::vector<SDL_Rect>* freeMap, int* map)//+
{
    Carve(x, y, freeMap, map);
}

/*void Map::StartRegion()//?????????
{
    //_currentRegion++;
}*/
/*	
void Map::_connectRegions(std::vector<SDL_Rect>* freeMap, int* map)//????????
{
	std::vector<Connector> connectors;

	for(int i=1; i<widthInCells-1; i++)
	{
		for(int j=1; j<widthInCells-1; j++)
		{

			if (!CheckTileWall(i, j, map))
				continue;

			std::vector<int> regions;

			for (int dir = north; dir <= east; ++dir)
			{
				SDL_Point ucell{i, j};

				switch(dir)
				{
					case north:
						ucell.y-=1;	
						break;
					case south:
						ucell.y+=1;
						break;
					case west:
						ucell.x-=1;
						break;
					case east:
						ucell.x+=1;
						break;
				}

				if (CheckTileWall(ucell.x, ucell.y, map)) continue;

				int region = GetRegion(ucell.x, ucell.y);//_regions[ucell.x][ucell.y];

				//if(region == -999)
				//{
				//	std::cout << "ucell.x:" << ucell.x << "ucell.y:" << ucell.y
				//		<< std::endl;
				//	if (CheckTileWall(ucell.x, ucell.y))
				//		std::cout << "WALL!!!" << std::endl;
				//}

				if(std::count(regions.begin(), regions.end(), region))
				{
				}
				else
				{
					if(region != -999)
						regions.push_back(region);
				}
				//}
			}

		   	// worldMap[j][i].tp = NOM_NOM;

			if (regions.size() < 2)
				continue;


			std::cout << "Regions Size:" << regions.size() << std::endl;
			Connector con;
			con.x = i;
			con.y = j;
			con.regions = regions;
	
			connectors.push_back(con);
		}
	}

	//
	//connectors = connectorRegions.key.toList();

	std::map<int, int> merged;
	std::vector<int> openRegions;
    	for (int i = 0; i <= _currentRegion; i++) {
      		merged[i] = i;
      		openRegions.push_back(i);
    	}

//	while ((openRegions.size() > 1)and(connectors.size()>0)) {

	while((openRegions.size()>1)and(connectors.size()>0))
	{
		//std::cout << "WHILE" << std::endl;
		int c;
		if(connectors.size() == 0)
		{
			c = NULL;
		}
		else
			c = rand()%connectors.size();

		Connector connector = connectors[c];

		AddJunction(connector.x, connector.y, freeMap, map);


	   	std::vector<int> regions;
	   	for(auto it : connector.regions)
	   	{
			regions.push_back(merged[it]);
	   	}


		int dest = regions.front();

		std::cout << "regions: " << regions.size() << std::endl;
		std::cout << "dest: " << dest << std::endl;


		std::vector<int> sources(regions.begin() + 1, regions.end());

		std::cout << "sources: " << sources.size() << std::endl;

		for(auto source : sources)
		{
			std::cout << "!~source: " << source << std::endl;
		}

	    	for (int i = 0; i <= _currentRegion; i++) {
			for(auto source : sources)
			{
				if (source == merged[i]) {
					merged[i] = dest;
				}
			}
		}

		for(auto it = sources.begin(); it<sources.end();)
		{
			for(auto it2 = openRegions.begin(); it2<openRegions.end();)
			{
				std::cout << "-openRegion: " << *it2 << std::endl;

				if(*it == *it2)
				{
					it2 = openRegions.erase(it2);

					std::cout << "openRegions: " << openRegions.size() << std::endl;
				}
				else
					it2++;
			}
			it++;
		}


		for(auto it2 = connectors.begin(); it2<connectors.end();)
		{
				bool removeFlag = false;

				if((it2->x<connector.x+2)and(it2->y<connector.y+2)and(it2->x>connector.x-2)and(it2->y>connector.y-2))
				{
					removeFlag = true;
				}

				std::vector<int> regs;

				for(auto itz : it2->regions)
				{
					if(!std::count(regs.begin(), regs.end(),
							   merged[itz]))
						regs.push_back(merged[itz]);
				}


				if(regs.size() < 2)
				{
					//if((connector.x!=it2->x)or(connector.y!=it2->y))
					//	worldMap[it2->x][it2->y].tp = CONNECTOR;
					removeFlag = true;
				}

				if(removeFlag)
				{
					it2 = connectors.erase(it2);
					//worldMap[it2->x][it2->y].tp = CONNECTOR;
					//std::cout << "REMOVED" << std::endl;
				}
				else
					it2++;
		}

	}
}*/

bool Map::CanCarve(int x, int y, int direction, int* map)//+
{
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

	if ((xtmp3>=widthInCells-1)or(ytmp3>=widthInCells-1)or(xtmp3<=0)or(ytmp3<=0)) return false;

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

	if(CheckTileWall(xtmp2, ytmp2, map))
	{
				flag6 = true;
	}

	if(CheckTileWall(xtmp3, ytmp3, map))
	{
				flag7 = true;
	}

	if((flag6)and(flag7))
		return true;

	return false;
}

void Map::GrowMaze(int x, int y, std::vector<SDL_Rect>* freeMap, int* map)//+
{
	std::vector<SDL_Rect> cells;
    
	SDL_Rect lastDir;
	SDL_Rect lastLastDir;
	SDL_Rect tmp{-1,-1,0,0};
	lastDir=tmp;
	lastLastDir=tmp;
	int lDir = -1;

	int windingPercent = 30;
	mazeSize = 0;
    
	//StartRegion();
    	Carve(x, y, freeMap, map);

	SDL_Rect start{x, y, 0, 0};
	cells.push_back(start);

    	while (!cells.empty()) 
	{
    		SDL_Rect cell = cells.back();

	  	std::vector<SDL_Rect> unmadeCells;

	  	for (int dir1 = north; dir1 <= east; dir1++)
	  	{
			if (CanCarve(cell.x, cell.y, dir1, map))
			{
				SDL_Rect ucell{cell.x, cell.y, 0, 0};

				switch(dir1)
				{
					case north:
						ucell.y-=1;	
						//std:hg:cout << "NORTH!!" << std::endl;
						unmadeCells.push_back(ucell);
						break;
					case south:
						ucell.y+=1;
						//std::cout << "SOUTH!!" << std::endl;
						unmadeCells.push_back(ucell);
						break;
					case west:
						ucell.x-=1;
						//std::cout << "WEST!!" << std::endl;
						unmadeCells.push_back(ucell);
						break;
					case east:
						ucell.x+=1;
						//std::cout << "EAST!!" << std::endl;
						unmadeCells.push_back(ucell);
						break;
				}
			}
	 	 }

      		if (!unmadeCells.empty()) {
        	
			// Based on how "windy" passages are, try to prefer carving in the
        		// same direction.
        		SDL_Rect dir;
			bool isflag = false;
			int tx;
			int ty;

			for(auto it : unmadeCells)
			{
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

				//int tx = cell.x + lDir.x;
				//int ty = cell.y + lDir.y;
				if ((it.x == tx)and(it.y == ty))
				{
					isflag = true;
					break;
				}

			}

			if((isflag) and (rand()%100 > windingPercent)) {
				dir.x = tx;
				dir.y = ty;
			}
			else
			{
					int i = rand()%(unmadeCells.size());

					dir = unmadeCells[i];
			}

			int k = dir.x - cell.x;
			int l = dir.y - cell.y;

			if(k > 0)
				lDir = 3;

			if(k<0)
				lDir = 2;

			if(l > 0)
				lDir = 1;

			if(l < 0)
				lDir = 0;

        		Carve(dir.x, dir.y, freeMap, map);

			SDL_Rect dir2;
			dir2.x = dir.x + k;
			dir2.y = dir.y + l;

        		Carve(dir2.x, dir2.y, freeMap, map);

        		cells.push_back(dir2);
        		lastDir = dir;
      		} else {
        		// No adjacent unCarved cells.
        		cells.pop_back();

        		// This path has ended.
        		lastDir = tmp;
			lDir = -1;
      		}		
	}
}

/*void Map::DrawPlayer(std::shared_ptr<RenderManager>& rend, Player* player,  double x, double y)
{
	if(player->sprite == 0)
    		rend->DrawTexture(blue_texture.get(), x, y);
	else if(player->sprite == 1)
    		rend->DrawTexture(orange_texture.get(), x, y);
}*/

void Map::DrawObstacles(std::shared_ptr<RenderManager>& rend, double x, double y,
							double w, double h)
{
    rend->DrawTexture(green_texture.get(), x, y, w, h);
}

void Map::DrawSimpleEnemy(std::shared_ptr<RenderManager>& rend, double x, double y,
							double w, double h)
{
    rend->DrawTexture(green_texture.get(), x, y, w, h);
}

void Map::DrawGoblin(std::shared_ptr<RenderManager>& rend, double x, double y,
							double w, double h)
{
	 rend->DrawTexture(goblin_texture.get(), x, y, w, h);
}


void Map::DrawExit(std::shared_ptr<RenderManager>& rend, double x, double y,
							double w, double h)
{
    rend->DrawTexture(door_texture.get(), x, y, w, h);
}

void Map::Draw(std::shared_ptr<RenderManager>& rend, Player* player)//+
{
	//this->DrawPlayer(rend, mapCenterX+player->rect.x, mapCenterY+player->rect.y);

	SDL_Rect rect0;
	rect0.x = player->rect.x/cellWidth;
	rect0.y = player->rect.y/cellWidth;
	rect0.w = cellWidth/2;
	rect0.h = cellWidth/2;

	int w = widthInCells;
	int h = heightInCells;

	for (int i = 0; i < w; i++) 
	{
		for (int j = 0; j < h; j++)
		{
			SDL_Rect rect1;
			rect1.x = i;//*cellWidth;
			rect1.y = j;//*cellWidth;
			rect1.w = cellWidth;
			rect1.h = cellWidth;

			if(Distance(rect1, rect0)<8)
			{

				if(currentMap[(0*w*h)+(i*h+j)] == WALL)
				{
					rend->DrawTexture(orange_texture.get(), mapCenterX+rect1.x*cellWidth, mapCenterY+rect1.y*cellWidth, cellWidth, cellWidth);
				}
			}
		}
	}

	for(int i = 0; i < enemies.size(); i++)
    	{
		SDL_Rect rect2;
		rect2.x = enemies[i].GetPositionX()/cellWidth;
		rect2.y = enemies[i].GetPositionY()/cellWidth;
		if(enemies[i].GetType() == simple)
		{
			rect2.w = cellWidth;
			rect2.h = cellWidth;
		}
		if(enemies[i].GetType() == goblin)
		{
			rect2.w = cellWidth/2;
			rect2.h = cellWidth/2;
		}


		if(Distance(rect2, rect0)<8)
		{
			if(enemies[i].GetType() == simple)
				this->DrawSimpleEnemy(rend, mapCenterX+enemies[i].GetPositionX(), mapCenterY+enemies[i].GetPositionY(), cellWidth, cellWidth);
			else if (enemies[i].GetType() == goblin)
				this->DrawGoblin(rend, mapCenterX+enemies[i].GetPositionX(), mapCenterY+enemies[i].GetPositionY(), cellWidth/2, cellWidth/2);
		}
	}

	SDL_Rect rect2;
	rect2.x = exit.x/cellWidth;
	rect2.y = exit.y/cellWidth;
	rect2.w = cellWidth;
	rect2.h = cellWidth;

	if(Distance(rect2, rect0)<8)
	{

		this->DrawExit(rend, mapCenterX+exit.x, mapCenterY+exit.y, cellWidth, cellWidth);
	}

	if(!pBullets.empty())
	{
		for(int i=0;i<pBullets.size();i++)
		{
			this->DrawObstacles(rend, pBullets[i].x+mapCenterX, pBullets[i].y+mapCenterY,
							pBullets[i].w, pBullets[i].h);
		}
	}


	//For DEBUG
	/*for(auto cell : currentFreeMap)
	{
		SDL_Rect rect2;
		rect2.x = cell.x;
		rect2.y = cell.y;
		rect2.w = cellWidth;
		rect2.h = cellWidth;

		if(Distance(rect2, rect0)<8)
		{
			this->DrawExit(rend, mapCenterX+cell.x*cellWidth+cellWidth/4, mapCenterY+cell.y*cellWidth+cellWidth/4, cellWidth/2, cellWidth/2);
		}
	}*/

	/*for(auto room : currentRooms)
	{
		
		SDL_Rect rect2;
		rect2.x = room.x;
		rect2.y = room.y;
		rect2.w = cellWidth;
		rect2.h = cellWidth;

		//if(Distance(rect2, rect0)<8)
		//{
		for (int xtmp = room.x; xtmp < (room.x+room.w); xtmp++) 
		{
			for(int ytmp = room.y; ytmp < (room.y+room.h); ytmp++)
			{
				this->DrawObstacles(rend, mapCenterX+xtmp*cellWidth+cellWidth/4, mapCenterY+ytmp*cellWidth+cellWidth/4, cellWidth/2, cellWidth/2);
			}
		}
		//}
	}*/
}

