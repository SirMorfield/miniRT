/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   compute_color.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jkoers <jkoers@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/12 16:15:37 by jkoers        #+#    #+#                 */
/*   Updated: 2021/02/12 16:15:37 by jkoers        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "intersect.h"
#include "constants.h"
#include "vector.h"
#include <math.h>

// Apparently adding the values as uint*_t is not presice enough and makes it
// seem there are fewer colors available, eg 150 per channel insted of 256
// Thus using the more precise doubles

static t_rgb	average_color(const t_rgb *colors, unsigned int n)
{
	double			r;
	double			g;
	double			b;
	unsigned int	i;

	r = 0.0;
	g = 0.0;
	b = 0.0;
	i = 0;
	while (i < n)
	{
		r += colors[i].r;
		g += colors[i].g;
		b += colors[i].b;
		i++;
	}
	r = round(r / n);
	g = round(g / n);
	b = round(b / n);
	return ((t_rgb){r, g, b});
}

t_rgb	compute_color(unsigned int x, unsigned int y, const t_gui *gui)
{
	double	aa;
	double	x_off;
	double	y_off;
	t_rgb	colors[ANTI_ALIASING_LEVEL];
	size_t	i;

	aa = 1.0 / sqrt(ANTI_ALIASING_LEVEL);
	i = 0;
	y_off = EPSILON;
	while (y_off < 1.0 && i < ANTI_ALIASING_LEVEL)
	{
		x_off = EPSILON;
		while (x_off < 1.0 && i < ANTI_ALIASING_LEVEL)
		{
			colors[i] = ray_to_color(
					ray_from_pix(x + x_off, y + y_off, gui), gui);
			i++;
			x_off += aa;
		}
		y_off += aa;
	}
	return (average_color(colors, ANTI_ALIASING_LEVEL));
}
