
#include "global.h"

Global g;
Image img[1] = {"./images/grillbys-reference5.png" };				
//Image img[1] = {"./images/marble.png" };				

class X11_wrapper {
	private:
		Display *dpy;
		Window win;
	public:
		X11_wrapper() {
			GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
			XSetWindowAttributes swa;
			setupScreenRes(g.xres, g.yres);
			dpy = XOpenDisplay(NULL);
			if (dpy == NULL) {
				printf("\n\tcannot connect to X server\n\n");
				exit(EXIT_FAILURE);
			}
			Window root = DefaultRootWindow(dpy);
			XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
			if (vi == NULL) {
				printf("\n\tno appropriate visual found\n\n");
				exit(EXIT_FAILURE);
			}
			Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
			swa.colormap = cmap;
			swa.event_mask = ExposureMask |
				KeyPressMask |
				KeyReleaseMask |
				ButtonPressMask |
				ButtonReleaseMask |
				PointerMotionMask |
				StructureNotifyMask |
				SubstructureNotifyMask;
			win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,	vi->depth,
					InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
			GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
			glXMakeCurrent(dpy, win, glc);
			setTitle();
		}
		~X11_wrapper() {
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
		}
		void setTitle() {
			//Set the window title bar.
			XMapWindow(dpy, win);
			XStoreName(dpy, win, "Free Sword");
		}
		void setupScreenRes(const int w, const int h) {
			g.xres = w;
			g.yres = h;
		}
		void reshapeWindow(int width, int height) {
			//window has been resized.
			setupScreenRes(width, height);
			//
			glViewport(0, 0, (GLint)width, (GLint)height);
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			glOrtho(0, g.xres, 0, g.yres, -1, 1);
			setTitle();
		}
		void checkResize(XEvent *e) {
			//The ConfigureNotify is sent by the server if the window is resized.
			if (e->type != ConfigureNotify)
				return;
			XConfigureEvent xce = e->xconfigure;
			if (xce.width != g.xres || xce.height != g.yres) {
				//Window size did change.
				reshapeWindow(xce.width, xce.height);
			}
		}
		bool getXPending() {
			return XPending(dpy);
		}
		XEvent getXNextEvent() {
			XEvent e;
			XNextEvent(dpy, &e);
			return e;
		}
		void swapBuffers() {
			glXSwapBuffers(dpy, win);
		}
} x11;

//function prototypes
void initOpengl(void);
int checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void initSounds(void);

void gameUpdate();
void animation(void);
void physics(void);
void render(void);

#ifdef USE_OPENAL_SOUND
void initSound();
void cleanupSound();
void playSound(ALuint source);

#endif //USE_OPENAL_SOUND


//-----------------------------------------------------------------------------
//Setup timers

double current_time()
{
	struct timespec now;

	//https://gist.github.com/jbenet/1087739
	#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
		clock_serv_t cclock;
		mach_timespec_t mts;
		host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
		clock_get_time(cclock, &mts);
		mach_port_deallocate(mach_task_self(), cclock);
		now.tv_sec = mts.tv_sec;
		now.tv_nsec = mts.tv_nsec;
	#else
		clock_gettime(CLOCK_REALTIME, &now);
	#endif

	return now.tv_sec + now.tv_nsec*1e-9;
}

const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
//struct timespec timeStart, timeCurrent;
//struct timespec timePause;
double timeStart, timeCurrent;
double timePause;
double physicsCountdown = 0.0;
double timeSpan = 0.0;
double timeDiff(struct timespec *start, struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec ) +
			(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}

//-----------------------------------------------------------------------------
/*
double timeDiff(struct timespec *start, struct timespec *end)
{
    return (double)(end->tv_sec = start->tv_sec) +
        (double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
*/
int main(int argc, char *argv[])
{
	if (argc) {}
	if (argv[0]) {}
	logOpen();
	initOpengl();
	init();
	initialize_fonts();
	srand((unsigned int)time(NULL));
	timePause = current_time();
	timeStart = current_time();
	int done = 0;
	while (!done) {
		gameUpdate();
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			done |= checkMouse(&e);
			done |= checkKeys(&e);
		}
		//
		//Below is a process to apply physics at a consistent rate.
		//1. Get the time right now.
		timeCurrent=current_time();
		//2. How long since we were here last?
		timeSpan = timeCurrent-timeStart;
		//3. Save the current time as our new starting time.
		timeStart=timeCurrent;
		//4. Add time-span to our countdown amount.
		physicsCountdown += timeSpan;
		//5. Has countdown gone beyond our physics rate?
		//	   if yes,
		//		   In a loop...
		//			  Apply physics
		//			  Reducing countdown by physics-rate.
		//			  Break when countdown < physics-rate.
		//	   if no,
		//		   Apply no physics this frame.
		while(physicsCountdown >= physicsRate) {
			//6. Apply physics
			if (!g.state[S_PAUSED]) {
				animation();
		    	physics();
			}
			//7. Reduce the countdown by our physics-rate
			physicsCountdown -= physicsRate;
		}
		//Always render every frame.
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
	return 0;
}


/*======================================================
   SOUND =================================================
  ======================================================*/
void initSound()
{
	#ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	g.alBufferDrip = alutCreateBufferFromFile("./sounds/drip.wav");
	g.alBufferTick = alutCreateBufferFromFile("./sounds/tick.wav");
	//
	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &g.alSourceDrip);
	alSourcei(g.alSourceDrip, AL_BUFFER, g.alBufferDrip);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(g.alSourceDrip, AL_GAIN, 1.0f);
	alSourcef(g.alSourceDrip, AL_PITCH, 1.0f);
	alSourcei(g.alSourceDrip, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	//Generate a source, and store it in a buffer.
	alGenSources(1, &g.alSourceTick);
	alSourcei(g.alSourceTick, AL_BUFFER, g.alBufferTick);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(g.alSourceTick, AL_GAIN, 1.0f);
	alSourcef(g.alSourceTick, AL_PITCH, 1.0f);
	alSourcei(g.alSourceTick, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	#endif //USE_OPENAL_SOUND
	return;
}

void cleanupSound()
{
	#ifdef USE_OPENAL_SOUND
	//First delete the source.
	alDeleteSources(1, &g.alSourceDrip);
	alDeleteSources(1, &g.alSourceTick);
	//Delete the buffer.
	alDeleteBuffers(1, &g.alBufferDrip);
	alDeleteBuffers(1, &g.alBufferTick);
	//Close out OpenAL itself.
	//Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context.
	alcMakeContextCurrent(NULL);
	//Release context(s).
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);
	#endif //USE_OPENAL_SOUND
	return;
}

void playSound(ALuint source)
{
	#ifdef USE_OPENAL_SOUND
		alSourcePlay(source);
	#endif //USE_OPENAL_SOUND
	return;
}

void initOpengl(void)
{
	//OpenGL initialization
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	//
	//choose one of these
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	glEnable(GL_TEXTURE_2D);
	//marble_texture = loadBMP("marble.bmp");
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	//load the image file into a ppm structure.
	//
	g.bgImage = &img[0];
	Log("Dimensions: %d %d\n", g.bgImage->width, g.bgImage->height);
				
	//
	//create opengl texture elements
	glGenTextures(1, &g.bgTexture);
	glBindTexture(GL_TEXTURE_2D, g.bgTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			g.bgImage->width, g.bgImage->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, g.bgImage->data);
	
}



void init()
{
	g.player.init();
    g.currentLevel = 1;
    g.level1.buildLevel1();

    //
	//initialize buttons...
	/*
	   g.nbuttons=0;
	//size and position
	g.button[g.nbuttons].r.width = 140;
	g.button[g.nbuttons].r.height = 60;
	g.button[g.nbuttons].r.left = 20;
	g.button[g.nbuttons].r.bot = 320;
	g.button[g.nbuttons].r.right =
	g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.width;
	g.button[g.nbuttons].r.top =
	g.button[g.nbuttons].r.bot + g.button[g.nbuttons].r.height;
	g.button[g.nbuttons].r.centerx =
	(g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.right) / 2;
	g.button[g.nbuttons].r.centery =
	(g.button[g.nbuttons].r.bot + g.button[g.nbuttons].r.top) / 2;
	strcpy(g.button[g.nbuttons].text, "Reset");
	g.button[g.nbuttons].down = 0;
	g.button[g.nbuttons].click = 0;
	g.button[g.nbuttons].color[0] = 0.4f;
	g.button[g.nbuttons].color[1] = 0.4f;
	g.button[g.nbuttons].color[2] = 0.7f;
	g.button[g.nbuttons].dcolor[0] = g.button[g.nbuttons].color[0] * 0.5f;
	g.button[g.nbuttons].dcolor[1] = g.button[g.nbuttons].color[1] * 0.5f;
	g.button[g.nbuttons].dcolor[2] = g.button[g.nbuttons].color[2] * 0.5f;
	g.button[g.nbuttons].text_color = 0x00ffffff;
	g.nbuttons++;
	g.button[g.nbuttons].r.width = 140;
	g.button[g.nbuttons].r.height = 60;
	g.button[g.nbuttons].r.left = 20;
	g.button[g.nbuttons].r.bot = 160;
	g.button[g.nbuttons].r.right =
	g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.width;
	g.button[g.nbuttons].r.top = g.button[g.nbuttons].r.bot +
	g.button[g.nbuttons].r.height;
	g.button[g.nbuttons].r.centerx = (g.button[g.nbuttons].r.left +
	g.button[g.nbuttons].r.right) / 2;
	g.button[g.nbuttons].r.centery = (g.button[g.nbuttons].r.bot +
	g.button[g.nbuttons].r.top) / 2;
	strcpy(g.button[g.nbuttons].text, "Quit");
	g.button[g.nbuttons].down = 0;
	g.button[g.nbuttons].click = 0;
	g.button[g.nbuttons].color[0] = 0.3f;
	g.button[g.nbuttons].color[1] = 0.3f;
	g.button[g.nbuttons].color[2] = 0.6f;
	g.button[g.nbuttons].dcolor[0] = g.button[g.nbuttons].color[0] * 0.5f;
	g.button[g.nbuttons].dcolor[1] = g.button[g.nbuttons].color[1] * 0.5f;
	g.button[g.nbuttons].dcolor[2] = g.button[g.nbuttons].color[2] * 0.5f;
	g.button[g.nbuttons].text_color = 0x00ffffff;
	g.nbuttons++;
	*/
}

void resetGame()
{
	g.player.init();
	g.state[S_GAMEOVER] = 0;
	g.state[S_WINNER]	= 0;
}

void gameUpdate()
{
	for(int i=0; i<g.number[N_ENEMIES]; i++){
		if (g.enemies[i].state == S_CHAR_DEAD) {
			g.enemies[i]=g.enemies[--g.number[N_ENEMIES]];
		}
	}
	/*if(g.number[N_ENEMIES]<5){
		spawnEnemy(RND()*(g.xres), RND()*(g.yres));
	}*/
}

int checkKeys(XEvent *e)
{
	if (e->type != KeyRelease && e->type != KeyPress)
		return 0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);

	switch (key) {
		case XK_Shift_L:
		case XK_Shift_R:
			if (e->type == KeyPress)
				g.isPressed[K_SHIFT]=1;
			else
				g.isPressed[K_SHIFT]=0;
			break;
		case XK_Escape:
			return 1;
		case XK_r:
			resetGame();
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
		case XK_a:
			if (e->type == KeyPress)
				g.isPressed[K_A]=1;
			else
				g.isPressed[K_A]=0;
			break;
		case XK_d:
			if (e->type == KeyPress)
				g.isPressed[K_D]=1;
			else
				g.isPressed[K_D]=0;
			break;
		case XK_w:
			if (e->type == KeyPress)
				g.isPressed[K_W]=1;
			else
				g.isPressed[K_W]=0;
			break;
		case XK_s:
			if (e->type == KeyPress)
				g.isPressed[K_S]=1;
			else
				g.isPressed[K_S]=0;
			break;
		case XK_e:
			if (e->type == KeyPress)
                interactDoor();
			break;
		case XK_p:
			if (e->type == KeyPress)
				g.state[S_PAUSED] ^= 1;
			break;
		case XK_o:
			if (e->type == KeyPress)
				g.state[S_DEBUG] ^= 1;
			break;
        case XK_z:
			if (e->type == KeyPress) {
                if (g.state[S_LEVELEDIT]) {
                    createWall(g.savex, g.savey);
                }
            }
            break;
        case XK_x:
            if (e->type == KeyPress) {
                if (g.state[S_LEVELEDIT]) {
                    createDoor(g.savex, g.savey);
                }
            }
            break;
        case XK_b: 
            if (e->type == KeyPress) {
                if (g.state[S_LEVELEDIT]) {
                    saveLevel();
                }
            }
            break;
        case XK_f:
            if (e->type == KeyPress) {
                if (g.state[S_LEVELEDIT]) {
                    loadLevel();
                }
            }
            break;
        case XK_l:
            if (e->type == KeyPress)
    			toggleEditMode();
            break;
		case XK_1:
			david_func();
			break;
		case XK_2:
			taylor_func();
			break;
		case XK_3:
			mason_func();
			break;
		case XK_4:
		    spawnEnemy(RND()*(g.xres), RND()*(g.yres));
			break;
	}
	return 0;
}

int checkMouse(XEvent *e)
{
	int x,y;
	int lbutton=0;
	int rbutton=0;
	//
	if (e->type == ButtonRelease) {
        g.isClicked[M_1] = false;
        g.wallChange = true;
        g.isClicked[M_2] = false;
        g.doorChange = true;
        return 0;
    }
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
            g.isClicked[M_1] = true;
			(void)lbutton;
			if(g.player.anim_handler==NULL){
				g.player.setVel(0,0);
				Animation *act = &g.anims[g.number[N_ANIMS]++];
				act->init(A_SWORD_SLASH);
				act->add_actor(&g.player);
			} else if (g.player.anim_handler->type == A_SWORD_SLASH
				&& g.player.anim_handler->can_cancel) {
				Animation *act = g.player.anim_handler;
				act->cancel();
				act->init(A_SWORD_SLASH2);
				act->add_actor(&g.player);
			}
		}
		if (e->xbutton.button==3) {
			//Right button is down
			rbutton=1;
            g.isClicked[M_2] = true;
            if (g.state[S_LEVELEDIT])
               rotateDoor(g.savex, g.savey); 
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = g.yres - y;
    if (g.isClicked[M_1] == true) {
        dragWall(x, y);
        dragDoor(x, y);
    }
	//printf("%d %d\n",x,y);
	if (g.savex != e->xbutton.x || g.savey != e->xbutton.y) {
		//Mouse moved
		g.savex = x;
		g.savey = y;
	}
	//for menu buttons
	/*for (i=0; i<g.nbuttons; i++) {
		g.button[i].over=0;
		if (x >= g.button[i].r.left &&
				x <= g.button[i].r.right &&
				y >= g.button[i].r.bot &&
				y <= g.button[i].r.top) {
			g.button[i].over=1;
			if (g.button[i].over) {
				if (lbutton) {
					switch (i) {
						case 0:
							resetGame();
							break;
						case 1:
							printf("Quit was clicked!\n");
							return 1;
					}
				}
			}
		}
	}*/
	return 0;
}


void animation(){
	int i=0;
	while ( i<g.number[N_ANIMS] ) {
		g.anims[i].play();
		if(g.anims[i].done){
			//g.anims[i].clear();
			g.anims[i]=g.anims[--g.number[N_ANIMS]];
		} else {
			i++;
		}
	}
}

void physics()
{
	if (g.state[S_GAMEOVER]) {
		std::cout<<"GAME OVER\n";
		return;
	}

    Player *p = &g.player;
	if (g.isPressed[K_W]) {
		p->addVel(0,1);
	} else if (g.isPressed[K_S]) {
		p->addVel(0,-1);
	} else if (ABS(g.player.vel[1]) > 0.1) {
		//keep player from maintaining velocity
		p->setVel(p->vel[0],p->vel[1]*0.9);
	} else {
		p->setVel(p->vel[0],0);
	}

	if (g.isPressed[K_D]) {
		p->addVel(1,0);
	} else if (g.isPressed[K_A]) {
		p->addVel(-1,0);
	} else if (ABS(g.player.vel[0]) > 0.1) {
		//keep player from maintaining velocity
		p->setVel(p->vel[0]*0.9,p->vel[1]);
	} else {
		p->setVel(0,p->vel[1]);
	}
	
	// if not attacking
	if(g.anims[0].can_cancel || g.anims[0].done){
		//update player position
		p->move();
		//look at last known mouse pos
		g.player.lookAt(g.savex,g.savey);
	}

	for(int i=0; i<g.number[N_ENEMIES]; i++){
		g.enemies[i].attackPlayer();
		g.enemies[i].move();
	}


    //player collision
    for (int i=0; i<100; i++) {
        for (int j=0; j<g.number[N_ENEMIES]; j++) {
            wallCollision(g.level1.doors[i], g.enemies[j]);
            wallCollision(g.level1.walls[i], g.enemies[j]);
        }
        
        wallCollision(g.level1.walls[i], g.player);
        wallCollision(g.level1.doors[i], g.player);
    }
    
	for (int i=0; i<g.number[N_ENEMIES]; i++) {
		for (int j=0; j<g.number[N_ENEMIES]; j++) {
			if (i!=j)
	        	characterCollision(g.enemies[i], g.enemies[j]);
        }
        characterCollision(g.enemies[i], g.player);
    }
	
	for (int i=0; i<g.player.nattacks; i++){
		for(int j=0; j<g.number[N_ENEMIES]; j++){
			if(g.player.attacks[i].intersect(g.enemies[j].hitbox)){
				g.enemies[j].kill();
			}
		}
	}
}


void render(void)
{
	//--------------------------------------------------------
	//This code is repeated several times in this program, so
	//it can be made more generic and cleaner with some work.
	//center of a grid
	//bq is the width of one grid section
	//--------------------------------------------------------
	//start the opengl stuff
	//set the viewing area on screen
	glViewport(0, 0, g.xres-1, g.yres-1);
	//clear color buffer
	glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	
	//init matrices
	glMatrixMode (GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//this sets to 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);


	//
	//screen background
	glColor3f(0.7f, 0.7f, 0.7f);
	glBindTexture(GL_TEXTURE_2D, g.bgTexture);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.0f, 0.0f); glVertex3i(0,      g.yres,0);
	glTexCoord2f(1.0f, 0.0f); glVertex3i(g.xres, g.yres,0);
	glTexCoord2f(1.0f, 1.0f); glVertex3i(g.xres, 0,     0);
	glTexCoord2f(0.0f, 1.0f); glVertex3i(0,      0,     0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	
	
	//draw character
	g.player.draw();
	
	
    for(int i=0; i<g.number[N_ENEMIES]; i++){
		g.enemies[i].draw();
	}

	for(int loop = 0; loop < 3; loop++) {
		//g.menuButt[ loop ].draw();
	}
	ggprint16(&g.title.r, 0, g.title.text_color, g.title.text);
    
    //draw level objects
    for (int i=0; i<100; i++) {
        g.level1.walls[i].draw();
        g.level1.doors[i].draw();
    }
	
    if (g.state[S_DEBUG])
		g.info.draw();
	if (g.state[S_PAUSED])
		pauseMenu();

	
	//glDisable(GL_DEPTH_TEST);
	

}

