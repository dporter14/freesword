#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "freesword.h"
#include "fonts.h"

#ifndef USE_OPENAL_SOUND
#define USE_OPENAL_SOUND	
#endif

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

#define MAXBUTTONS 4
typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
} Button;

class Image {
	public:
		int width, height;
		unsigned char *data;
		~Image() { delete [] data; }
		Image(const char *fname) {
			if (fname[0] == '\0')
				return;
			//printf("fname **%s**\n", fname);
			int ppmFlag = 0;
			char name[40];
			strcpy(name, fname);
			int slen = strlen(name);
			char ppmname[80];
			if (strncmp(name+(slen-4), ".ppm", 4) == 0)
				ppmFlag = 1;
			if (ppmFlag) {
				strcpy(ppmname, name);
			} else {
				name[slen-4] = '\0';
				//printf("name **%s**\n", name);
				sprintf(ppmname,"%s.ppm", name);
				//printf("ppmname **%s**\n", ppmname);
				char ts[100];
				//system("convert img.jpg img.ppm");
				sprintf(ts, "convert %s %s", fname, ppmname);
				system(ts);
			}
			//sprintf(ts, "%s", name);
			FILE *fpi = fopen(ppmname, "r");
			if (fpi) {
				char line[200];
				fgets(line, 200, fpi);
				fgets(line, 200, fpi);
				while (line[0] == '#')
					fgets(line, 200, fpi);
				sscanf(line, "%i %i", &width, &height);
				fgets(line, 200, fpi);
				//get pixel data
				int n = width * height * 3;			
				data = new unsigned char[n];			
				for (int i=0; i<n; i++)
					data[i] = fgetc(fpi);
				fclose(fpi);
			} else {
				printf("ERROR opening image: %s\n",ppmname);
				exit(0);
			}
			if (!ppmFlag)
				unlink(ppmname);
		}
};

struct Global {
	int xres, yres;
	Player player;
	int gridDim;
	int boardDim;
	int gameover;
	int winner;
	Image *marbleImage;
	GLuint marbleTexture;
	Button button[MAXBUTTONS];
	int nbuttons;
	Button title;
	//
	ALuint alBufferDrip, alBufferTick;
	ALuint alSourceDrip, alSourceTick;
	Global() {
		xres = 800;
		yres = 600;
		gridDim = 40;
		gameover = 0;
		winner = 0;
		nbuttons = 0;
		marbleImage=NULL;
		title.r.left   = xres/2;
		title.r.bot	= yres-100;
		title.r.center = 1;
		strcpy(title.text,"Freesword");
		title.text_color = 0x00ffffff;
	}
};


#endif