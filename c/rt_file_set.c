/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_file_set.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jkoers <jkoers@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/18 15:43:37 by jkoers        #+#    #+#                 */
/*   Updated: 2021/01/18 15:43:37 by jkoers        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parse_rt.h"
#include "constants.h"
#include "../lib/libft/include/libft.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

void	set_point(t_vec3 *origin, char *str)
{
	if (ft_strcount(str, ',') != 2)
		exit_e("Wrong number of args in point");
	origin->x = strtodbl_clamp(str, ',', DOUBLE_MIN, DOUBLE_MAX);
	str = ft_strchr(str, ',') + 1;
	origin->y = strtodbl_clamp(str, ',', DOUBLE_MIN, DOUBLE_MAX);
	str = ft_strchr(str, ',') + 1;
	origin->z = strtodbl_clamp(str, '\0', DOUBLE_MIN, DOUBLE_MAX);
}

// Set direction vector of a shape, eg. a surface normal of a plane

void	set_dir(t_vec3 *dir, char *str)
{
	double	len;

	if (ft_strcount(str, ',') != 2)
		exit_e("Wrong number of args in direction vector");
	dir->x = strtodbl_clamp(str, ',', -1.0, 1.0);
	str = ft_strchr(str, ',') + 1;
	dir->y = strtodbl_clamp(str, ',', -1.0, 1.0);
	str = ft_strchr(str, ',') + 1;
	dir->z = strtodbl_clamp(str, '\0', -1.0, 1.0);
	if (!ALLOW_ABNORMAL_DIR)
	{
		len = length(*dir);
		if (len < 0.9999 || len > 1.0001)
		{
			printf("Direction vector is not normalized (length %.10g)\n", len);
			normalize(dir);
			printf("Normalized: %.10g,%.10g,%.10g\n", dir->x, dir->y, dir->z);
			exit_e("");
		}
	}
}

void	set_color(t_rgb *color, char *str)
{
	if (ft_strcount(str, ',') != 2)
		exit_e("Wrong number of args in color");
	color->r = (unsigned char)strtonum_clamp(str, ',', 0, 255);
	str = ft_strchr(str, ',') + 1;
	color->g = (unsigned char)strtonum_clamp(str, ',', 0, 255);
	str = ft_strchr(str, ',') + 1;
	color->b = (unsigned char)strtonum_clamp(str, '\0', 0, 255);
}
