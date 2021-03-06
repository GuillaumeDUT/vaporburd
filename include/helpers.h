#ifndef HELPERS_H
#define HELPERS_H

float absFloat(float n);
int absInt(int n);

// On suppose a<b
int rand_a_b(int a, int b);
double frand_a_b(double a, double b);
float fmap(float value, float istart, float istop, float ostart, float ostop);

float floatMax(float a, float b);
int intMax(int a, int b);
float floatMin(float a, float b);
int intMin(int a, int b);

int noise2(int x, int y);
float lin_inter(float x, float y, float s);
float smooth_inter(float x, float y, float s);
float noise2d(float x, float y);
float perlin2d(float x, float y, float freq, int depth);

#endif
