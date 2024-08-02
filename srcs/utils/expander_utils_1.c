/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:09:06 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/08/02 02:11:52 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_number(char *content)
{
	char	*result;
	char	*start;

	start = ft_strchr(content, '$');
	if (!start)
		return (content);
	if (is_between_quotes(content))
	{
		content = ft_delete_quotes(content);
		start = ft_strchr(content, '$');
	}
	start += 2;
	if (*(start - 1) == '0')
		result = ft_strjoin("minishell", start);
	else
		result = ft_strdup(start);
	free(content);
	return (result);
}

int	calculate_extra_length(char *string, int num_len)
{
	int	i;
	int	extra_length;

	i = 0;
	extra_length = 0;
	while (string[i])
	{
		if (string[i] == '$' && string[i + 1] == '?')
		{
			extra_length += num_len - 2;
			i++;
		}
		i++;
	}
	return (extra_length);
}

char	*store_value(char *string)
{
	int		i;
	int		length;
	char	*key;

	i = 1;
	length = 0;
	while (string[i] != '$' && string[i] != '\0'
		&& string[i] != ' ' && (ft_isalnum(string[i]) || string[i] == '_'))
	{
		i++;
		length++;
	}
	key = malloc(sizeof(char) * length + 1);
	i = 0;
	while (i < length)
	{
		key[i] = string[i + 1];
		i++;
	}
	key[i] = '\0';
	return (key);
}

char	*expand_new_string(char *value, char *key, char *string)
{
	size_t	new_string_length;
	char	*new_string;

	new_string_length = (ft_strlen(string) - ft_strlen(value)
			+ ft_strlen(key) + 1);
	new_string = ft_allocate_new_string(new_string_length);
	if (!new_string)
		return (NULL);
	replace_value_with_key(&new_string, value, key, string);
	return (new_string);
}

void	copy_key_to_new_string(char *new_string, int *j, char *key)
{
	int	k;

	k = 0;
	while (key[k])
		new_string[(*j)++] = key[k++];
}
