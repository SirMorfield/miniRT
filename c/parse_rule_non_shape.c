/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_rule_non_shape.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jkoers <jkoers@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/05 19:05:04 by jkoers        #+#    #+#                 */
/*   Updated: 2021/01/12 14:46:50 by jkoers        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "parse_rt.h"
#include "../lib/libft/include/libft.h"
#include <math.h>
#include <stdlib.h>
#include <limits.h>

void	add_camera(t_gui *gui, char *line)
{
	char		**items;
	t_camera	*cam;

	cam = calloc_safe(sizeof(t_camera));
	items = split_clamp(line, 4);
	set_point(&cam->origin, items[1]);
	set_dir(&cam->dir, items[2]);
	cam->fov = strtodbl_clamp(items[3], '\0', 0.0, 180.0);
	cam->fov = ft_radians(cam->fov);
	ft_free_until_null_char(items);
	if (ft_arr_push(&gui->cameras, cam) == NULL)
		exit_e("malloc");
	if (gui->cameras->length == 1)
		ft_memcpy(&gui->camera, cam, sizeof(t_camera));
}

void	add_light(t_arr **lights, char *line)
{
	char	**items;
	t_light	*light;

	light = malloc_safe(sizeof(t_light));
	items = split_clamp(line, 4);
	set_point(&light->origin, items[1]);
	light->brightness = strtodbl_clamp(items[2], '\0', 0.0, 1.0);
	set_color(&light->color, items[3]);
	ft_free_until_null_char(items);
	if (ft_arr_push(lights, light) == NULL)
		exit_e("malloc");
}

void	set_resolution(t_gui *gui, char *line)
{
	char	**items;

	items = split_clamp(line, 3);
	gui->x_size = (unsigned)strtonum_clamp(items[1], '\0', 1, LONG_MAX);
	gui->y_size = (unsigned)strtonum_clamp(items[2], '\0', 1, LONG_MAX);
	ft_free_until_null_char(items);
}

void	set_ambient(t_ambient *ambient, char *line)
{
	char	**items;

	items = split_clamp(line, 3);
	ambient->brightness = strtodbl_clamp(items[1], '\0', 0.0, 1.0);
	set_color(&ambient->color, items[2]);
	ambient->scalar.r = round(ambient->color.r * ambient->brightness);
	ambient->scalar.g = round(ambient->color.g * ambient->brightness);
	ambient->scalar.b = round(ambient->color.b * ambient->brightness);
	ft_free_until_null_char(items);
}
