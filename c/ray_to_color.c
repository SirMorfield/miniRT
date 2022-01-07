/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ray_to_color.c                                     :+:    :+:            */
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

// Get closest t_obj * (relative to ray.origin) from *shapes

t_bounce	get_bounce(const t_arr *shapes, t_ray ray)
{
	size_t		i;
	t_obj		*obj;
	t_hit		hit;
	t_bounce	bounce;
	double		closest_dist;

	i = 0;
	bounce.obj = NULL;
	bounce.ray_origin = ray.origin;
	closest_dist = DOUBLE_MAX;
	while (ft_arr_get(shapes, i) != NULL)
	{
		obj = ft_arr_get(shapes, i);
		hit = hit_obj(obj->shape, obj->pos, ray);
		if (hit.hit && hit.dist < closest_dist)
		{
			closest_dist = hit.dist;
			bounce.obj = obj;
			bounce.color = obj->color;
			bounce.point = hit.point;
			bounce.normal = hit.normal;
		}
		i++;
	}
	return (bounce);
}

// Add light from *light to *l

static void	merge_lights(
	t_rgb *l,
	const t_light *light,
	t_bounce bounce,
	const t_arr *shapes)
{
	double		intensity;

	if (!is_clear_path(bounce, light, shapes))
		return ;
	intensity = relative_intensity(bounce.point, bounce.normal, light);
	*l = add_color(*l, light->color, intensity);
}

t_rgb	ray_to_color(t_ray ray, const t_gui *gui)
{
	size_t		i;
	t_rgb		l;
	t_bounce	bounce;

	bounce = get_bounce(gui->shapes, ray);
	if (bounce.obj == NULL)
		return (no_bounce());
	i = 0;
	l = gui->ambient.scalar;
	while (ft_arr_get(gui->lights, i) != NULL)
	{
		merge_lights(&l, ft_arr_get(gui->lights, i), bounce, gui->shapes);
		i++;
	}
	return (mix_color(l, bounce.color));
}
