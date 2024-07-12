/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:21:32 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 14:45:09 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_quotes(char c, size_t *d_quote, size_t *s_quote)
{
	if (c == DOUBLE_QUOTE && *s_quote)
		*d_quote = !*d_quote;
	else if (c == SINGLE_QUOTE && *d_quote)
		*s_quote = !*s_quote;
}

void	check_word_boundary(char c, char sep, size_t *flag, size_t *j)
{
	if (c != sep && *flag)
	{
		*flag = 0;
		(*j)++;
	}
}

size_t	word_count(char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	flag;
	size_t	d_quote;
	size_t	s_quote;

	i = 0;
	j = 0;
	flag = 1;
	d_quote = 1;
	s_quote = 1;
	while (s[i])
	{
		check_word_boundary(s[i], c, &flag, &j);
		if (s[i] == c && (d_quote && s_quote))
			flag = 1;
		update_quotes(s[i], &d_quote, &s_quote);
		i++;
	}
	return (j);
}
