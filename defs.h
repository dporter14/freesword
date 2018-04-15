#ifndef _DEFS_H
#define _DEFS_H

typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

#define VecMake(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecNegate(a)	(a)[0]=(-(a)[0]); (a)[1]=(-(a)[1]); (a)[2]=(-(a)[2]);
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecLen(a)	sqrt((a)[0]*(a)[0]+(a)[1]*(a)[1]+(a)[2]*(a)[2])
#define VecLen2(a)	((a)[0]*(a)[0]+(a)[1]*(a)[1]+(a)[2]*(a)[2])
#define VecCopy(a,b)	(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];
#define VecAdd(a,b,c) (c)[0]=(a)[0]+(b)[0]; (c)[1]=(a)[1]+(b)[1]; (c)[2]=(a)[2]+(b)[2]
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; (c)[1]=(a)[1]-(b)[1]; (c)[2]=(a)[2]-(b)[2]
#define VecS(A,a,b) (b)[0]=(A)*(a)[0]; (b)[1]=(A)*(a)[1]; (b)[2]=(A)*(a)[2]
#define VecAddS(A,a,b,c) (c)[0]=(A)*(a)[0]+(b)[0]; (c)[1]=(A)*(a)[1]+(b)[1]; (c)[2]=(A)*(a)[2]+(b)[2]
#define VecCross(a,b,c) (c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1]; (c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2]; (c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]
#define VecZero(v) (v)[0]=0.0;(v)[1]=0.0;v[2]=0.0
#define ABS(a) (((a)<0)?(-(a)):(a))
#define SGN(a) (((a)<0)?(-1):(1))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define SWP(a,b) (a)^=(b);(b)^=(a);(a)^=(b);
#define SGND(a) (((a)<0.0)?(-1.0):(1.0))
#define CLAMP(a,b,c) ((a)<(b)?(b):((a)>(c)?(c):(a)))
#define RND() (double)rand()/(double)RAND_MAX
#define PI 3.1415926535897932

/*typedef struct t_rect {
	int left;
	int top;
	int right;
	int bot;
	int width;
	int height;
	int center;
	int centerx;
	int centery;
} Rect;*/

typedef struct Mouse {
	int x,y;
	int lastx,lasty;
	int lbuttondown;
} Mouse;

typedef struct Screen2 {
	int x_res, y_res;
	int screen_x_res, screen_y_res;
	float gl_perspective_viewangle;
	Flt xcenter, ycenter;
	Flt fy_res, fx_res;
} Screen2;

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
				//printf("%d %d\n",width, height);
				fgets(line, 200, fpi);
				//get pixel data
				int n = width * height * 4;
				data = new unsigned char[n];
				for (int i=0; i<n; i++){
					if (!((i+1)&3)) {
						if (data[i-3]>=253 && data[i-2]<=2 && data[i-1]>=253) {
							data[i] = 0;
						} else {
							data[i] = 255;
						}
					} else {
						data[i] = fgetc(fpi);
					}
					//if (i%3==2)
					//	printf("(%d,%d): %d %d %d\n",(int)floor(i/3)%width,
					//(int)floor((i/3)/width),data[i-2],data[i-1],data[i]);
				}
				fclose(fpi);
			} else {
				printf("ERROR opening image: %s\n",ppmname);
				exit(0);
			}
			if (!ppmFlag)
				unlink(ppmname);
				
		}
};


#endif

