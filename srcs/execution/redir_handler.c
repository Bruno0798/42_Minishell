/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 14:54:26 by brpereir          #+#    #+#             */
/*   Updated: 2024/03/29 17:05:31 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_check_redirect(t_commands *command)
{
	t_token *temp;
	int count;
	int	fd;

	fd = 0;
	temp = command->token;
	while(temp)
	{
		if(temp->type == redir_out || temp->type == redir_out2 || temp->type == redir_in){
			count--;
			if(temp->type == redir_out){
				fd = open(temp->next->content, O_CREAT | O_RDWR | O_TRUNC, 0644);
				dup2(fd, STDOUT_FILENO);
			}
			else if (temp->type == redir_out2){
				fd = open(temp->next->content, O_CREAT | O_RDWR | O_APPEND, 0644);
				dup2(fd, STDOUT_FILENO);
			}
			else if (temp->type == redir_in){
				fd = open(temp->next->content, O_CREAT | O_RDWR, 0644);
				dup2(fd, STDIN_FILENO);
			}
		}
		temp = temp->next;
	}
    if(!fd)
        return (0);
	return (1);
}

void ft_handle_redirect(t_commands *command)
{
	t_token *temp;
	int		old_fd;

	temp = command->token;
	while(temp)
	{
		if(temp->next && (temp->next->type == redir_out || temp->next->type == redir_out2 || temp->next->type == redir_in))
			temp->next = NULL;
		temp = temp->next;
	}
	ft_execute(command);
}