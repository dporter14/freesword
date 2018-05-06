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
		VecMake(0.2, 0.2, 0.2, e->color);
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
		
		//weapon
		VecMake(1.0, 1.0, 1.0, e->weapon.color);
		VecMake(0, 60, 10, e->weapon.pos);
		VecAdd(e->weapon.pos, e->pos, e->weapon.pos);
		VecCopy(e->weapon.pos, e->weapon.fake_pos);
		VecMake(10, 30, 0, e->weapon.scale);
		e->weapon.rot=0;
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
		} else if (VecLen(vel)>0) {
			addVel(vel[0]*-0.5,vel[1]*-0.5);
			//if(VecLen(vel)<0.7){
				setVel(0,0);
				if(anim_handler==NULL){
					Animation *act = g.animator.init(A_SWORD_WINDUP);
					act->add_actor(&weapon);
				}
			//}
		}
		
		
	} else if (VecLen(vel)>0) {
		addVel(vel[0]*-0.5,vel[1]*-0.5);
	}
}

void Enemy::kill(){
	g.eKilled++;
	state = S_CHAR_DEAD;
}

void Enemy::see(){
	Vec toPlayer, temp;
	VecSub(g.player.pos, pos, toPlayer);
	Flt len = VecLen(toPlayer);
	VecS(1/len, toPlayer, toPlayer);
	VecMake(0,1,0,temp);
	Flt a = acos(VecDot(toPlayer, temp));
	static char* info_here = g.info.get_place();
	sprintf(info_here, "See: %f %f %f", len, a, rot);
	if (!wallBetween(*this,g.player)){
		if ((a*180/PI)-rot<(v_fov/2) && len<v_dist){
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
			set_frames(10);
			can_cancel=1;
			break;
		case A_BOW_RELEASE:
			set_frames(10);
			can_cancel=1;
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
		case A_SWORD_WINDUP:
			sword_windup();
			break;
		case A_BOW_DRAW:
			bow_draw();
			break;
		case A_BOW_RELEASE:
			bow_release();
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
	
	Weapon* actor = (Weapon*)actors[0];
	
	static Vec orig_pos;
	static Flt orig_rot;
	if(frame==0){
		//VecCopy(actor->pos, orig_pos);
		//VecCopy(actor->dir, orig_dir);
		orig_rot = actor->rot;
		//actor->pos[0] = 50;
		//actor->fake_pos[1] += 35;

	}
	if (frame==nframes+1) {
		VecCopy(actor->pos, actor->fake_pos);
		//VecCopy(orig_dir, actor->dir);
		actor->rot = orig_rot;
		done=1;
	}
	Vec dir;
	//VecMake(-sin(orig_rot*PI/180), cos(orig_rot*PI/180), 0, dir);
		
	if(frame<10){
		float angle = 90*(float(frame)/9.0)+(orig_rot-30);
		actor->pos[0] += (-100/float(9))*float(frame)+50;
		actor->pos[1] += (-100/float(9))*float(frame)+50;
		//actor->dir[0] = cosf(angle);
		//actor->dir[1] = sinf(angle);
		actor->rot = angle;
		//cout << (-100/28)*frame+50 << endl;
	}
	
	if(frame==9)
		can_cancel=1;
	
	if (frame==4) {
		VecAddS(60, dir, orig_pos, actor->attacks[0].pos);
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
	Weapon* actor = (Weapon*)actors[0];
	static Vec orig_pos;
	static Flt orig_rot;
	if(frame==0){
		VecCopy(actor->pos, orig_pos);
		//VecCopy(actor->dir, orig_dir);
		orig_rot = actor->rot;
		//actor->pos[0] = 50;
		actor->pos[1] = 35;

	}
	if (frame==nframes+1) {
		VecCopy(orig_pos, actor->pos);
		//VecCopy(orig_dir, actor->dir);
		actor->rot = orig_rot;
		done=1;
	}
	Vec dir;
	
	if(frame<10){
		actor->pos[0] = (100/float(9))*float(frame)-50;
		float angle = (-90/float(9))*float(frame)+(30);
		//actor->dir[0] = cosf(angle);
		//actor->dir[1] = sinf(angle);
		actor->rot = angle;
		//cout << (-100/28)*frame+50 << endl;
	}
	VecMake(-sin(orig_rot*PI/180), cos(orig_rot*PI/180), 0, dir);
	
	if (frame==4) {
		VecAddS(60, dir, orig_pos, actor->attacks[0].pos);
		actor->attacks[0].scale[0] =
		actor->attacks[0].scale[1] = 40;
		actor->attacks[0].active = 1;
		actor->nattacks++;
	} else if (frame>=7 && actor->attacks[0].active) {
		actor->attacks[0].active = 0;
		actor->nattacks--;
	}
	frame++;
	
}

void Animation::sword_windup()
{
	Enemy* actor = (Enemy*)actors[0];
	static Vec orig_pos;
	static Flt orig_rot;
	if(frame==0){
		VecCopy(actor->pos, orig_pos);
		//VecCopy(actor->dir, orig_dir);
		orig_rot = actor->rot;
		//actor->pos[0] = 50;
		//actor->pos[1] = 35;

	}
	if (frame==nframes+1) {
		VecCopy(orig_pos, actor->pos);
		//VecCopy(orig_dir, actor->dir);
		actor->rot = orig_rot;
		done=1;
	}
	
	if(frame<10){
		actor->pos[1] = 50*(float)frame/nframes;
		float angle = -60*(float)frame/nframes;
		//actor->dir[0] = cosf(angle);
		//actor->dir[1] = sinf(angle);
		actor->rot = angle;
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
	
	if(frame==12)
		frame=1;
}

void Animation::bow_release() {}

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
		dist = rayBox(r, g.level.doors[k].hitbox);
		if (dist>0 && dist<close)
			close = dist;
	}
	
	//printf("%f\n",close);
	return (close < 1);
		
}

string getTile(float x, float y)
{
	char holder[20];
	int xx = round(x/50);
	int yy = round(y/50);
	sprintf(holder,"%d %d",xx,yy);
	return holder;
}
void placeTile(float x, float y) 
{
	string index = getTile(x,y);
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
	for(float x = (g.player.pos[0]-(g.xres/2))+fun; x < (g.player.pos[0]+(g.xres/2))+50-fun; x+=50){
		for(float y = (g.player.pos[1]-(g.yres/2))+fun; y < (g.player.pos[1]+(g.yres/2))+50-fun; y+=50){
			string index = getTile(x,y);
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
			glTranslatef(round(x/50)*50, round(y/50)*50, -900);
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
}

void clearTiles(){
	//not sure what i was expecting
	g.tilemap.clear();
}
