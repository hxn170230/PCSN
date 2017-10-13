#include <stdio.h>
#include <math.h>
#include <limits.h>

double getXRandom(double m, double k, double *s) {
	(*s) = fmod(k*(*s), m);
	double r = (*s)/m;
	return r;
}

int main() {
	double m = 2147483647;
        double k = 16807;
        double s = 1111;
        double r1 = 0;
	int i = 0;
	int rzero = 0;
	int rone = 0;
	int szero = 0;
	int sone = 0;
	int zeroincorrect = 0;
	int zerocorrect = 0;
	int oneincorrect = 0;
	int onecorrect = 0;
	int error = 0;
	for (i = 0; i < 10000; i++) {
		int bit = 0;
		r1 = getXRandom(m,k,&s);
		if (r1 < 0.45) {
			bit = 0;
		} else {
			bit = 1;
		}

		double r2 = getXRandom(m,k,&s);
		int rbit = 0;
		if (bit == 0) {
			if (r2 < 0.15) {
				rbit = 1;
				oneincorrect+=1;
			} else {
				rbit = 0;
				zerocorrect += 1;
			}
		} else {
			if (r2 < 0.05) {
				rbit = 0;
				zeroincorrect += 1;
			} else {
				rbit = 1;
				onecorrect += 1;
			}
		}
		szero += (bit == 0)?1:0;
		sone += (bit == 1)?1:0;
		rzero += (rbit == 0)?1:0;
		rone  += (rbit == 1)?1:0;
		error += (bit == rbit)?0:1;
	}
	printf("RZ: %d RO: %d SZ: %d SO: %d error: %d\n", rzero, rone, szero, sone, error);
	printf("RZEROCORRECT: %d RZEROINCORRECT: %d RONECORRECT: %d RONEINCORRECT: %d\n", zerocorrect, zeroincorrect, onecorrect, oneincorrect);
	printf("P(0 received): %lf\n", (double)rzero/(double)10000);
	printf("P(1 was transmitted| 1 was received): %lf\n", (double)onecorrect/(double)rone);
	printf("P(error): %lf\n", (double)error/(double)10000);
	return 1;
}
