/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 16:54:46 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/03/29 16:12:10 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void free_split(char **arr, size_t j)
{
	size_t i;

	for (i = 0; i < j; i++)
		free(arr[i]);
	free(arr);
}

static size_t word_count(char const *s, char c)
{
	size_t i;
	size_t j;
	size_t flag;
	size_t d_quote;
	size_t s_quote;

	i = 0;
	j = 0;
	flag = 1;
	d_quote = 1;
	s_quote = 1;
	while (s[i])
	{
		if (s[i] != c && flag)
		{
			flag = 0;
			j++;
		}
		if (s[i] == c && (d_quote && s_quote))
			flag = 1;
		else if (s[i] == DOUBLE_QUOTE && s_quote)
			d_quote = !d_quote;
		else if (s[i] == SINGLE_QUOTE && d_quote)
			s_quote = !s_quote;
		i++;
	}
	return j;
}

static size_t letters_in_word(char const *s, char c, size_t i)
{
	size_t size;
	size_t d_quote;
	size_t s_quote;

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
	return size;
}

char **ft_split2(char const *s, char c)
{
	size_t i;
	size_t j;
	size_t words;
	char **arr;

	if (!s)
		return NULL;
	j = 0;
	i = 0;
	words = word_count(s, c);
	arr = (char **)malloc(sizeof(char *) * (words + 1));
	if (!arr)
		return NULL;
	while (j < words)
	{
		while (s[i] == c)
			i++;
		size_t len = letters_in_word(s, c, i);
		arr[j] = ft_substr(s, i, len);
		if (!arr[j])
		{
			free_split(arr, j);
			return NULL;
		}
		i += len;
		j++;
	}
	arr[j] = NULL;
	return arr;
}

