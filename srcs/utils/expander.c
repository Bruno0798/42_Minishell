/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:09:50 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/27 13:07:11 by bsousa-d         ###   ########.fr       */
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

void	expand_token_content(t_token *token, t_commands *cmd)
{
	if (*(ft_strchr(token->content, '$') + 1) == '?')
	{
		token->content = expand_exit_code(token->content);
		token->content = needs_expansion(token->content, cmd);
	}
	else
	{
		token->content = needs_expansion(token->content, cmd);
	}
}

void	process_heredoc_tokens(t_commands *cmd)
{
	t_token	*token;

	token = cmd->token;
	while (token && token->type != redir_in2)
	{
		if (ft_strchr(token->content, '$')
			&& *(ft_strchr(token->content, '$') + 1) != '\0'
			&& is_dollar_outside_single_quotes(token->content))
		{
			expand_token_content(token, cmd);
		}
		token = token->next;
	}
}

void	ft_expander_heredoc(t_commands *commands)
{
	t_commands	*head;

	head = commands;
	while (commands)
	{
		process_heredoc_tokens(commands);
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
	{
		token->content = process_token_content(token->content, commands);
		token->content = needs_expansion(token->content, commands);
	}
}
