/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:34:32 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/11 14:36:07 by bsousa-d         ###   ########.fr       */
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

char	*needs_expansion(char *input, t_commands *command)
{
	bool	single_quotes;
	bool	double_quotes;
	int		i;
	int		length;

	single_quotes = false;
	double_quotes = false;
	i = -1;
	while (input[++i])
	{
		if (input[i] == '\'' && !double_quotes)
			single_quotes = !single_quotes;
		else if (input[i] == '"' && !single_quotes)
			double_quotes = !double_quotes;
		else if (input[i] == '$' && !single_quotes)
		{
			if (input[i + 1] != ' ' && input[i + 1] != '\'' && input[i + 1] != '\"')
			{
				length = ft_strlen(input);
				input = expand_variable(input, i, command);
				i += ft_strlen(input) - length;
			}
		}
		if (i > ft_strlen(input))
			break ;
	}
	return (input);
}

char *expand_variable(char *string, int i, t_commands *commands)
{
	char *value;
	char *key;
	char *new_string;

	key = store_value(&string[i]);
	if (!key)
		return NULL;
	value = ft_get_value(commands->env, key);
	if (!value)
		value = "";
	new_string = replace_variable(string, key, value, i);
	free(key);
	free(string);
	return (new_string);
}

char *replace_variable(char *string, char *key, char *value, int i)
{
	char *new_string;
	int h;
	int k;
	int j;

	h = 0;
	k = 0;
	j = 0;
	new_string = malloc(ft_strlen(string) - ft_strlen(key) + ft_strlen(value) + 1);
	while (string[h])
	{
		if (string[h] == '$' && ft_strncmp(&string[h + 1], key, ft_strlen(key)) == 0 && i == h)
		{
			h += ft_strlen(key) + 1;
			while (value[k])
				new_string[j++] = value[k++];
			while (string[h])
				new_string[j++] = string[h++];
			break ;
		}
		new_string[j++] = string[h++];
	}
	new_string[j] = '\0';
	return (new_string);
}

bool	is_dollar_outside_single_quotes(char *str)
{
	bool	s_quote;
	bool	d_quote;
	int		i;

	i = -1;
	s_quote = false;
	d_quote = false;
	while (str[++i])
	{
		if (str[i] == DOUBLE_QUOTE)
			d_quote = !d_quote;
		if (str[i] == SINGLE_QUOTE && !d_quote)
			s_quote = !s_quote;
		else if (str[i] == '$' && !s_quote)
			return (true);
	}
	return (false);
}
