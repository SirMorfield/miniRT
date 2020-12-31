/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_file.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jkoers <jkoers@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/28 13:39:05 by jkoers        #+#    #+#                 */
/*   Updated: 2020/12/31 01:48:44 by jkoers        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"
#include "shapes.h"
#include "constants.h"
#include "../lib/minilibx-linux/mlx.h"
#include "../lib/ft_printf/ft_printf.h"
#include "../lib/libft/include/libft.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// bool	has_duplicate_rules(char **rt)
// {
// 	return (false);
// }

e_msg	destroy_set_resolution(e_msg msg, char **params)
{
	ft_free_until_null_char(params);
	if (msg != SUCCESS)
		ft_printf("Failed to set resolution\n"); // illegal
	return (msg);
}

e_msg	set_resolution(char *line, unsigned long *x, unsigned long *y)
{
	char	**params;

	if (split_clamp(&params, line, 3) != SUCCESS)
		return (destroy_set_resolution(ERR_RT_BADRULE, params));
	if (strtonum_clamp((long *)x, params[1], '\0', 1, LONG_MAX) != SUCCESS)
		return (destroy_set_resolution(ERR_RT_BADVALUE, params));
	if (strtonum_clamp((long *)y, params[2], '\0', 1, LONG_MAX) != SUCCESS)
		return (destroy_set_resolution(ERR_RT_BADVALUE, params));	
	return (destroy_set_resolution(SUCCESS, params));
}

e_msg	parse_rt_line(char *line, t_gui *gui)
{
	if (line[0] == '#' || ft_strlen(line) <= 1)
		return (SUCCESS);
	else if (ft_strcmp(line, RULE_RESOLUTION) == ' ')
		return (set_resolution(line, &gui->x_size, &gui->y_size));
	else if (ft_strcmp(line, RULE_SPHERE) == ' ')
		return (add_sphere(line, gui->shapes));
	else
		return (ERR_RT_UNKNOWN_RULE);
}

e_msg	destroy_parse_rt(e_msg msg, char **rt, size_t i)
{
	ft_free_until_null_char(rt);
	if (msg != SUCCESS)
		printf("Failed at line %lu\n", i + 1); // illegal
	return (msg);
}

e_msg	parse_rt(char *rt_filename, t_gui *gui)
{
	size_t	rt_lines;
	char	**rt;
	e_msg	msg;
	size_t	i;

	rt = ft_split_file(rt_filename, &rt_lines);
	if (rt == NULL || rt_lines == 0)
		return (destroy_parse_rt(ERR_RT_EMPTY, rt, 0));
	i = 0;
	while (rt[i])
	{
		msg = parse_rt_line(rt[i], gui);
		if (msg != SUCCESS)
			return (destroy_parse_rt(msg, rt, i));
		i += 1;
	}
	return (destroy_parse_rt(SUCCESS, rt, i));
}
