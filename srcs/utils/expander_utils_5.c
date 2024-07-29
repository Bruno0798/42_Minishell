/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:16:30 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/29 19:30:52 by bsousa-d         ###   ########.fr       */
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
			if (input[i + 1] != ' ' && input[i + 1] != '.')
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

char	*expand_heredoc(char *input, t_commands *commands)
{
	int		i;
	int		length;
	char	*new_input;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$')
		{
			if (!ft_strchr("'\"", input[i + 1]) && input[i + 1] != ' ')
			{
				length = (int)ft_strlen(input);
				new_input = expand_variable(input, i, commands);
				i += ft_strlen(new_input) - length;
				input = new_input;
			}
		}
		i++;
		if (i > (int)ft_strlen(input))
			break ;
	}
	return (input);
}
