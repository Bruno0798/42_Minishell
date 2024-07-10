/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:09:50 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/07 15:34:34 by bsousa-d         ###   ########.fr       */
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
char	*expand_number(char *content);

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
	t_token		*token;
	t_commands	*head;
	char		*dollar_pos;

	token = commands->token;
	head = commands;
	while (commands)
	{
		token = commands->token;
		while (token)
		{
			dollar_pos = ft_strchr(token->content, '$');
			if (dollar_pos && *(dollar_pos + 1) != '\0' && is_dollar_outside_single_quotes(token->content))
			{
				if (*(dollar_pos + 1) == '?')
					token->content = expand_exit_code(token->content);
				else if (isdigit(*(dollar_pos + 1)))
					token->content = expand_number(token->content);
				else
					token->content = needs_expansion(token->content, commands);
			}
			token = token->next;
		}
		commands = commands->next;
	}
	commands = head;
}

char	*expand_number(char *content)
{
	char	*result;
	char	*start;

	start = ft_strchr(content, '$');
	if (!start)
		return (content);
	if (content[1] == '0')
	{
		result = ft_strdup("minishell");
		return (result);
	}
	start += 2;
	result = ft_strdup(start);
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
