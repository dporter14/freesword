#include <iostream>

#include "global.h"

using namespace std;
void taylor_func()
{
	strcpy(g.title.text,"Taylor breathes code");
	g.title.text_color = 0x0000ffff;

}

void spawnEnemy(Flt x, Flt y){
	Enemy *e = &g.enemies[g.nenemies++];
	VecMake(x, y, 0, e->pos);
	VecMake(1.0, 0.2, 0.2, e->color);
	e->pradius = 30;
	e->status = 0;
	VecMake(0,1,0,e->dir);
	VecMake(0,0,0,e->vel);
	VecMake(0,30,0,e->rhand_pos);
	VecMake(0,1,0,e->rhand_dir);
}

void Enemy::attackPlayer(){
	Vec toPlayer;
	VecSub(g.player.pos, pos, toPlayer);
	lookAt(g.player.pos[0], g.player.pos[1]);
	float len = VecLen(toPlayer);
	if(len>0){
		addVelocity(toPlayer[0]/len*0.5,toPlayer[1]/len*0.5);
	} else if (VecLen(vel)>0) {
		addVelocity(vel[0]*-0.5,vel[1]*-0.5);
	}
}

void Animation::play() {
	if (frame<=nframes){
		(*this.*func)(frame++);
	} else {
		done=1;
	}
}
	
void Animation::sword_slash(int frame) {
	Character* actor = (Character*)actors[0];
	static Vec orig_pos;
	static Vec orig_dir;
	if(frame==0){
		VecCopy(actor->rhand_pos, orig_pos);
		VecCopy(actor->rhand_dir, orig_dir);
		actor->rhand_pos[0] = 35;
		actor->rhand_pos[1] = 35;
		
	} 
	if(frame==16){
		VecCopy(orig_pos, actor->rhand_pos);
		VecCopy(orig_dir, actor->rhand_dir);
	} else {
		actor->rhand_pos[0] = (-100/15)*frame+50;
		float angle = (PI/2/15)*frame+(PI/3);
		actor->rhand_dir[0] = cosf(angle);
		actor->rhand_dir[1] = sinf(angle);
		
		//cout << (-100/28)*frame+50 << endl;
	}
}














