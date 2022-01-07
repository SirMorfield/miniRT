/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_rule_shapes.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jkoers <jkoers@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/27 20:55:30 by jkoers        #+#    #+#                 */
/*   Updated: 2021/01/12 14:15:10 by jkoers        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parse_rt.h"
#include "../lib/libft/include/libft.h"
#include "gui.h"
#include "constants.h"
#include "parse_rt.h"
#include "vector.h"
#include "intersect.h"
#include <stdlib.h>

void	add_sphere(t_arr **shapes, char *line)
{
	t_obj	*obj;
	char	**items;

	obj = malloc_safe(sizeof(t_obj));
	obj->shape = SHAPE_SPHERE;
	items = split_clamp(line, 4);
	set_point(&obj->pos.sp.origin, items[1]);
	obj->pos.sp.radius = strtodbl_clamp(items[2], '\0', 0.0, DOUBLE_MAX) * 0.5;
	obj->pos.sp.radius2 = obj->pos.sp.radius * obj->pos.sp.radius;
	set_color(&obj->color, items[3]);
	ft_free_until_null_char(items);
	if (ft_arr_push(shapes, obj) == NULL)
		exit_e("malloc");
}

void	add_plane(t_arr **shapes, char *line)
{
	t_obj	*obj;
	char	**items;

	obj = malloc_safe(sizeof(t_obj));
	obj->shape = SHAPE_PLANE;
	items = split_clamp(line, 4);
	set_point(&obj->pos.pl.origin, items[1]);
	set_dir(&obj->pos.pl.normal, items[2]);
	set_color(&obj->color, items[3]);
	ft_free_until_null_char(items);
	if (ft_arr_push(shapes, obj) == NULL)
		exit_e("malloc");
}

void	add_cylinder(t_arr **shapes, char *line)
{
	t_obj	*obj;
	char	**items;

	obj = malloc_safe(sizeof(t_obj));
	obj->shape = SHAPE_CYLINDER;
	items = split_clamp(line, 6);
	set_point(&obj->pos.cy.origin, items[1]);
	set_dir(&obj->pos.cy.dir, items[2]);
	obj->pos.cy.radius = strtodbl_clamp(items[3], '\0', 0.0, DOUBLE_MAX) * 0.5;
	obj->pos.cy.height = strtodbl_clamp(items[4], '\0', 0.0, DOUBLE_MAX);
	set_color(&obj->color, items[5]);
	obj->pos.cy.base2 = translate(
			obj->pos.cy.origin, obj->pos.cy.dir, obj->pos.cy.height);
	ft_free_until_null_char(items);
	if (ft_arr_push(shapes, obj) == NULL)
		exit_e("malloc");
}

void	add_triangle(t_arr **shapes, char *line)
{
	t_obj	*obj;
	char	**items;

	obj = malloc_safe(sizeof(t_obj));
	obj->shape = SHAPE_TRIANGLE;
	items = split_clamp(line, 5);
	set_point(&obj->pos.tr.p0, items[1]);
	set_point(&obj->pos.tr.p1, items[2]);
	set_point(&obj->pos.tr.p2, items[3]);
	set_color(&obj->color, items[4]);
	ft_free_until_null_char(items);
	obj->pos.tr.edge1 = subtract(obj->pos.tr.p1, obj->pos.tr.p0);
	obj->pos.tr.edge2 = subtract(obj->pos.tr.p2, obj->pos.tr.p0);
	obj->pos.tr.normal = normal_tr(
			obj->pos.tr.p0, obj->pos.tr.p1, obj->pos.tr.p2);
	if (ft_arr_push(shapes, obj) == NULL)
		exit_e("malloc");
}
