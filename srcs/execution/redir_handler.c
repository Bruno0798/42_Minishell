/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 14:54:26 by brpereir          #+#    #+#             */
/*   Updated: 2024/07/29 18:46:17 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_redirection(t_token *token)
{
	int	fd;

	fd = -1;
	if (token->type == redir_out)
		fd = handle_redir_out(token);
	else if (token->type == redir_out2)
		fd = handle_redir_out2(token);
	else if (token->type == redir_in)
		fd = handle_redir_in(token);
	if (fd == -1)
	{
		print_error(ERROR_DIR, token->next->content, 1);
		return (0);
	}
	token->next->type = files;
	return (1);
}

int	handle_redir_out(t_token *token)
{
	int	fd;

	fd = open(token->next->content, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd != -1)
		dup2(fd, STDOUT_FILENO);
	return (fd);
}

int	handle_redir_out2(t_token *token)
{
	int	fd;

	fd = open(token->next->content, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd != -1)
		dup2(fd, STDOUT_FILENO);
	return (fd);
}

int	handle_redir_in(t_token *token)
{
	int	fd;

	fd = open(token->next->content, O_RDONLY, 0644);
	if (fd != -1)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (fd);
}

int	ft_check_redirect(t_commands *command)
{
	t_token	*temp;

	temp = command->token;
	while (temp)
	{
		if (temp->type == error)
		{
			print_error(ERROR_REDIR, NULL, 2);
			return (0);
		}
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
