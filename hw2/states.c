#include <stdio.h>

int main() {
	double initStates[3] = {0,1,0};
	double initP[3][3] = {{0.67,0.33,0},{0.25,0.5,0.25},{0,0.5,0.5}};
	double total =0;
	printf("init: %lf %lf\n", initP[0][0], initP[1][0]);
	int i = 0;
	for (i = 0; i < 2; i++) {
		double init1[3] = {0,0,0};
		init1[0] = initStates[0]*initP[0][0] +
				initStates[1]*initP[1][0] +
				initStates[2]*initP[2][0];
		init1[1] = initStates[0]*initP[0][1] +
				initStates[1]*initP[1][1] +
				initStates[2]*initP[2][1];
		init1[2] = initStates[0]*initP[0][2] +
				initStates[1]*initP[1][2] +
				initStates[2]*initP[2][2];
		initStates[0] = init1[0];
		initStates[1] = init1[1];
		initStates[2] = init1[2];
		total += initStates[2];
		printf("[avg]changed to [%lf %lf %lf]\n", initStates[0], initStates[1],initStates[2]);
	}
	initStates[0] = 1;
	initStates[1] = 0;
	initStates[2] = 0;
	total = 0;
	for (i = 0; i < 1; i++) {
		double init1[3] = {0,0,0};
		init1[0] = initStates[0]*initP[0][0] +
				initStates[1]*initP[1][0] +
				initStates[2]*initP[2][0];
		init1[1] = initStates[0]*initP[0][1] +
				initStates[1]*initP[1][1] +
				initStates[2]*initP[2][1];
		init1[2] = initStates[0]*initP[0][2] +
				initStates[1]*initP[1][2] +
				initStates[2]*initP[2][2];
		initStates[0] = init1[0];
		initStates[1] = init1[1];
		initStates[2] = init1[2];
		total += initStates[2];
	}
	printf("[weak]changed to [%lf %lf %lf]\n", initStates[0], initStates[1],initStates[2]);
	initStates[0] = 0;
	initStates[1] = 1;
	initStates[2] = 0;
	total = 0;
	for (i = 0; i < 1; i++) {
		double init1[3] = {0,0,0};
		init1[0] = initStates[0]*initP[0][0] +
				initStates[1]*initP[1][0] +
				initStates[2]*initP[2][0];
		init1[1] = initStates[0]*initP[0][1] +
				initStates[1]*initP[1][1] +
				initStates[2]*initP[2][1];
		init1[2] = initStates[0]*initP[0][2] +
				initStates[1]*initP[1][2] +
				initStates[2]*initP[2][2];
		initStates[0] = init1[0];
		initStates[1] = init1[1];
		initStates[2] = init1[2];
		total += initStates[2];
	}
	printf("[avg]changed to [%lf %lf %lf]\n", initStates[0], initStates[1],initStates[2]);

	return 0;
}
