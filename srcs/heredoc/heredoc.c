/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:59:32 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/02 17:34:35 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_files(t_commands *command, int flag)
{
	while (command)
	{
		if (flag == 1)
			check_heredoc(command);
		else
			files_exist(command->token);
		command = command->next;
	}
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
			if (fd < 0)
			{
				exist = false;
				ft_fprintf(2, ERROR_DIR); //TODO: exit code
			}
		}
		curr = curr->next;
	}
	return (exist);
}

void	check_heredoc(t_commands *command)
{
	t_token	*token;

	token = command->token;
	while (token)
	{
		if (token->type == redir_in2 && token->next)
		{
			ft_heredoc(token->next->content, command);
			dup2(open("heredoc.txt", O_RDONLY), STDIN_FILENO);
		}
		token = token->next;
	}
	delete_redir(command);
}

void	ft_heredoc(char *delimiter, t_commands *command)
{
	int		temp;
	pid_t	pid;

	ft_handle_signals(HERE_DOC);
	pid = fork();
	if (pid == 0)
	{
		heredoc_cycle(delimiter, command);
		free_all(command, 2);
		exit(g_exit_status);
	}
	waitpid(pid, &temp, 0);
	g_exit_status = temp >> 8;
}

void	heredoc_cycle(char *line, t_commands *commands)
{
	int		fd;
	char	*buff;

	ft_handle_signals(HERE_DOC);
	fd = open("heredoc.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	buff = readline(">");
	while (buff)
	{
		if (!ft_strcmp(buff, line))
			break ;
		if (ft_strchr(buff, '$') && *(ft_strchr(buff, '$') + 1)
			!= '\0' && is_dollar_outside_single_quotes(buff))
		{
			if (*(ft_strchr(buff, '$') + 1) == '?')
				buff = expand_exit_code(buff);
			else
				buff = needs_expansion(buff, commands);
		}
		buff = ft_strjoin(buff, "\n");
		ft_fprintf(fd, "%s", buff);
		free(buff);
		buff = readline(">");
	}
	close(fd);
}
