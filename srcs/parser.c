/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:11:34 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 13:45:06 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	check_syntax(t_commands *commands)
{
	t_commands	*temp;
	t_token		*curr;

	temp = commands;
	while (temp)
	{
		curr = commands->token;
		while (curr)
		{
			if (curr->type == redir_in || curr->type == redir_out)
			{
				if (curr->next && curr->next->type != command
					&& (curr->next->type != files
						|| curr->next->type == redir_in2
						|| curr->next->type == redir_out2))
				{
					ft_printf("error token: %s \n", curr->next->content);
					return (true);
				}
			}
			curr = curr->next;
		}
		temp = temp->next;
	}
	return (false);
}

int	ft_parser(char *input, t_commands **commands, t_env *env)
{
	*commands = pipe_commands(input, env);
	if (*commands == NULL)
		return (EXIT_FAILURE);
	if (check_syntax(*commands))
		return (EXIT_FAILURE);
	if (!has_here_doc(*commands))
		ft_expander(*commands);
	else
		ft_expander_heredoc(*commands);
	ft_empty_nodes(*commands);
	ft_remove_quotes(*commands);
	ft_expand_others(*commands);
	return (EXIT_SUCCESS);
}

void	free_and_update_token(t_commands *cmd,
		t_token **token_temp, t_token **prev)
{
	t_token	*temp;

	temp = cmd->token;
	if (*prev != NULL)
		(*prev)->next = cmd->token->next;
	else
		*token_temp = cmd->token->next;
	free(temp->content);
	free(temp);
	if (*prev == NULL)
		cmd->token = *token_temp;
	else
		cmd->token = (*prev)->next;
}

void	process_tokens(t_commands *cmd)
{
	t_token	*token_temp;
	t_token	*prev;

	token_temp = cmd->token;
	prev = NULL;
	while (cmd->token != NULL)
	{
		if (cmd->token->content != NULL
			&& ft_strcmp(cmd->token->content, "") == 0)
			free_and_update_token(cmd, &token_temp, &prev);
		else
		{
			prev = cmd->token;
			cmd->token = cmd->token->next;
		}
	}
	cmd->token = token_temp;
}

void	ft_empty_nodes(t_commands *commands)
{
	t_commands	*head;

	head = commands;
	while (commands)
	{
		process_tokens(commands);
		commands = commands->next;
	}
	commands = head;
}
