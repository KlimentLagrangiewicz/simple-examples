
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

long long TimeValue = 0;

unsigned long long time_RDTSC() {
	union ticks {
		unsigned long long tx;
		struct dblword {
			long tl, th;
		} dw;
	} t;
	__asm__ ("rdtsc\n"
	  : "=a" (t.dw.tl), "=d"(t.dw.th)
	  );
	return t.tx;
} /* for x86 only! */

void time_start() { TimeValue = time_RDTSC(); }

long long time_stop() { return time_RDTSC() - TimeValue; }

double dist1(double *x, double *y, int m, int l, int k) {
	double d, r = 0;
	int i;
	for (i = 0; i < m; i++) {
		d = x[l + i] - y[k + i];
		r += d * d;
	}
	return r;
}

double dist2(double *x, double *y, int m) {
	double d, r = 0;
	size_t i = 0;
	while (i++ < m) {
		d = *(x++) - *(y++);
		r += d * d;
	}
	return r;
}

void vec_upload(double *x, int n) {
	double d;
	int i;
	for (i = 0; i < n; i++) {
		d = (double)(rand() % 500);
		x[i] = d + d / 600;
	}
}

int main(int argc, char **argv) {
	if (argc < 5) {
		printf("Insufficient number of parameters!\n");
		exit(1);
	}
	int n = atoi(argv[1]), m = atoi(argv[2]), k = atoi(argv[3]), l = atoi(argv[4]);
	if ((n < 0) || (m < 0) || (k < 0) || (l < 0) || (k + m >= n) || (l + m >= n)) {
		printf("Value of parameters is not correctly!\n");
		exit(1);
	}
	double *x = (double*)malloc(n * sizeof(double));
	vec_upload(x, n);
	double *y = (double*)malloc(n * sizeof(double));
	vec_upload(y, n);
	double r1, r2;
    long long l1, l2;
	time_start();
	r1 = dist1(x, y, m, k, l);
	l1 = time_stop();
	time_start();
	r2 = dist2(&x[k], &y[l], m);
	l2 = time_stop();
	if (r1 == r2) {
		printf("First method: %lld number of processor clock cycles;\n", l1);
		printf("Second method: %lld number of processor clock cycles;\n", l2);
	}
	printf("The work of the program is completed!\n");
	free(x);
	free(y);
	return 0;
}
