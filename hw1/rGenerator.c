#include <stdio.h>
#include <limits.h>
#include <math.h>

int findRange(double r, int *range) {
	int i = 0;
	for (i = 1; i < 11; i++) {
		double ra = ((double)i)/((double)10);
		if (r < ra) return i-1;
	}
	return -1;
}

int main() {
	double m = 2147483647;
	double k = 16807;
	double s = 1111;
	double r = 0;
	double umean = 0;
	double uvar = 0;
	double expMean = 0;
	double expVar = 0;

	int range[10] = {0,};

	int i = 0;
	for (i = 0; i < 10000; i++) {
		s = fmod(k*s, m);
		r = s/m;
		printf("random number: %lf\n", r);
		umean+=r;
		uvar+=r*r;
		double expY = (-0.5)*(log(r));
		expMean+=expY;
		expVar+=expY*expY;
		int j = findRange(r, range);
		if (j >= 0) {
			range[j] += 1;
		}
	}

	printf("Mean(Uniform): %lf\n", umean/10000);
	printf("Variance(Uniform): %lf\n", (uvar/10000 - ((umean/10000)*(umean/10000))));

	printf("Mean(Exponential): %lf\n", expMean/10000);
	printf("Variance(Exponential): %lf\n", (expVar/10000 - ((expMean/10000)*(expMean/10000))));
	for (i = 0; i < 10; i++) {
		float range1 = (float)i/10;
		float range2 = ((float)i+1)/10;
		printf("Range[%f, %f]: %d\n", range1, range2, range[i]);
	}

	return 0;
}
