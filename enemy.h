#ifndef _ENEMY_H
#define _ENEMY_H

#include <SDL2/SDL.h>
#include "gamestate.h"

enum enemyType { simple, goblin, dwarf, gnome, spider, skeleton, rat, wolf, bear, leshy, ghost, zombie, roach };
enum enemyMind { silly, common, smart, genius, boss };

typedef struct {
  SDL_FRect rect;
  int direction;
  double Speed;

  SDL_Texture* enemyTexture;
  int level;
  int type;
  enum enemyMind mind;
  int health;
} Enemy;

class Enemy0
{
	private:
  		SDL_FRect rect;
  		int direction;
  		double Speed;
  		//std::shared_ptr<SDL_Texture> enemyTexture;

  		enemyType type;
		enemyMind mind;
  		int health;
		double hitTimer;
		//bool fSeePlayer;

	public:

		bool InitEnemy(Enemy* enemy, double x, double y, double w, double h, double cellWind);
		void DestroyEnemy();
		void Move(double pX, double pY);
		void Draw(std::shared_ptr<RenderManager>& rend);
		void UpdateEnemy(float deltatime);
		void AttackF(int, int, int, int);


		double GetPositionX(){return rect.x;};
		double GetPositionY(){return rect.y;};
		double GetWidth(){return rect.w;};
		double GetHeight(){return rect.h;};
		int GetDirection(){return direction;};
		enemyType GetType(){return type;};
		enemyMind GetMind(){return mind;};


		void SetPositionX(double x){rect.x = x;};
		void SetPositionY(double y){rect.y = y;};
		void SetWidth(double wd){rect.w = wd;};
		void SetHeight(double ht){rect.h = ht;};
		void SetHitTimer(double ht){hitTimer = ht;};


		void SetDirection(int dir){direction = dir;};
		void SetType(enemyType tp){type = tp;};
		void SetMind(enemyMind md){mind = md;};

		double GetHitTimer(){return hitTimer;};
		void IncreaseHitTimer(float dt){hitTimer+=dt;};
		void ZeroHitTimer(){hitTimer = 0;};
};

#endif //_ENEMY_H
