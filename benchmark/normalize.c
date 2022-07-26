#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define COUNT 200000000

typedef struct s_vec3
{
	double			x;
	double			y;
	double			z;
}					t_vec3;

typedef union u_rsqrt
{
	double		f;
	uint64_t	i;
}	t_rsqrt;

static double	q_rsqrt(double number)
{
	double	x2;
	double	threehalfs;
	t_rsqrt	conv;

	x2 = number * 0.5;
	threehalfs = 1.5;
	conv.f = number;
	conv.i = 0x5fe6eb50c7b537a9 - (conv.i >> 1);
	conv.f *= threehalfs - (x2 * conv.f * conv.f);
	conv.f *= threehalfs - (x2 * conv.f * conv.f);
	return (conv.f);
}

void	q_normalize(t_vec3 *v)
{
	double	inv_len;

	inv_len = q_rsqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x *= inv_len;
	v->y *= inv_len;
	v->z *= inv_len;
}

void	normalize(t_vec3 *v)
{
	double	inv_len;

	inv_len = 1 / sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x *= inv_len;
	v->y *= inv_len;
	v->z *= inv_len;
}

int main() {
	t_vec3 *vecs = malloc(COUNT * sizeof(t_vec3));

	{
		clock_t start = clock();
		for (size_t i = 0; i < COUNT; i++) {
			normalize(&vecs[i]);
		}
		double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
		printf("normalize(): %.5f seconds.\n", elapsed);
	}
	{
		clock_t start = clock();
		for (size_t i = 0; i < COUNT; i++) {
			q_normalize(&vecs[i]);
		}
		double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
		printf("q_normalize(): %.5f seconds.\n", elapsed);
	}

	return 0;
}
