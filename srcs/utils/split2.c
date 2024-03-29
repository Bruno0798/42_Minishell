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

static size_t	word_count(char const *s, char c)
{
	size_t	i;  /* Loop counter */
	size_t	j;  /* Word count */
	size_t	flag;  /* Flag indicating whether the current character is part of a word */
	size_t	d_quote;  /* Flag indicating whether the current character is inside double quotes */
	size_t 	s_quote;  /* Flag indicating whether the current character is inside single quotes */

	i = -1;
	j = 0;
	flag = 1;
	d_quote = 1;
	s_quote = 1;
	while (s[++i])  /* Iterate over each character in the string */
	{
		if (s[i] != c && flag)  /* If the character is not equal to the provided character and the flag is set */
		{
			flag = 0;  /* Reset the flag */
			j++;  /* Increment the word count */
		}
		if (s[i] == c  && (d_quote && s_quote))  /* If the character is equal to the provided character and both the double quote and single quote flags are set */
			flag = 1;  /* Set the flag */
		else if(s[i] == DOUBLE_QUOTE && s_quote)  /* If the character is a double quote and the single quote flag is set */
			d_quote = !d_quote;  /* Toggle the double quote flag */
		else if(s[i] == SINGLE_QUOTE && d_quote)  /* If the character is a single quote and the double quote flag is set */
			s_quote = !s_quote;  /* Toggle the single quote flag */
	}
	return (j);  /* Return the word count */
}

static size_t	letters_in_word(char const *s, char c, size_t i)
{
	size_t	size;  /* Number of characters in the word */
	size_t	d_quote;  /* Flag indicating whether the current character is inside double quotes */
	size_t	s_quote;  /* Flag indicating whether the current character is inside single quotes */

	size = 0;
	d_quote = 1;
	s_quote = 1;
	while (s[i] && !(s[i] == c && ((d_quote && s_quote))))  /* Iterate over each character in the string */
	{
		if(s[i] == DOUBLE_QUOTE && s_quote)  /* If the character is a double quote and the single quote flag is set */
			d_quote = !d_quote;  /* Toggle the double quote flag */
		else if(s[i] == SINGLE_QUOTE && d_quote)  /* If the character is a single quote and the double quote flag is set */
			s_quote = !s_quote;  /* Toggle the single quote flag */
		size++;  /* Increment the size */
		i++;  /* Move to the next character */
	}
	return (size);  /* Return the size */
}

char	**ft_split2(char const *s, char c)
{
	size_t	i;  /* Current index in the string */
	size_t	j;  /* Current index in the array of words */
	size_t	words;  /* Number of words in the string */
	char	**arr;  /* Array of words */

	if (!s)  /* If the string is NULL */
		return (NULL);  /* Return NULL */
	j = 0;
	i = 0;
	words = word_count(s, c);  /* Initialize the word count */
	arr = (char **)malloc(sizeof(char *) * (words + 1));  /* Allocate memory for the array of words */
	if (!arr)  /* If memory allocation fails */
		return (NULL);  /* Return NULL */
	while (j < words)  /* Iterate over each word in the string */
	{
		while (s[i] == c)  /* Skip any leading characters that are equal to the provided character */
			i++;
		arr[j] = ft_substr(s, i, letters_in_word(s, c, i));  /* Create a substring representing the word */
		if (!arr[j++])  /* If the substring creation fails */
			return (NULL);  /* Return NULL */
		i += letters_in_word(s, c, i);  /* Increment the index by the number of characters in the word */
	}
	arr[j] = NULL;  /* Set the last element of the array to NULL */
	return (arr);  /* Return the array of words */
}


