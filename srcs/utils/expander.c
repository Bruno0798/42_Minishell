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
			if (ft_strchr(token->content, '$')
				&& *(ft_strchr(token->content, '$') + 1) != '\0'
				&& is_dollar_outside_single_quotes(token->content))
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
	t_commands	*head;
	t_token		*token;

	head = commands;
	while (commands)
	{
		token = commands->token;
		while (token)
		{
			process_token(token, commands);
			token = token->next;
		}
		commands = commands->next;
	}
	commands = head;
}

void	process_token(t_token *token, t_commands *commands)
{
	char	*dollar_pos;

	dollar_pos = ft_strchr(token->content, '$');
	if (dollar_pos && *(dollar_pos + 1) != '\0'
		&& is_dollar_outside_single_quotes(token->content))
		token->content = process_token_content(token->content, commands);
}

char	*process_token_content(char *content, t_commands *commands)
{
	char	*dollar_pos;

	dollar_pos = ft_strchr(content, '$');
	if (*(dollar_pos + 1) == '?')
		return (expand_exit_code(content));
	else if (isdigit(*(dollar_pos + 1)))
		return (expand_number(content));
	else
		return (needs_expansion(content, commands));
}
