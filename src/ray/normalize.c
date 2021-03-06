/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   normalize.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jkoers <jkoers@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/13 01:01:31 by jkoers        #+#    #+#                 */
/*   Updated: 2021/02/13 01:01:31 by jkoers        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include "constants.h"
#include <math.h>

/*
** A double implementation of the famous quake Q_rsqrt()
** Changed a bit to allow for norminette
** https://en.wikipedia.org/wiki/Fast_inverse_square_root
*/

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

void	normalize(t_vec3 *v)
{
	double	inv_len;

	inv_len = q_rsqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x *= inv_len;
	v->y *= inv_len;
	v->z *= inv_len;
}

t_vec3	unit(t_vec3 v)
{
	normalize(&v);
	return (v);
}
