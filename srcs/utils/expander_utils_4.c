/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:11:29 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 14:11:29 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*allocate_new_string(char *string, char *key, char *value)
{
	return (malloc(ft_strlen(string) - ft_strlen(key) + ft_strlen(value) + 1));
}

bool	is_variable_match(char *string, char *key, int i, int h)
{
	return (string[h] == '$'
		&& ft_strncmp(&string[h + 1], key, ft_strlen(key)) == 0 && i == h);
}

char	*replace_variable(char *string, char *key, char *value, int i)
{
	char	*new_string;
	int		h;
	int		j;

	h = 0;
	j = 0;
	new_string = allocate_new_string(string, key, value);
	while (string[h])
	{
		if (is_variable_match(string, key, i, h))
		{
			h += ft_strlen(key) + 1;
			copy_value_to_new_string(new_string, value, &j);
			copy_remaining_string(new_string, string, &h, &j);
			break ;
		}
		new_string[j++] = string[h++];
	}
	new_string[j] = '\0';
	return (new_string);
}

bool	is_dollar_outside_single_quotes(char *str)
{
	bool	s_quote;
	bool	d_quote;
	int		i;

	i = -1;
	s_quote = false;
	d_quote = false;
	while (str[++i])
	{
		if (str[i] == DOUBLE_QUOTE)
			d_quote = !d_quote;
		if (str[i] == SINGLE_QUOTE && !d_quote)
			s_quote = !s_quote;
		else if (str[i] == '$' && !s_quote)
			return (true);
	}
	return (false);
}

void	copy_remaining_string(char *new_string, char *string, int *h, int *j)
{
	while (string[*h])
		new_string[(*j)++] = string[(*h)++];
}
