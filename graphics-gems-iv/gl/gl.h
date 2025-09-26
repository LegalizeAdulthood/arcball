#pragma once

#define FALSE 0
#define TRUE 1

void keepaspect(int, int);
void prefposition(int, int, int, int);
int winopen(const char *);
void doublebuffer();
void RGBmode(void);
void gconfig();
void ortho(double, double, double, double, double, double);
void translate(double, double, double);
void getsize(long *, long *);
void getorigin(long *, long *);
void gexit();
void reshapeviewport();
void swapbuffers();
void pushmatrix();
void loadmatrix(const float *);
void ortho2(double, double, double, double);
void scale(double, double, double);
void circ(double, double, double);
void popmatrix();
void bgnline();
void endline();
void v3f(const float *);
void RGBcolor(double, double, double);
typedef float *Matrix;
void bgnpolygon();
void v4f(const float *);
void endpolygon();
void clear();
void multmatrix(const float *);
