/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   settings.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jkoers <jkoers@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/07 18:16:05 by jkoers        #+#    #+#                 */
/*   Updated: 2021/02/07 18:16:05 by jkoers        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETTINGS_H
# define SETTINGS_H

/*
** README
** This the settings file for the miniRT project.
** After changing a setting you of course have to recompile with make re
** The first line of every setting is a question that the property
** answer to.
*/

/*
** Bool: Enable verbose logging? :boolean
*/
# define VERBOSE 1

/*
** Allow non-normalized direction vector in .rt file? :boolean
** The vector will still be normalized before internal use.
*/
# define ALLOW_ABNORMAL_DIR 1 // illegal

/*
** When reading the rt file, only allow a window resolution that is <= than the
** display resolution? :boolean
** This max resolution is always ignored when exporting a bmp file.
*/
# define MAX_WINDOW_SIZE 0

/*
** Allow missing rules in the .rt file such as the R (resolution) rule? :boolean
*/
# define ALLOW_UNSET_RULES 1

/*
** Allow comments (line prefixed by #) .rt file? :boolean
*/
# define ALLOW_RT_COMMENTS 1

#endif