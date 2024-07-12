/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:16:30 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 14:16:30 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*needs_expansion(char *input, t_commands *command)
{
	bool	single_quotes;
	bool	double_quotes;
	size_t	i;

	single_quotes = false;
	double_quotes = false;
	i = 0;
	while (input[i])
	{
		handle_quotes(input[i], &single_quotes, &double_quotes);
		if (should_expand(input[i], single_quotes))
		{
			if (!ft_strchr("'\"", input[i + 1]) && input[i + 1] != ' ')
				input = process_expansion(input, &i, command);
		}
		i++;
		if (i > ft_strlen(input))
			break ;
	}
	return (input);
}

char	*expand_variable(char *string, int i, t_commands *commands)
{
	char	*value;
	char	*key;
	char	*new_string;

	key = store_value(&string[i]);
	if (!key)
		return (NULL);
	value = ft_get_value(commands->env, key);
	if (!value)
		value = "";
	new_string = replace_variable(string, key, value, i);
	free(key);
	free(string);
	return (new_string);
}

void	copy_value_to_new_string(char *new_string, char *value, int *j)
{
	int	k;

	k = 0;
	while (value[k])
		new_string[(*j)++] = value[k++];
}
