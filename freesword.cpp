#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <iostream>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>

#include "log.h"
#include "fonts.h"
#include "global.h"

#define USE_OPENAL_SOUND	
#ifdef __MACH__
	#include <mach/clock.h>
	#include <mach/mach.h>
	#include <mach/mach_init.h>
	#include <mach/mach_time.h>
	#include <AL/alut.h>
#else
	#ifdef USE_OPENAL_SOUND
		#include </usr/include/AL/alut.h>
	#endif //USE_OPENAL_SOUND
#endif

//macros

#define DIRECTION_DOWN  0
#define DIRECTION_LEFT  1
#define DIRECTION_UP	2
#define DIRECTION_RIGHT 3
#define DIRECTION_W	 0
#define DIRECTION_A	 1
#define DIRECTION_S	 2
#define DIRECTION_D	 3
//player shape radius
#define PRADIUS 25
//

Global g;
Image img[1] = {"./images/marble.gif" };


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
			XStoreName(dpy, win, "snake");
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
void physics(void);
void render(void);
void getGridCenter(const int i, const int j, int cent[2]);
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
//const double oobillion = 1.0 / 1e9;
//struct timespec timeStart, timeCurrent;
//struct timespec timePause;
double timeStart, timeCurrent;
double timePause;
double physicsCountdown = 0.0;
double timeSpan = 0.0;
/*double timeDiff(struct timespec *start, struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec ) +
			(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}
*/
//-----------------------------------------------------------------------------

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
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			done = checkMouse(&e);
			done = checkKeys(&e);
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
			physics();
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

#ifdef USE_OPENAL_SOUND
void playSound(ALuint source)
{
	alSourcePlay(source);	
}
#endif //USE_OPENAL_SOUND

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
	//g.marbleImage = ppm6GetImage("./images/marble.ppm");
	g.marbleImage = &img[1];
	//
	//create opengl texture elements
	glGenTextures(1, &g.marbleTexture);
	glBindTexture(GL_TEXTURE_2D, g.marbleTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
			g.marbleImage->width, g.marbleImage->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, g.marbleImage->data);
}

void initPlayer()
{
	g.player.status = 0;
	g.player.dir = 0;
	g.player.orientation[0] = 0;
	g.player.orientation[1] = 1;
	g.player.pointer[0] = 75;
	g.player.pointer[1] = 75;
	g.player.pos[0] = 50;
	g.player.pos[1] = 50;
	g.player.vel[0] = 0;
	g.player.vel[1] = 1;
}

void init()
{
	g.boardDim = g.gridDim * 10;
	//
	initPlayer();
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
	initPlayer();
	g.gameover  = 0;
	g.winner	= 0;
}

int checkKeys(XEvent *e)
{
	static int shift=0;
	if (e->type != KeyRelease && e->type != KeyPress)
		return 0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//if (e->type == KeyPress)
		//mason_func();
	if (e->type == KeyRelease) {
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift=1;
		return 0;
	}
	(void)shift;
	switch (key) {
		case XK_r:
			resetGame();
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
		case XK_a:
			g.player.dir = DIRECTION_A;
			movePlayer(&g.player);
			break;
		case XK_d:
			g.player.dir = DIRECTION_D;
			movePlayer(&g.player);
			break;
		case XK_w:
			g.player.dir = DIRECTION_W;
			movePlayer(&g.player);
			break;
		case XK_s:
			g.player.dir = DIRECTION_S;
			movePlayer(&g.player);
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
			jacob_func();
			break;
	}
	return 0;
}

int checkMouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	int x,y;
	int lbutton=0;
	int rbutton=0;
	//
	if (e->type == ButtonRelease)
		return 0;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
			rbutton=1;
			if (rbutton){}
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = g.yres - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}

	if (savex > (g.player.pos[0]+10)) {
		g.player.orientation[0] = 1;
	} else if (savex < (g.player.pos[0]-10)) {
		g.player.orientation[0] = -1;
	} else {
		g.player.orientation[0] = 0;
	}
	if (savey < (g.yres - g.player.pos[1] - 10)) {
		g.player.orientation[1] = 1;
	} else if (savey > (g.yres - g.player.pos[1] + 10)) {
		g.player.orientation[1] = -1;
	} else {
		g.player.orientation[1] = 0;
	}

	setPlayerOrientation(&g.player);

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



void getGridCenter(const int i, const int j, int cent[2])
{
	//This function can be optimized, and made more generic.
	int b2 = g.boardDim/2;
	int screenCenter[2] = {g.xres/2, g.yres/2};
	int s0 = screenCenter[0];
	int s1 = screenCenter[1];
	int bq;
	//quad upper-left corner
	int quad[2];
	//bq is the width of one grid section
	bq = (g.boardDim / g.gridDim);
	//-------------------------------------
	//because y dimension is bottom-to-top in OpenGL.
	int i1 = g.gridDim - i - 1;
	quad[0] = s0-b2;
	quad[1] = s1-b2;
	cent[0] = quad[0] + bq/2;
	cent[1] = quad[1] + bq/2;
	cent[0] += (bq * j);
	cent[1] += (bq * i1);
}


void physics()
{
	if (g.gameover) {
		std::cout<<"GAME OVER\n";
		return;
	}
	//
	//
	//Is it time to move the snake?
	/*static struct timespec snakeTime;
	  static int firsttime=1;
	  if (firsttime) {
	  firsttime=0;
	  clock_gettime(CLOCK_REALTIME, &snakeTime);
	  }
	  struct timespec tt;
	  clock_gettime(CLOCK_REALTIME, &tt);
	  timeCopy(&snakeTime, &tt);
	  */
	//update player position
	if (g.player.vel[0] < 100) {
		g.player.pos[0] += g.player.vel[0];
		g.player.pointer[0] += g.player.vel[0];
	}
	
	if (g.player.vel[1] < 100) {
		g.player.pos[1] += g.player.vel[1];
		g.player.pointer[1] += g.player.vel[1];
	}

	//keep player from maintaining velocity
	if (g.player.vel[0] > 0) {
		g.player.vel[0] -= 0.01;
	} else if (g.player.vel[0] < 0) {
		g.player.vel[0] += 0.01;
	}

	if (g.player.vel[1] > 0) {
		g.player.vel[1] -= 0.01;
	} else if (g.player.vel[1] < 0) {
		g.player.vel[1] += 0.01;
	}

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
}


void render(void)
{
	//Rect r;
	//--------------------------------------------------------
	//This code is repeated several times in this program, so
	//it can be made more generic and cleaner with some work.
	//center of a grid
	//bq is the width of one grid section
	//--------------------------------------------------------
	//start the opengl stuff
	//set the viewing area on screen
	glViewport(0, 0, g.xres, g.yres);
	//clear color buffer
	glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//init matrices
	glMatrixMode (GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//this sets to 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);

	
	
	//
	//screen background
	/*glColor3f(0.5f, 0.5f, 0.5f);
	  glBindTexture(GL_TEXTURE_2D, g.marbleTexture);
	  glBegin(GL_QUADS);
	  glTexCoord2f(0.0f, 0.0f); glVertex2i(0,	  0);
	  glTexCoord2f(0.0f, 1.0f); glVertex2i(0,	  g.yres);
	  glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, g.yres);
	  glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, 0);
	  glEnd();
	  glBindTexture(GL_TEXTURE_2D, 0);
	  */
	/*
	//draw the main game board in middle of screen
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2i(s0-b2, s1-b2);
	glVertex2i(s0-b2, s1+b2);
	glVertex2i(s0+b2, s1+b2);
	glVertex2i(s0+b2, s1-b2);
	glEnd();
	// */
	/*//grid lines...
	  int x0 = s0-b2;
	  int x1 = s0+b2;
	  int y0 = s1-b2;
	  int y1 = s1+b2;
	  glColor3f(0.1f, 0.1f, 0.1f);
	  glBegin(GL_LINES);
	  for (i=1; i<g.gridDim; i++) {
	  y0 += 10;
	  glVertex2i(x0,y0);
	  glVertex2i(x1,y0);
	  }
	  x0 = s0-b2;
	  y0 = s1-b2;
	  y1 = s1+b2;
	  for (j=1; j<g.gridDim; j++) {
	  x0 += 10;
	  glVertex2i(x0,y0);
	  glVertex2i(x0,y1);
	  }
	  glEnd();
	  */ 
	//
	//draw player
	glColor3f(0.5f, 0.0f, 0.5f);
	glPushMatrix();
	//	glTranslatef(g.player.pos[0], g.player.pos[1], 0.0);

	//starting coordinates
	float cx = g.player.pos[0];
	float cy = g.player.pos[1];
	//radius
	float ra = PRADIUS;
	int num_segments = 100;

	float theta = 2 * 3.1415926 / float(num_segments); 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = ra;//we start at angle = 0 
	float y = 0; 

	glBegin(GL_POLYGON); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 

		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd();  

	//draw player pointer
	glTranslatef(g.player.pointer[0], g.player.pointer[1], 0.0);
	glColor3f(0.5f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(-5.0f, 0.0f);
	glVertex2f(0.0f, 5.0f);
	glVertex2f(5.0f, 0.0f);
	glEnd();
	glPopMatrix();
	
	ggprint16(&g.title.r, 0, g.title.text_color, g.title.text);

}

