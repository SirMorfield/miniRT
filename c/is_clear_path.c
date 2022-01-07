/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   is_clear_path.c                                    :+:    :+:            */
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

static bool	same_point(t_vec3 p1, t_vec3 p2, double epsilon)
{
	return ((p1.x >= p2.x - epsilon && p1.x <= p2.x + epsilon)
		&& (p1.y >= p2.y - epsilon && p1.y <= p2.y + epsilon)
		&& (p1.z >= p2.z - epsilon && p1.z <= p2.z + epsilon));
}

// Pointing a new ray to hitpoint, form origin *l

t_bounce	bounce_from_light(
		t_vec3 hitpoint, const t_light *l, const t_arr *shapes)
{
	t_bounce	from_light;
	t_ray		ray;

	ray.origin = l->origin;
	ray.dir = unit(subtract(hitpoint, l->origin));
	from_light = get_bounce(shapes, ray);
	return (from_light);
}

// If there is a clear path the detected bounce, and the light *l
// Assuming from_camera has bounced

bool	is_clear_path(
	t_bounce from_camera, const t_light *l, const t_arr *shapes)
{
	t_bounce	from_light;

	from_light = bounce_from_light(from_camera.point, l, shapes);
	if (from_light.obj == NULL)
		return (false);
	if (from_camera.obj != from_light.obj)
		return (false);
	if (!same_point(from_camera.normal, from_light.normal, 0.01))
		return (false);
	if (!same_point(from_camera.point, from_light.point, 0.01))
		return (false);
	return (true);
}
