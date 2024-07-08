/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 14:54:26 by brpereir          #+#    #+#             */
/*   Updated: 2024/07/08 20:38:48 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_redirection(t_token *token)
{
	int	fd;

	if (token->type == redir_out)
	{
		fd = open(token->next->content,O_CREAT | O_RDWR |O_TRUNC, 0644);
		if (fd !=-1)
			dup2(fd, STDOUT_FILENO);
	}
	else if (token->type == redir_out2)
	{
			fd = open(token->next->content,O_CREAT |O_RDWR | O_APPEND, 0644);
			if (fd !=-1)
				dup2(fd, STDOUT_FILENO);
		}
	else if (token->type == redir_in)
	{
		fd = open(token->next->content, O_RDONLY, 0644);
		if (fd != -1)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	if (fd == -1)
	{
		g_exit_status = 1;
		return (0);
	}
	token->next->type = files;
	return (1);
}

int	ft_check_redirect(t_commands *command)
{
	t_token	*temp;

	temp = command->token;
	while (temp)
	{
		if (temp->type == redir_out || temp->type == redir_out2
			|| temp->type == redir_in)
		{
			if (!handle_redirection(temp))
				return (0);
		}
		temp = temp->next;
	}
	return (1);
}

void	ft_handle_redirect(t_commands *commands)
{
	t_token	*token;
	t_token	*temp;
	t_token	*head;

	head = NULL;
	token = NULL;
	temp = commands->token;
	while (temp)
	{
		if (temp->type == command || temp->type == option)
		{
			token = malloc(sizeof(t_token));
			token->content = ft_strdup(temp->content);
			token->type = temp->type;
			token->next = NULL;
			ft_token_addback(&head, token);
		}
		temp = temp->next;
	}
	free_tokens(commands->token);
	commands->token = head;
}
