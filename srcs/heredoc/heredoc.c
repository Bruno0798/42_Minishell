/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:59:32 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/05/21 13:59:32 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void heredoc_files(t_commands *command, int flag)
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

bool files_exist(t_token *token)
{
	bool exist;
	int fd;
	t_token *curr;

	exist = true;
	curr = token;
	while(curr)
	{
		if (curr->type == redir_in && curr->next)
		{
			fd = open(curr->next->content, O_RDONLY, 0644);
			if (fd < 0)
			{
				exist = false;
				print_error(ERROR_DIR, curr->next->content, 1); //TODO: exit code
			}
		}
		curr = curr->next;
	}
	return exist;
}

void check_heredoc(t_commands *command)
{
	t_token *token;

	token = command->token;

	while (token)
	{
		if (token->type == redir_in2 && token->next)
		{
			ft_heredoc(token->next->content);
			dup2(open("heredoc.txt", O_RDONLY), STDIN_FILENO);
		}
		token = token->next;
	}
	delete_redir(command);
}

void ft_heredoc(char *delimiter)
{
	int temp;
	pid_t pid;
	char *line;

	handle_signals();
	pid = fork();
	if (pid == 0)
	{
		line = ft_strjoin(delimiter, "\n");
		heredoc_cycle(line);
		free(line);
		exit(EXIT_STATUS);
	}
	waitpid(pid, &temp, 0);
	EXIT_STATUS = temp >> 8;
}

void heredoc_cycle(char *line)
{
	int fd;
	int output;
	char buff[4095];

	ft_signals_heredoc();
	fd = open("heredoc.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	write(1, "> ", 2);
	output = read(1, buff, 4095);
	while(output > 0)
	{
		ft_signals_heredoc();
		buff[output] = '\0';
		if (!ft_strcmp(buff, line))
			break;
		write(fd, buff, output);
		write(1, "> ", 2);
		output = read(1, buff, 4095);
	}
	close(fd);
}

bool has_here_doc(t_commands *commands)
{
	t_token *curr = commands->token;

	while (curr)
	{
		if (curr->type == redir_in2)
			return true;
		curr = curr->next;
	}
	return false;
}

void remake_commands(t_commands *command)
{
	t_commands *temp;

	temp = command;
	heredoc_files(temp, 1);
	heredoc_files(temp, 2);
}