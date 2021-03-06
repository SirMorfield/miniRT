/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_rt.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jkoers <jkoers@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/28 13:47:29 by jkoers        #+#    #+#                 */
/*   Updated: 2021/01/12 14:48:43 by jkoers        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_RT_H
# define PARSE_RT_H

# include "gui.h"
# include "constants.h"
# include "../lib/libft/include/libft.h"

void	parse_rt(t_gui *gui, const char *rt_filename);
void	exit_on_illegal_rule_n(char **rt);

long	strtonum_clamp(char *str, char end, long min, long max);
double	strtodbl_clamp(char *str, char end, double min, double max);
char	**split_clamp(char *line, size_t should_be_n);

void	set_point(t_vec3 *origin, char *str);
void	set_dir(t_vec3 *dir, char *str);
void	set_color(t_rgb *color, char *str);

void	add_sphere(t_arr **shapes, char *line);
void	add_plane(t_arr **shapes, char *line);
void	add_square(t_arr **shapes, char *line);
void	add_cylinder(t_arr **shapes, char *line);
void	add_triangle(t_arr **shapes, char *line);

void	log_shapes(const t_arr *shapes);

void	add_camera(t_gui *gui, char *line);
void	add_light(t_arr **lights, char *line);
void	set_resolution(t_gui *gui, char *line);
void	set_ambient(t_ambient *ambient, char *line);
bool	is_rule(char *line, t_rule rule);

#endif
