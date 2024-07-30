/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:36:18 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 07:57:10 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	replace_exit_code(char *string,
	char *new_string, char *num, int num_len)
{
	int	i;
	int	j;
	int	k;

	k = 0;
	i = 0;
	while (string[i])
	{
		if (string[i] == '$' && string[i + 1] == '?')
		{
			j = 0;
			while (j < num_len)
				new_string[k++] = num[j++];
			i += 2;
		}
		else
			new_string[k++] = string[i++];
	}
	new_string[k] = '\0';
}

char	*expand_exit_code(char *string)
{
	char	*num;
	char	*new_string;
	int		num_len;
	int		extra_length;

	num = ft_itoa(g_exit_status);
	if (!num)
	{
		free(string);
		return (NULL);
	}
	num_len = ft_strlen(num);
	extra_length = calculate_extra_length(string, num_len);
	new_string = malloc(ft_strlen(string) + extra_length + 1);
	if (!new_string)
	{
		free(num);
		free(string);
		return (NULL);
	}
	replace_exit_code(string, new_string, num, num_len);
	free(num);
	free(string);
	return (new_string);
}

char	*expand_variables(t_commands *commands, char *string)
{
	char	*value;
	char	*key;
	char	*new_string;

	value = store_value(string);
	if (!value)
		return (NULL);
	key = ft_get_value(commands->env, value);
	if (key)
		new_string = expand_new_string(value, key, string);
	else
		new_string = expand_new_string(value, "", string);
	free(value);
	if (!new_string)
		return (NULL);
	if (new_string != string)
	{
		free(string);
		string = new_string;
	}
	return (string);
}

char	*ft_allocate_new_string(size_t length)
{
	char	*new_string;

	new_string = malloc(sizeof(char) * length);
	if (!new_string)
		return (NULL);
	return (new_string);
}
