// by Taylor Redden
#include <iostream>

#include "global.h"

using namespace std;
void taylor_func()
{
//	strcpy(g.title.text,"Taylor breathes code");
//	g.title.text_color = 0x0000ffff;

}

bool unitTests(){
	if (!rayBoxTest()) {
		cout << "Raybox test failed." << endl;
		return 0;
	}
	return 1;
}

Flt angleFrom(Vec a, Vec b){
	Flt angl;
	Vec cross, upz;
	angl = acos(VecDot(a, b)/(VecLen(a)*VecLen(b)));	
	VecCross(a, b, cross);
	if (VecDot(upz, cross)<0)
		angl = -angl;
	return angl;
}

void spawnEnemy(Flt x, Flt y, Flt rot){
	if(g.number[N_ENEMIES]<MAXENEMIES){
		Enemy *e = &g.enemies[g.number[N_ENEMIES]++];
		VecMake(x, y, 0, e->pos);
		VecMake(1.0, 1.0, 1.0, e->color);
		e->pradius = 25;
		VecMake(50,75,0,e->scale);
		e->state = 0;
		e->max_speed = 6;
		e->rot=rot;
		e->fake_rot=0;
		VecMake(0,0,0,e->vel);
		
		e->swapSprites();
		
		//hitbox
		e->hitbox.scale[0] = e->hitbox.scale[1] = e->pradius;
		e->hitbox.dynamic=1;
	
		//e->sprt = &g.sprites[SB_ENEMY_NORMAL_F];
		
		//weapon
		VecMake(1.0, 1.0, 1.0, e->weapon.color);
		VecMake(0, -60, 10, e->weapon.pos);
		VecAdd(e->weapon.pos, e->pos, e->weapon.pos);
		VecCopy(e->weapon.pos, e->weapon.fake_pos);
		VecMake(10, 30, 0, e->weapon.scale);
		e->weapon.dist = 40;
		e->weapon.rot=180;
		e->weapon.fake_rot=180;
		e->weapon.sprt = &g.sprites[SB_ITEM_SWORD];
		e->weapon.parent = e;
	}
}

void rotateEnemy(int mousex, int mousey)
{
	for (int i=0; i<g.number[N_ENEMIES]; i++) {
		if (mousex<=g.enemies[i].pos[0]+25 && mousex>=g.enemies[i].pos[0]-25) {
			if (mousey<=g.enemies[i].pos[1]+25 && mousey>=g.enemies[i].pos[1]-25) {
				printf("Rotate enemy #%d\n", i);
				g.enemies[i].rot=int(g.enemies[i].rot+90)%360;
				g.enemies[i].swapSprites();
				/*VecMake(0, -60, 10, e->weapon.pos);
				VecAdd(e->weapon.pos, e->pos, e->weapon.pos);
				*/
				//break;
			}
		}
	}

}

void Enemy::attackPlayer()
{
	if(state==S_CHAR_ANGRY){
		Vec toPlayer;
		VecSub(g.player.pos, pos, toPlayer);
		lookAt(g.player.pos[0], g.player.pos[1]);
		float len = VecLen(toPlayer);
		if (len>100){
			if (acos(VecDot(toPlayer, vel)/(len*VecLen(vel)))>(PI/4)){
				addVel(vel[0]*-0.5,vel[1]*-0.5);
			}
			addVel(toPlayer[0]/len,toPlayer[1]/len);
		} else {
			setVel(0,0);
			if(weapon.anim_handler==NULL){
				Animation *act = g.animator.init(A_SWORD_WINDUP);
				act->add_actor(&weapon);
			}
		}/* else {
			if (VecLen(vel)>0) 
				addVel(vel[0]*-0.5,vel[1]*-0.5);
		}*/
		
		
	} else if (VecLen(vel)>0) {
		addVel(vel[0]*-0.5,vel[1]*-0.5);
	}
}

void Enemy::kill()
{
	g.eKilled++;
	if(weapon.anim_handler!=NULL){
		weapon.anim_handler->cancel();
	}
	state = S_CHAR_DEAD;
	if (g.number[N_ITEMS] < 10) {
		if (RND()*100 < 50) {
			std::cout << N_ITEMS << std::endl;
			g.items[g.number[N_ITEMS]].type = I_POTION;
			std::cout << "Set type\n" << std::endl;
			g.items[g.number[N_ITEMS]].spawnPotion(pos[0], pos[1]);
			std::cout << "Spawn\n" << std::endl;
			g.number[N_ITEMS]++;
		} else if (RND()*100 < 50) {
			g.items[g.number[N_ITEMS]].type = I_AMMO;
			g.items[g.number[N_ITEMS]].spawnAmmo(pos[0], pos[1]);
			g.number[N_ITEMS]++;
		}
	}
}

void Enemy::see(){
	Vec toPlayer;
	VecSub(g.player.pos, pos, toPlayer);
	Flt len = VecLen(toPlayer);
	VecS(1/len, toPlayer, toPlayer);
	
	Vec base, upz, cross;
	VecMake(0, 1, 0, base);
	VecMake(0, 0, 1, upz);
	Flt angl = acos(VecDot(toPlayer, base));	
	VecCross(base, toPlayer, cross);
	if (VecDot(upz, cross)<0)
		angl=-angl+2*PI;
	static char* info_here = g.info.get_place();
	sprintf(info_here, "See: %f %f %f %f", len, v_dist, angl*180/PI, rot);
	if (!wallBetween(*this,g.player)){
		if (ABS((angl*180/PI)-rot)<(v_fov/2) && len<v_dist){
			state = S_CHAR_ANGRY;
		}
		if (len<v_close){
			state = S_CHAR_ANGRY;
		}
	}
}

/////////////* Animation Functions */////////////////

Animation* Animator::init(anim_type type){
	if (nanims<MAXANIMATIONS) {
		Animation* a = &anims[nanims++];
		a->init(type);
		return a;
	}
	else
		return NULL;
}

void Animator::checkin(Animation* anim){
	for(int i=0; i<nanims; i++) {
		if (&anims[i]==anim) {
			anims[i]=anims[--nanims];
			break;
		}
	}
}

void Animator::play(){
	int i=0;
	while ( i < nanims ) {
		anims[i].play();
		if(anims[i].done){
			//g.anims[i].clear();
			//printf("fram: %d\n",anims[i].frame);
			anims[i]=anims[--nanims];
		} else {
			i++;
		}
	}
}

void Animation::add_actor(Object* actor)
{
	actors[nactors++] = actor;
	actor->anim_handler = this;
}

void Animation::init(anim_type t)
{
	frame=0;
	done=0;
	nactors=0;
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
		case A_SWORD_WINDUP:
			set_frames(10);
			can_cancel=0;
			break;
		case A_BOW_DRAW:
			set_frames(60);
			can_cancel=1;
			break;
		case A_BOW_RELEASE:
			set_frames(10);
			can_cancel=0;
			break;
		case A_SPECIAL_WINDUP:
			set_frames(120);
			can_cancel=1;
			break;
		case A_SPECIAL_RELEASE:
			set_frames(30);
			can_cancel=0;
			break;
		case A_TEST:
			test();
			break;
	}
}

void Animation::play()
{
	//printf("play\n");
	switch(type) {
		case A_SWORD_SLASH:
			sword_slash();
			break;
		case A_SWORD_SLASH2:
			sword_slash2();
			break;
		case A_SWORD_WINDUP:
			sword_windup();
			break;
		case A_BOW_DRAW:
			bow_draw();
			break;
		case A_BOW_RELEASE:
			bow_release();
			break;
		case A_SPECIAL_WINDUP:
			special_windup();
			break;
		case A_SPECIAL_RELEASE:
			special_release();
			break;
		case A_TEST:
			test();
			break;
	}
	if(done){
		//printf("yep\n");
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
	//printf("can: %d\n",done);
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
	
	Weapon* actor = (Weapon*)actors[0];
	
	if(frame==0){
		actor->faking = 0;
		VecCopy(actor->pos, actor->orig_pos);
		//VecCopy(actor->dir, orig_dir);
		actor->orig_rot = actor->rot;
		//actor->pos[0] = 50;
		//VecCopy(actor->pos, actor->fake_pos);
		//actor->pos[1] += 35;
	
	}
	if (frame==nframes+1) {
		//VecCopy(orig_pos, actor->pos);
		//VecCopy(orig_dir, actor->dir);
		actor->rot = actor->orig_rot;
		actor->faking = 0;
		done=1;
		
	}
	
	//Vec dir;
	//VecMake(-sin(orig_rot*PI/180), cos(orig_rot*PI/180), 0, dir);
		
	if(frame<10){
		float angle = 90*(float(frame)/9.0)+(actor->orig_rot-30);
		Vec temp;
		VecSub(actor->pos, actor->parent->pos, temp);
		float s = 70;//VecLen(temp);
		actor->pos[0] = -sin(angle*PI/180)*s+actor->parent->pos[0];
		actor->pos[1] = cos(angle*PI/180)*s+actor->parent->pos[1];
		actor->fake_rot = angle;
		//printf("angl: %f\n", angle);
		//actor->rot = angle;
		
		//cout << (-100/28)*frame+50 << endl;
	} else if (frame<nframes) {
		float angle = 60+actor->orig_rot;
		Vec temp;
		VecSub(actor->pos, actor->parent->pos, temp);
		float s = 70;//VecLen(temp);
		actor->pos[0] = -sin(angle*PI/180)*s+actor->parent->pos[0];
		actor->pos[1] = cos(angle*PI/180)*s+actor->parent->pos[1];
		actor->fake_rot = angle;
	}
	if(frame==9)
		can_cancel=1;
	
	if (frame==4) {
		//VecAddS(60, dir, orig_pos, actor->attacks[0].pos);
		VecCopy(actor->pos, actor->attacks[0].pos);
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
	
	Weapon* actor = (Weapon*)actors[0];
	
	if(frame==0){
		actor->faking = 0;
		VecCopy(actor->pos, actor->orig_pos);
		//VecCopy(actor->dir, orig_dir);
		actor->orig_rot = actor->rot;
		//actor->pos[0] = 50;
		//VecCopy(actor->pos, actor->fake_pos);
		//actor->pos[1] += 35;

	}
	if (frame==nframes+1) {
		//VecCopy(orig_pos, actor->pos);
		//VecCopy(orig_dir, actor->dir);
		actor->rot = actor->orig_rot;
		actor->faking = 0;
		done=1;
	}
	//Vec dir;
	//VecMake(-sin(orig_rot*PI/180), cos(orig_rot*PI/180), 0, dir);
		
	if(frame<10){
		float angle = -90*(float(frame)/9.0)+(actor->orig_rot+30);
		Vec temp;
		VecSub(actor->pos, actor->parent->pos, temp);
		float s = 70;//VecLen(temp);
		actor->pos[0] = -sin(angle*PI/180)*s+actor->parent->pos[0];
		actor->pos[1] = cos(angle*PI/180)*s+actor->parent->pos[1];
		actor->fake_rot = angle;
		//printf("angl: %f\n", angle);
		//actor->rot = angle;
		
		//cout << (-100/28)*frame+50 << endl;
	} else if (frame<nframes) {
		float angle = -60+actor->orig_rot;
		Vec temp;
		VecSub(actor->pos, actor->parent->pos, temp);
		float s = 70;//VecLen(temp);
		actor->pos[0] = -sin(angle*PI/180)*s+actor->parent->pos[0];
		actor->pos[1] = cos(angle*PI/180)*s+actor->parent->pos[1];
		actor->fake_rot = angle;
	}
	if(frame==9)
		can_cancel=1;
	
	if (frame==4) {
		//VecAddS(60, dir, orig_pos, actor->attacks[0].pos);
		VecCopy(actor->pos, actor->attacks[0].pos);
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

void Animation::sword_windup()
{
	Weapon* actor = (Weapon*)actors[0];
	static Flt orig_dist;
	if(frame==0){
		VecCopy(actor->pos, actor->orig_pos);
		//VecCopy(actor->dir, orig_dir);
		actor->orig_rot = actor->rot;
		orig_dist = actor->dist;
		//actor->pos[0] = 50;
		//actor->pos[1] = 35;

	}
	if (frame==nframes+1) {
		VecCopy(actor->orig_pos, actor->pos);
		//VecCopy(orig_dir, actor->dir);
		actor->rot = actor->orig_rot;
		actor->dist = orig_dist;
		
		done=1;
	}
	
	if(frame<10){
		actor->dist = 70*(float)frame/nframes;
		float angle = (actor->orig_rot-60)*(float)frame/nframes;
		actor->pos[0] = -sin(angle*PI/180)*actor->dist+actor->parent->pos[0];
		actor->pos[1] = cos(angle*PI/180)*actor->dist+actor->parent->pos[1];
		//actor->dir[0] = cosf(angle);
		//actor->dir[1] = sinf(angle);
		actor->fake_rot = angle;
		//cout << (-100/28)*frame+50 << endl;
	}
	
	if(frame==9) {
		cancel();
		Animation *act = g.animator.init(A_SWORD_SLASH);
		act->add_actor(actor);
		return;
	}
	frame++;
	
}

void Animation::bow_draw() {
	Weapon* actor = (Weapon*)actors[0];
	if (frame==0){
		actor->sprt->nextFrame();
	} else if (frame==15) {
		actor->sprt->nextFrame();
	} else if (frame==30) {
		actor->sprt->nextFrame();
	}
	if(frame==nframes+1){
		done=1;
	}
	if(frame<40)
		frame++;
}

void Animation::bow_release() {
	Weapon* actor = (Weapon*)actors[0];
	if(frame==0){
		actor->sprt->setFrame(0);
		done=1;
	}
	frame++;
}

void Animation::special_windup() {
	Weapon* actor = (Weapon*)actors[0];
	//printf("%d\n",frame);
	if(frame==0){
		actor->faking = 0;
		VecCopy(actor->pos, actor->orig_pos);
		//VecCopy(actor->dir, orig_dir);
		actor->orig_rot = actor->rot;
		//actor->pos[0] = 50;
		//VecCopy(actor->pos, actor->fake_pos);
		//actor->pos[1] += 35;
		actor->sprt = &g.sprites[SB_SPECIAL];

	}
	
	if (frame==nframes+1) {
		//VecCopy(orig_pos, actor->pos);
		//VecCopy(orig_dir, actor->dir);
		actor->rot = actor->orig_rot;
		actor->faking = 0;
		this->done=1;
		actor->sprt = &g.sprites[SB_ITEM_SWORD];
		return;
		//printf("fasg\n");
	}
	//Vec dir;
	//VecMake(-sin(orig_rot*PI/180), cos(orig_rot*PI/180), 0, dir);
		
	if (frame<nframes) {
		float angle = 60+actor->orig_rot;
		Vec temp;
		VecSub(actor->pos, actor->parent->pos, temp);
		float s = 70;//VecLen(temp);
		actor->pos[0] = -sin(angle*PI/180)*s+actor->parent->pos[0];
		actor->pos[1] = cos(angle*PI/180)*s+actor->parent->pos[1];
		actor->fake_rot = angle;
	}
	//printf("asdfsd\n");
	//printf("%d %d\n",frame, actor->sprt->nframes);
	if (frame==20){	
		actor->sprt->nextFrame();
	} else if (frame==40) {
		actor->sprt->nextFrame();
	} else if (frame==60) {
		actor->sprt->nextFrame();
	} else if (frame==80) {
		actor->sprt->setFrame(4);
	} else if (frame==85) {
		actor->sprt->setFrame(0);
	}
	
	if (frame==85)
		frame=75;
	
	frame++;
}

void Animation::special_release()
{
	Weapon* actor = (Weapon*)actors[0];
	
	if(frame==0){
		actor->faking = 0;
		VecCopy(actor->pos, actor->orig_pos);
		//VecCopy(actor->dir, orig_dir);
		actor->orig_rot = actor->rot;
		//actor->pos[0] = 50;
		//VecCopy(actor->pos, actor->fake_pos);
		//actor->pos[1] += 35;

	}
	if (frame==nframes+1) {
		//VecCopy(orig_pos, actor->pos);
		//VecCopy(orig_dir, actor->dir);
		actor->rot = actor->orig_rot;
		actor->faking = 0;
		done=1;
		if (actor->attacks[0].active) {
			actor->attacks[0].active = 0;
			actor->nattacks=0;
		}
	}
	//Vec dir;
	//VecMake(-sin(orig_rot*PI/180), cos(orig_rot*PI/180), 0, dir);
		
	if(frame<nframes){
		float angle = actor->orig_rot+60-10*frame;
		while (angle<180){
			angle+=360;
		}
		Vec temp;
		VecSub(actor->pos, actor->parent->pos, temp);
		float s = 70;//VecLen(temp);
		actor->pos[0] = -sin(angle*PI/180)*s+actor->parent->pos[0];
		actor->pos[1] = cos(angle*PI/180)*s+actor->parent->pos[1];
		actor->fake_rot = angle;
		//printf("angl: %f\n", angle);
		//actor->rot = angle;
		
		//cout << (-100/28)*frame+50 << endl;
	}
	
	if (frame>4) {
		//VecAddS(60, dir, orig_pos, actor->attacks[0].pos);
		VecCopy(actor->pos, actor->attacks[0].pos);
		actor->attacks[0].scale[0] =
		actor->attacks[0].scale[1] = 40;
		actor->attacks[0].active = 1;
		actor->nattacks=1;
	}
	frame++;
}

void Animation::test()
{
	//do stuff
}

//Infobox

void Info::draw(){
	Rect t;
	t.left = 10;
	t.bot = g.yres - 30;
	t.center = 0;
	for(int i=0; i<nstats; i++){
		ggprint8b(&t, 16, 0x00ffffff, stats[i]);
	}
}

void characterCollision(Character& o1, Character& o2)
{
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

float rayBox(Ray &r, Hitbox &h)
{
	float tmin, tmax;
	float tymin, tymax;
	
	if (r.d[0] >= 0) {
		tmin = (h.pos[0] - h.scale[0] - r.o[0]) / r.d[0];
		tmax = (h.pos[0] + h.scale[0] - r.o[0]) / r.d[0];
	}
	else {
		tmax = (h.pos[0] - h.scale[0] - r.o[0]) / r.d[0];
		tmin = (h.pos[0] + h.scale[0] - r.o[0]) / r.d[0];
	}
	
	if (r.d[1] >= 0) {
		tymin = (h.pos[1] - h.scale[1] - r.o[1]) / r.d[1];
		tymax = (h.pos[1] + h.scale[1] - r.o[1]) / r.d[1];	
	}
	else {
		tymax = (h.pos[1] - h.scale[1] - r.o[1]) / r.d[1];
		tymin = (h.pos[1] + h.scale[1] - r.o[1]) / r.d[1];
	}
	
	if ((tmin > tymax) || (tymin > tmax)) 
        return -1;
	
	if (tymin > tmin) 
        tmin = tymin; 

	if (tmin >= 0)
		return tmin;
		
    if (tymax < tmax) 
        tmax = tymax;
        
	if (tmax >= 0)
		return tmax;
		
	return -1;
	
}

bool rayBoxTest()
{
	Ray r;
	Hitbox h;
	float result;
	VecMake(2,2,0,h.pos);
	VecMake(1,1,0,h.scale);
	
	VecMake(0,0,0,r.o);
	VecMake(1,1,0,r.d);
	result=rayBox(r,h);
	if(result!=1.00) {
		printf("%0.2f\n",result);
		return 0;
	}
	
	VecMake(2,5,0,r.o);
	VecMake(0,-1,0,r.d);
	result=rayBox(r,h);
	if(result!=2.00) {
		printf("%0.2f\n",result);
		return 0;
	}
	
	VecMake(0,3,0,r.o);
	VecMake(1,0,0,r.d);
	result=rayBox(r,h);
	if(result!=1.00) {
		printf("%0.2f\n",result);
		return 0;
	}
	
	VecMake(2,5,0,r.o);
	VecMake(-1,-1,0,r.d);
	result=rayBox(r,h);
	if(result!=-1) {
		printf("%0.2f\n",result);
		return 0;
	}
	
	return 1;
}

bool wallBetween(Object& o1, Object& o2)
{
	float dist, close=1e10;
	Ray r;
	VecCopy(o1.pos,r.o);
	VecSub(o2.pos,o1.pos,r.d);
	
	//Log("Pl: %0.2f %0.2f\n",g.player.pos[0],g.player.pos[1]);
	//Log("Dir: %0.2f %0.2f\n",r.d[0],r.d[1]);
	for (int k=0; k<g.number[N_WALLS]; k++) {
		dist = rayBox(r, g.level.walls[k].hitbox);
		//Log("%0.2f\n",dist);
		if (dist>0 && dist<close)
			close = dist;
	}
	
	for (int k=0; k<g.number[N_DOORS]; k++) {
		if(!g.level.doors[k].isOpen){
			dist = rayBox(r, g.level.doors[k].hitbox);
			if (dist>0 && dist<close)
				close = dist;
		}
	}
	
	//printf("%f\n",close);
	return (close < 1);
		
}

string getTile(float x, float y)
{
	char holder[20];
	int xx = floor((x/50)+0.5);
	int yy = floor((y/50)+0.5);
	//printf("%f %f\n",(x/50)+0.5,(y/50)+0.5);
	sprintf(holder,"%d %d",xx,yy);
	return holder;
}
void placeTile(float x, float y) 
{
	//static char* info_here = g.info.get_place();
	
	string index = getTile(x,y);
	//sprintf(info_here, "tile: %f %f %s", x, y, index.c_str());
	switch(g.state[S_TILE]) {
		case 0:
			g.tilemap[index]=0;
			break;
		case 1:
			g.tilemap[index]=1;
			break;
		case 2:
			if (RND()<0.04)
				g.tilemap[index]=3;
			else
				g.tilemap[index]=2;
			break;
		case 3:
			g.tilemap[index]=4;
			break;
		
	}
	
}

void drawTiles(){
	int fun=0;
	float x = floor(g.player.pos[0]/50+0.5)*50;
	float y = floor(g.player.pos[1]/50+0.5)*50;
	for(int i = -12+fun; i < 13-fun; i++){
		for(int j = -9+fun; j < 10-fun; j++){
			string index = getTile(x+50*i,y+50*j);
			//cout << index << endl;
			map<string,int>::iterator it = g.tilemap.find(index);
			if (it == g.tilemap.end())
				continue;
			//cout << it->first << ": " << it->second << endl;
			int tile = it->second;
			if (tile==0) {
				g.tilemap.erase (it);
				continue;
			}
			glPushMatrix();
			glColor3f(1,1,1);
			glTranslatef(x+50*i, y+50*j, -10);
			glBindTexture(GL_TEXTURE_2D, g.spriteTextures[SS_TILES].tex);
			Sprite* sprt;
			switch(tile) {
				case 1:
					sprt = &g.sprites[SB_TILE_WOOD];
					break;
				case 2:
					sprt = &g.sprites[SB_TILE_GRASS];
					break;
				case 3:
					sprt = &g.sprites[SB_TILE_GRASS2];
					break;
				case 4:
					sprt = &g.sprites[SB_TILE_STONE];
					break;
			}

			glBegin(GL_QUADS);
				glTexCoord2f(sprt->pos[0], sprt->pos[1]);
				glVertex3f(-25,  25, 0);
		
				glTexCoord2f(sprt->pos[0]+sprt->w, sprt->pos[1]);
				glVertex3f( 25,  25, 0);
		
				glTexCoord2f(sprt->pos[0]+sprt->w, sprt->pos[1]+sprt->h);
				glVertex3f( 25, -25, 0);
		
				glTexCoord2f(sprt->pos[0], sprt->pos[1]+sprt->h);
				glVertex3f(-25, -25, 0);
			glEnd();
			glBindTexture(GL_TEXTURE_2D,0);
	
			glPopMatrix();
		}
	}
	//cin.get();
}

void clearTiles(){
	//not sure what i was expecting
	g.tilemap.clear();
}

void Player::swapWeapon(wep_type type){
	switch(type) {
		case W_BOW:
			weapon.type=W_BOW;
			VecMake(60, 90, 0, weapon.scale);
			weapon.sprt = &g.sprites[SB_ITEM_BOW];
			break;
		case W_SWORD:
			weapon.type=W_SWORD;
			VecMake(10, 30, 0, weapon.scale);
			weapon.sprt = &g.sprites[SB_ITEM_SWORD];
			break;
		case W_NONE:
			break;
	}
}

void spawnArrow(){
	if(g.number[N_ARROWS]<MAXARROWS){
		g.player.ammo--;
		int num=g.number[N_ARROWS];
		g.number[N_ARROWS]++;
		Arrow& arrow = g.arrows[num];
		
		VecMake(1,1,1, arrow.color);
		VecCopy(g.player.pos, arrow.pos);
		VecMake(60,9,0,arrow.scale);
		arrow.rot = g.player.rot;
		arrow.fake_rot = arrow.rot+90;
		
		arrow.vel[0] = -sin(arrow.rot*PI/180)*15;//arrow.speed;
		arrow.vel[1] = cos(arrow.rot*PI/180)*15;//arrow.speed;
		VecAddS(1, arrow.vel, arrow.pos, arrow.pos);
		VecMake(10,10,0,arrow.hitbox.scale);
		
		arrow.hitbox.active = 1;
		
		arrow.sprt = &g.sprites[SB_ITEM_ARROW];
	}
}

void Arrow::move(){
	Vec diff;
	VecCopy(pos,diff);
	pos[0] += vel[0];
	pos[1] += vel[1];
	//calc difference between new and old positions
	VecSub(pos, diff, diff);
	hitbox.pos[0] = pos[0]-sin(rot*PI/180)*35;
	hitbox.pos[1] = pos[1]+cos(rot*PI/180)*35;

}