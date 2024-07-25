/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:59:32 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/25 19:53:58 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_files(t_commands *command, int flag)
{
	int			flag_here;
	t_commands	*temp;

	temp = command;
	flag_here = 0;
	while (temp)
	{
		if (flag == 1)
			flag_here = check_heredoc(temp, command);
		else
			files_exist(temp->token);
		temp = temp->next;
	}
	if (flag_here)
		dup2(open("heredoc.txt", O_RDONLY), STDIN_FILENO);
}

bool	files_exist(t_token *token)
{
	bool	exist;
	int		fd;
	t_token	*curr;

	exist = true;
	curr = token;
	while (curr)
	{
		if (curr->type == redir_in && curr->next)
		{
			fd = open(curr->next->content, O_RDONLY, 0644);
			if (fd == -1)
				exist = false;
		}
		curr = curr->next;
	}
	return (exist);
}

int	check_heredoc(t_commands *command, t_commands *head)
{
	t_token	*token;
	int		flag;

	flag = 0;
	token = command->token;
	while (token)
	{
		if (token->type == redir_in2 && token->next)
		{
			ft_heredoc(token->next->content, command, head);
			flag = 1;
		}
		token = token->next;
	}
	delete_redir(command);
	return (flag);
}

void	ft_heredoc(char *delimiter, t_commands *command, t_commands *head)
{
	int		temp;
	pid_t	pid;

	ft_handle_signals(HERE_DOC);
	pid = fork();
	if (pid == 0)
	{
		heredoc_cycle(delimiter, command);
		free_all(head, 2);
		exit(g_exit_status);
	}
	waitpid(pid, &temp, 0);
	g_exit_status = temp >> 8;
}

void	heredoc_cycle(char *line, t_commands *commands)
{
	int		fd;
	char	*buff;

	fd = open("heredoc.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (1)
	{
		buff = readline(">");
		if (buff == NULL || !ft_strcmp(buff, line))
		{
			if (buff == NULL)
				eof_heredoc(line);
			break ;
		}
		if (ft_strchr(buff, '$') && *(ft_strchr(buff, '$') + 1)
			!= '\0' && is_dollar_outside_single_quotes(buff))
		{
			if (*(ft_strchr(buff, '$') + 1) == '?')
				buff = expand_exit_code(buff);
			else
				buff = needs_expansion(buff, commands);
		}
		end_heredoc(buff, fd);
	}
	close(fd);
}
