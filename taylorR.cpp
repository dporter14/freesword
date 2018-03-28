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
	e->hitbox.dynamic=1;
	
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
			set_frames(24);
			can_cancel=0;
			break;
		case A_SWORD_SLASH2:
			set_frames(18);
			can_cancel=0;
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
		case A_SWORD_SLASH2:
			sword_slash2();
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
	frame = nframes+1;
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
	}
	
	if(frame<10){
		actor->rhand_pos[0] = (-100/float(9))*float(frame)+50;
		float angle = (90/float(9))*float(frame)+(-30);
		//actor->rhand_dir[0] = cosf(angle);
		//actor->rhand_dir[1] = sinf(angle);
		actor->rhand_rot = angle;
		//cout << (-100/28)*frame+50 << endl;
	}
	
	if(frame==9)
		can_cancel=1;
	
	if (frame==4) {
		Vec dir;
		VecMake(-sin(actor->rot*PI/180), cos(actor->rot*PI/180), 0, dir);
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

void Animation::sword_slash2()
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
	}
	
	if(frame<10){
		actor->rhand_pos[0] = (100/float(9))*float(frame)-50;
		float angle = (-90/float(9))*float(frame)+(30);
		//actor->rhand_dir[0] = cosf(angle);
		//actor->rhand_dir[1] = sinf(angle);
		actor->rhand_rot = angle;
		//cout << (-100/28)*frame+50 << endl;
	}
	
	if (frame==4) {
		Vec dir;
		VecMake(-sin(actor->rot*PI/180), cos(actor->rot*PI/180), 0, dir);
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

void object_collision(Character& o1, Character& o2){
	/*static char* derp = g.info.get_place();
	if(&g.player == &o2)
		sprintf(derp, "col: %0.2f", g.player.vel[0]);
	*/
	Hitbox h1 = o1.hitbox, h2 = o2.hitbox;
	//Log("Dimensions: %0.3f %0.3f\n", h1.scale[0], h2.scale[0]);
	float t = 0;
	bool flag=0;
	float xt1, xt2, yt1, yt2, xt, yt;
	if(h1.intersect(h2)){ 
		static char* derp = g.info.get_place();
		sprintf(derp, "vel: %f", g.player.vel[0]);

		// magic
		if (ABS(o1.vel[0] - o2.vel[0]) > 1e-8) {
			flag = 1;
			xt1 = ((h2.pos[0]-h2.scale[0]) - 
				(h1.pos[0]+h1.scale[0])) / (o1.vel[0] - o2.vel[0]);
			xt2 = ((h2.pos[0]+h2.scale[0]) - 
				(h1.pos[0]-h1.scale[0])) / (o1.vel[0] - o2.vel[0]);
			//Log("Dimensions: %0.3f %0.3f\n", xt1, xt2);
		} else {
			xt1 = xt2 = 1e10;
		}
		
		if (ABS(o1.vel[1] - o2.vel[1]) > 1e-8) {
			flag = 1;
			yt1 = ((h2.pos[1]-h2.scale[1]) - 
				(h1.pos[1]+h1.scale[1])) / (o1.vel[1] - o2.vel[1]);
			yt2 = ((h2.pos[1]+h2.scale[1]) - 
				(h1.pos[1]-h1.scale[1])) / (o1.vel[1] - o2.vel[1]);
		} else {
			yt1 = yt2 = 1e10;
		}
		
		if (flag) {
			xt =  (ABS(xt1) < ABS(xt2)) ? xt1 : xt2;
			yt =  (ABS(yt1) < ABS(yt2)) ? yt1 : yt2;
			t =  (ABS(xt) < ABS(yt)) ? xt : yt;
			t = CLAMP(t * 1.0001, -1.5, 1.5);
			//Log("Dimensions: %0.3f %0.3f\n", xt, yt);
			
			if (ABS(xt) < ABS(yt)) {
				o1.addPos(o1.vel[0]*t, 0);
				o2.addPos(o2.vel[0]*t, 0);
				
				float temp = o1.vel[0];
				o1.setVel(o2.vel[0]*0.8, o1.vel[1]);
				o2.setVel(temp*0.8, o2.vel[1]);
			} else {
				o1.addPos(0, o1.vel[1]*t);
				o2.addPos(0, o2.vel[1]*t);
				
				float temp = o1.vel[1];
				o1.setVel(o1.vel[0], o2.vel[1]*0.8);
				o2.setVel(o2.vel[0], temp*0.8);
			}
		}
	}
}











