/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:09:50 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/02 11:04:37 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "../../includes/minishell.h"

char	*expand_exit_code(char *string);
char	*expand_variables(t_commands *commands, char *string);
char	*store_value(char *string);
int		value_length(char *string);
char	*expand_new_string(char *value, char*key, char *string);
char	*needs_expansion(char *input, t_commands *command);
char	*expand_variable(char *string, int i, t_commands *commands);
bool	is_dollar_outside_single_quotes(char *str);

void	ft_expander_heredoc(t_commands *commands)
{
	t_token		*token;
	t_commands	*head;

	head = commands;
	token = commands->token;

	while (commands)
	{
		token = commands->token;
		while (token && token->type != redir_in2)
		{
			if (ft_strchr(token->content, '$') && *(ft_strchr(token->content, '$') + 1) != '\0' && is_dollar_outside_single_quotes(token->content))
			{
				if (*(ft_strchr(token->content, '$') + 1) == '?')
					token->content = expand_exit_code(token->content);
				else
					token->content = needs_expansion(token->content, commands);
			}
			token = token->next;
		}
		commands = commands->next;
	}
	
	commands = head;
}

void	ft_expander(t_commands *commands)
{
	t_token *token = commands->token;
	t_commands *head = commands;

	while (commands)
	{
		token = commands->token;
		while (token)
		{
			if (ft_strchr(token->content, '$') && *(ft_strchr(token->content, '$') + 1) != '\0' && is_dollar_outside_single_quotes(token->content))
			{
				if (*(ft_strchr(token->content, '$') + 1) == '?')
					token->content = expand_exit_code(token->content);
				
				token->content = needs_expansion(token->content, commands);
			}
			token = token->next;
		}
		commands =commands->next;
	}
	commands = head;

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

void	replace_exit_code(char *string, char *new_string, char *num, int num_len)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = 0;
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

char	*expand_exit_code(char *string) {
	char	*num;
	char	*new_string;
	int		num_len;
	int		extra_length;

	num = ft_itoa(g_exit_status);
	num_len = ft_strlen(num);
	extra_length = calculate_extra_length(string, num_len);
	new_string = malloc(ft_strlen(string) + extra_length + 1);
	replace_exit_code(string, new_string, num, num_len);
	free(num);
	return (new_string);
}

char	*expand_variables(t_commands *commands, char *string)
{
	char	*value;
	char	*key;
	char	*new_string;

	value = store_value(string);
	if (!value)
		return NULL;
	key = ft_get_value(commands->env, value);
	if (key)
		new_string = expand_new_string(value, key, string);
	else
		new_string = expand_new_string(value, "", string);
	free(value);
	if (!new_string)
		return NULL;
	if (new_string != string)
	{
		free(string);
		string = new_string;
	}
	return (string);
}

char	*expand_new_string(char *value, char* key, char *string)
{
	int		i;
	int		j;
	int		k;
	char	*new_string;

	i = 0;
	j = 0;
	k = 0;
	new_string = malloc(sizeof(char) * (ft_strlen(string) - ft_strlen(value) + ft_strlen(key) + 1));
	if (!new_string) // Check for malloc failure
		return NULL;
	while (string[i])
	{
		if (string[i] == '$' && ft_strncmp(&string[i + 1], value, ft_strlen(value)) == 0)
		{
			while (key[k])
				new_string[j++] = key[k++];
			i += ft_strlen(value) + 1;
			while (string[i])
				new_string[j++] = string[i++];
			break;
		}
		new_string[j++] = string[i++];
	}
	new_string[j] = '\0'; // Null-terminate the new string
	return new_string;
}

char	*store_value(char *string)
{
	int		i;
	int		length;
	char	*key;

	i = 1;
	length = 0;
	while (string[i] != '$' && string[i] != DOUBLE_QUOTE && string[i] != '\0' && string[i] != ' ' && string[i] != SINGLE_QUOTE)
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

int	value_length(char *string) {
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
			return current->value;
		current = current->next;
	}
	return (NULL);
}

char	*needs_expansion(char *input, t_commands *command)
{
	bool single_quotes;
	bool double_quotes;
	int i;
	int length;

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
			if (input[i + 1] != ' ' && input[i + 1] != SINGLE_QUOTE && input[i + 1] != DOUBLE_QUOTE)
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

char	*expand_variable(char *string, int i, t_commands *commands)
{
    char	*value;
    char	*key;
    char	*new_string;
    int h = 0;
    int k = 0;
    int j = 0;

    key = store_value(&string[i]);
    if (!key)
        return NULL;
	value = ft_get_value(commands->env, key);
	if (!value)
		value = "";
	new_string = malloc(strlen(string) - strlen(key) + strlen(value) + 1);
	if (!new_string)
    {
		free(key);
		return (NULL);
	}
	while (string[h])
	{
		if (string[h] == '$' && strncmp(&string[h + 1], key, strlen(key)) == 0 && i == h)
		{
			h += strlen(key) + 1;
			while (value[k])
				new_string[j++] = value[k++];
			while (string[h])
				new_string[j++] = string[h++];
			break ;
		}
		new_string[j++] = string[h++];
	}
	new_string[j] = '\0';
	free(key);
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
