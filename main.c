#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define RAND(a) (rand() % (a - 1))
#define LEN(a) (sizeof(a) / sizeof(a[0]))
#define MAX(a, b) ( (a > b) ? a : b )

float *gen(int, int, int);
float calc_median(float*);
float calc_mode(float*);
void calc_stat(float*, float*, float*, int, int, int);
float *crt_a2(float*, int, int, int);
float *crt_a3(float*, int, int, int);
void show_hist(float*, int, int);

int
main()
{
	srand(time(NULL));
	int h = 10;
	printf("Равномерное распределение\n");
	int b = 1;
	for (int r = 10; r <= 1000; r *= 10) {
		for (int n = 10000; n <= 100000; n *= 10) {
			printf("r = %d; n = %d\n", r, n);
			float *a = gen(n, b, r);
			for (int i = 1; i < LEN(a); i++) {
				if (a[i] < a[i-1]) {
					float tmp = a[i];
          a[i] = a[i-1];
          a[i-1] = tmp;
          i = 1;
				}
			}
			float *a2 = crt_a2(a, h, r, n);
			float *a3 = crt_a3(a, h, r, n);
			calc_stat(a, a2, a3, n, h ,r);
			show_hist(a2, h, n);
		}
	}

	printf("Нормальное распределение\n");
	b = 0;
	for (int r = 10; r <= 1000; r *= 10) {
		for (int n = 10000; n <= 100000; n *= 10) {
			printf("r = %d; n = %d\n", r, n);
			float *a = gen(n, b, r);
			float *a2 = crt_a2(a, h, r, n);
      float *a3 = crt_a3(a, h, r, n);
			calc_stat(a, a2, a3, n, h ,r);
			show_hist(a2, h, n);
		}
	}
}

float
*gen(int n, int b, int r)
{
	float *a;
	a = malloc(sizeof(float*)*n);
	for (int i = 0; i < n; i++) {
		float num = 0;
		if (b) {
			num = RAND(r);
		} else {
			for (int j = 0; j < 5; j++) {
				num += RAND(r);
				num /= 5.0;
			}
		}
		a[i] = num;
	}
	return a;
}

float
calc_median(float *a)
{
	int n = LEN(a);
	float median;
	for (int i = 1; i < n; i++) {
		if (a[i] < a[i-1]) {
			float tmp = a[i];
			a[i] = a[i-1];
			a[i-1] = tmp;
			i = 1;
		}
	}
	if (n % 2) {
		median = a[n / 2];
	} else {
		int i = n / 2;
		median = (a[i] + a[i+1]) / 2;
	}
	return median;
}

float
calc_mode(float *a3)
{
	int max_count = 0, mode = 0;
	for (int i = 0; i < LEN(a3); i++) {
		max_count = MAX(max_count, a3[i]);
		mode = i;
	}
	return mode;
}

void
calc_stat(float *a, float *a2, float *a3, int n, int h, int r)
{
	float sr = 0, sr_kv, tmp = 0;
	for (int i = 0; i < LEN(a); i++) sr += a[i];
	for (int x = 0; x < LEN(a); x++) tmp += pow( (sr - x), 2);
	sr_kv = sqrt(tmp/n);
	printf("\nСреднее:%g\n", sr);
	printf("Мода:   %g\n", calc_mode(a3));
	printf("Медиана:%g\n", calc_median(a));
	printf("Ср. кв.:%g\n", sr_kv);
}

float
*crt_a2(float* a, int h, int r, int n)
{
	float *a2;
	memset(a2, 0, (h+1)*sizeof(float));
	int y = 0;
	float s = (float)r / h;
	
	for (int i = 0; i <= h; i++) {
		for (int j = 0; j < n; j++) {
			if (y <= ( a[j] < y) + s) {
				a2[i] += 1;
				y += s;
			}
		}
	}
	return a2;
}

float
*crt_a3(float* a, int h, int r, int n)
{
	float *a3;
	memset(a3, 0, (h+1)*sizeof(float));
	int y = 0;
	float s = (float)r / h;
	
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < n; j++) {
			if (a[j] == i) a3[i] += 1;
		}
	}
	return a3;
}

void
show_hist(float *a2, int h, int n)
{
	printf("Гистограмма:\n");
	n = n / 100;
	for (int i = 0; i < LEN(a2); i++) {
		if (a2[i] < n) {
			printf(".");
		} else {
		  for (int j = 0; j < (a2[i]/2); j++) {
		    printf("*");
		  }
		}
		printf(" [%g]\n", a2[i]);
	}
}

