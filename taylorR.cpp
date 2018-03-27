// by Taylor Redden
#include <iostream>

#include "global.h"

using namespace std;
void taylor_func()
{
	strcpy(g.title.text,"Taylor breathes code");
	g.title.text_color = 0x0000ffff;

}

void spawnEnemy(Flt x, Flt y){
	Enemy *e = &g.enemies[g.number[N_ENEMIES]++];
	VecMake(x, y, 0, e->pos);
	VecMake(1.0, 0.2, 0.2, e->color);
	e->pradius = 30;
	e->state = 0;
	e->max_speed = 6;
	//VecMake(0,1,0,e->dir);
	e->rot=0;
	VecMake(0,0,0,e->vel);
	VecMake(0,30,0,e->rhand_pos);
	//VecMake(0,1,0,e->rhand_dir);
	e->rhand_rot=0;
	e->hitbox.scale[0] = e->hitbox.scale[1] = e->pradius;

}

void Enemy::attackPlayer()
{
	Vec toPlayer;
	VecSub(g.player.pos, pos, toPlayer);
	lookAt(g.player.pos[0], g.player.pos[1]);
	float len = VecLen(toPlayer);
	if (len>100){
		if (acos(VecDot(toPlayer, vel)/(len*VecLen(vel)))>(PI/4)){
			addVel(vel[0]*-0.5,vel[1]*-0.5);
		}
		addVel(toPlayer[0]/len,toPlayer[1]/len);
	} else if (VecLen(vel)>0) {
		addVel(vel[0]*-0.5,vel[1]*-0.5);
	}
}

void Enemy::kill(){
	state = S_CHAR_DEAD;
}

void Animation::add_actor(Object* actor)
{
	actors[nactors++] = actor;
	actor->anim_handler = this;
}

void Animation::init(int t)
{
	frame=0;
	done=0;
	type = t;
	switch(type) {
		case A_SWORD_SLASH:
			set_duration(0.15);
			break;
		case A_TEST:
			test();
			break;
	}
}

void Animation::play()
{
	switch(type) {
		case A_SWORD_SLASH:
			sword_slash();
			break;
		case A_TEST:
			test();
			break;
	}
	if(done){
		while(nactors) {
			nactors--;
			actors[nactors]->anim_handler=NULL;
		}
	}
}

void Animation::cancel()
{
	frame = nframes;
	play();
}

void Animation::set_frames(int frames)
{
	nframes = frames;
}

void Animation::set_duration(float duration)
{
	nframes = duration * 60;
}

void Animation::sword_slash()
{
	static double tix = 0.0;
	static char* info_here = g.info.get_place();
	double startTime, endTime;
	startTime = current_time();
	
	Player* actor = (Player*)actors[0];
	static Vec orig_pos;
	static Flt orig_rot;
	if(frame==0){
		VecCopy(actor->rhand_pos, orig_pos);
		//VecCopy(actor->rhand_dir, orig_dir);
		orig_rot = actor->rhand_rot;
		//actor->rhand_pos[0] = 50;
		actor->rhand_pos[1] = 35;

	}
	if (frame==nframes+1) {
		VecCopy(orig_pos, actor->rhand_pos);
		//VecCopy(orig_dir, actor->rhand_dir);
		actor->rhand_rot = orig_rot;
		done=1;
	} else {
		actor->rhand_pos[0] = (-100/float(nframes))*float(frame)+50;
		float angle = (90/float(nframes))*float(frame)+(-30);
		//actor->rhand_dir[0] = cosf(angle);
		//actor->rhand_dir[1] = sinf(angle);
		actor->rhand_rot = angle;
		//cout << (-100/28)*frame+50 << endl;
	}
	
	if (frame==4) {
		Vec dir;
		VecMake(-sin(actor->rot*PI/180), cos(actor->rot*PI/180), 0, dir);
		static char* info_here = g.info.get_place();
		sprintf(info_here, "Dir: %f %f %f", actor->rot, dir[0], dir[1]);
		VecAddS(60, dir, actor->pos, actor->attacks[0].pos);
		actor->attacks[0].scale[0] =
		actor->attacks[0].scale[1] = 40;
		actor->attacks[0].active = 1;
		actor->nattacks++;
	} else if (frame>=7 && actor->attacks[0].active) {
		actor->attacks[0].active = 0;
		actor->nattacks--;
	}
	frame++;
	
	//return time spent
	endTime = current_time();
	tix += endTime - startTime;
	sprintf(info_here, "Sword Slash Animation: %f", tix);
}

void Animation::test()
{
	//do stuff
}

void Info::draw(){
	Rect t;
    t.left = 10;
    t.bot = g.yres - 30;
    t.center = 0;
    for(int i=0; i<nstats; i++){
		ggprint8b(&t, 16, 0x00ffffff, stats[i]);
	}
}












