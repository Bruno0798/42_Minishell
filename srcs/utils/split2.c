/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 16:54:46 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 12:44:30 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_split(char **arr, size_t j)
{
	size_t	i;

	i = 0;
	while (i < j)
		free(arr[i++]);
	free(arr);
}

size_t	letters_in_word(char const *s, char c, size_t i)
{
	size_t	size;
	size_t	d_quote;
	size_t	s_quote;

	size = 0;
	d_quote = 1;
	s_quote = 1;
	while (s[i] && !(s[i] == c && (d_quote && s_quote)))
	{
		if (s[i] == DOUBLE_QUOTE && s_quote)
			d_quote = !d_quote;
		else if (s[i] == SINGLE_QUOTE && d_quote)
			s_quote = !s_quote;
		size++;
		i++;
	}
	return (size);
}
