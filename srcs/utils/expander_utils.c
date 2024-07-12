/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:34:32 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 14:11:55 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	value_length(char *string)
{
	int	i;
	int	length;

	i = 0;
	length = 0;
	while (string[i] && string[i] != '$')
		i++;
	if (string[i] == '\0')
		return (0);
	i++;
	while (string[i] && string[i] != ' ' && string[i] != '$')
	{
		length++;
		i++;
	}
	return (length);
}

char	*ft_get_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (!strcmp(current->key, key))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	handle_quotes(char c, bool *single_quotes, bool *double_quotes)
{
	if (c == '\'' && !*double_quotes)
		*single_quotes = !*single_quotes;
	else if (c == '"' && !*single_quotes)
		*double_quotes = !*double_quotes;
}

bool	should_expand(char c, bool single_quotes)
{
	return (c == '$' && !single_quotes);
}

char	*process_expansion(char *input, size_t *i, t_commands *command)
{
	int		length;
	char	*new_input;

	length = ft_strlen(input);
	new_input = expand_variable(input, *i, command);
	*i += ft_strlen(new_input) - length;
	return (new_input);
}
