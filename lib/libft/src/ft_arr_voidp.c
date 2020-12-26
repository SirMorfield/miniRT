/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_arr_voidp.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jkoers <jkoers@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/25 13:59:18 by jkoers        #+#    #+#                 */
/*   Updated: 2020/12/26 16:21:12 by jkoers        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "internal.h"
#include <stdlib.h>
#include <stddef.h>

t_arr_voidp	*ft_arr_voidp(size_t initial_size)
{
	t_arr_voidp	*arr;

	if (initial_size == 0)
		initial_size = ARR_DEFAULT_SIZE;
	arr = malloc(sizeof(t_arr_voidp));
	if (arr == NULL)
		return (NULL);
	arr->table = malloc(initial_size * sizeof(void *));
	if (arr->table == NULL)
	{
		free(arr);
		return (NULL);
	}
	arr->start_i = 0;
	arr->length = 0;
	arr->size = initial_size;
	return (arr);
}

static void	*ft_arr_voidp_grow(t_arr_voidp *arr, size_t new_size)
{
	void		*new_table;

	arr->size = new_size;
	new_table = malloc(arr->size * sizeof(void *));
	if (new_table == NULL)
		return (NULL);
	ft_memcpy(new_table, arr->table, arr->size * sizeof(void *));
	free(arr->table);
	arr->table = new_table;
	return (arr->table);
}

void		*ft_arr_voidp_set(t_arr_voidp **arr, size_t i, void *value)
{
	if (arr == NULL || *arr == NULL)
		*arr = ft_arr_voidp(i + ARR_DEFAULT_SIZE);
	else if ((*arr)->start_i + i >= (*arr)->size)
	{
		if (ft_arr_voidp_grow(*arr, (*arr)->size * 2 + 1) == NULL)
			return (NULL);
	}
	(*arr)->table[(*arr)->start_i + i] = value;
	if ((*arr)->length <= i)
		(*arr)->length = i + 1;
	return (*((*arr)->table + (*arr)->start_i + i));
}

void		*ft_arr_voidp_get(t_arr_voidp *arr, size_t i)
{
	if (arr == NULL)
		return (NULL);
	if (i > arr->length - 1)
		return (NULL);
	return (arr->table[arr->start_i + i]);
}

void		*ft_arr_voidp_push(t_arr_voidp **arr, void *value)
{
	size_t	i;

	if (arr == NULL || *arr == NULL)
		i = 0;
	else
		i = (*arr)->length;
	return (ft_arr_voidp_set(arr, i, value));
}