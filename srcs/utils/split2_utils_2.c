/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:25:08 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 14:25:08 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**allocate_array(size_t words)
{
	return ((char **)malloc(sizeof(char *) * (words + 1)));
}

char	*extract_word(const char *s, char c, size_t *i)
{
	size_t	len;
	char	*word;

	while (s[*i] == c)
		(*i)++;
	len = letters_in_word(s, c, *i);
	word = ft_substr(s, *i, len);
	*i += len;
	return (word);
}

char	**free_and_return_null(char **arr, size_t j)
{
	free_split(arr, j);
	return (NULL);
}

char	**ft_split2(char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	words;
	char	**arr;
	char	*word;

	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	words = word_count(s, c);
	arr = allocate_array(words);
	if (!arr)
		return (NULL);
	while (j < words)
	{
		word = extract_word(s, c, &i);
		if (!word)
			return (free_and_return_null(arr, j));
		arr[j++] = word;
	}
	arr[j] = NULL;
	return (arr);
}
