/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   count_rules.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jkoers <jkoers@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/07 17:39:01 by jkoers        #+#    #+#                 */
/*   Updated: 2021/02/07 17:39:01 by jkoers        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parse_rt.h"
#include "constants.h"
#include "../lib/libft/include/libft.h"
#include <stdlib.h>
#include <stdio.h>

static void	count_rules(unsigned int *rules_n, char **rt)
{
	size_t	i;
	t_rule	rule_i;

	i = 0;
	while (rt[i])
	{
		rule_i = 0;
		while (rule_i < RULE_LAST)
		{
			if (is_rule(rt[i], rule_i))
				rules_n[rule_i] += 1;
			rule_i++;
		}
		i++;
	}
}

static void	exit_not_one_declaration(t_rule rule, unsigned int got)
{
	printf("Rule %s (%s) has %u occurrences, only 1 declaration allowed\n",
		rule_id(rule), rule_name(rule), got);
	exit(1);
}

void	exit_on_illegal_rule_n(char **rt)
{
	unsigned int	rules_n[RULE_LAST];

	ft_bzero(rules_n, sizeof(rules_n));
	count_rules(rules_n, rt);
	if (rules_n[RULE_RESOLUTION] != 1)
		exit_not_one_declaration(RULE_RESOLUTION, rules_n[RULE_RESOLUTION]);
	if (rules_n[RULE_AMBIENT] != 1)
		exit_not_one_declaration(RULE_AMBIENT, rules_n[RULE_AMBIENT]);
	if (rules_n[RULE_CAMERA] == 0)
	{
		printf("0 %s (%s) found, expected at least 1\n",
			rule_id(RULE_CAMERA), rule_name(RULE_CAMERA));
		exit(1);
	}
}
