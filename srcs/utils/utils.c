/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 18:08:16 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/22 15:16:59 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool is_between_quotes(char *str)
{
	int s_quote;
	int d_quote;
	int i;

	i =-1;
	s_quote = 0;
	d_quote = 0;
	while (str[++i])
	{
		if (str[i] == DOUBLE_QUOTE && !s_quote)
			d_quote = !d_quote;
		else if (str[i] == SINGLE_QUOTE && !d_quote)
			s_quote = !s_quote;
	}
	if (s_quote != 0 || d_quote != 0)
		return true;
	return false;
}

bool is_everything_space(char *str)
{
	int i;

	i = -1;
	while (str[++i])
		if (str[i] != ' ')
			return false;
	return true;
}

bool	is_space(char c)
{
	if (c == 32)
		return true;
	return false;
}

