#include "enemy.h"

bool Enemy0::InitEnemy(Enemy* enemy, double x, double y, double w, double h, double cellWidth) {
	printf("enemy created\n");
	hitTimer = 1.9;
	rect.x = x;
	rect.y = y;
	rect.w = cellWidth/2;
	rect.h = cellWidth/2;
	
	return true;
}

void Enemy0::DestroyEnemy() {
  printf("enemy destroyed\n");
  //free(sngPlayer.map);
}

void Enemy0::Move(double pX, double pY)
{
	//if(fSeePlayer)
	//{
		rect.x += pX;
		rect.y += pY;
	//}
}

void Enemy0::AttackF(int px, int py, int mapCenterX, int mapCenterY)
{
	//rect.x = lastCell.x;
	//rect.y = lastCell.y;
}


void Enemy0::Draw(std::shared_ptr<RenderManager>& rend) {
  printf("enemy draw\n");
}

void Enemy0::UpdateEnemy(float deltatime) {
  printf("enemy update %f\n", deltatime);
}
