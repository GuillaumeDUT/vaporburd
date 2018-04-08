#include "libs.h"

#include "helpers.h"

float absFloat(float n) {
	return n < 0 ? -n : n;
}
int absInt(int n) {
	return n < 0 ? -n : n;
}

// On suppose a<b
int rand_a_b(int a, int b) {
    return rand()%(b-a) +a;
}
double frand_a_b(double a, double b) {
    return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}
float fmap(float value, float istart, float istop, float ostart, float ostop) {
  return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}