/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:21:30 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/10 14:21:30 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_valid_input(char *input, t_env *env)
{
	if (!input)
	{
		free_env(env);
		g_exit_status = 0;
		exit(g_exit_status);
	}
	if (is_everything_space(input))
		return (false);
	if (!syntax_checker(input))
		return (false);
	if (check_quotes(input))
		return (false);
	return (true);
}

bool	is_between_single_quotes(char *str)
{
	int	s_quote;
	int	d_quote;
	int	i;

	i = -1;
	s_quote = 0;
	d_quote = 0;
	while (str[++i])
	{
		if (str[i] == DOUBLE_QUOTE && !s_quote)
			d_quote = !d_quote;
		else if (str[i] == SINGLE_QUOTE && !d_quote)
			s_quote = !s_quote;
	}
	if (s_quote != 0)
		return (true);
	return (false);
}
