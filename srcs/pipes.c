/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:44:25 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/04 18:19:27 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_pipes(int pipes[]);
void	setup_pipes(int pipes[], int i, int fd_in, int command_count);

void	open_pipes(t_commands *command)
{
	int	fd_in;
	int	pipes_count;

	pipes_count = count_commands(command);
	if (pipes_count > 1)
	{
		fd_in = dup(STDIN_FILENO);
		child_process(command, fd_in, pipes_count);
		parent_process(fd_in, pipes_count);
	}
	else
		ft_execute(command, command);
}

void	parent_process(int fd_in, int count_pipes)
{
	int	i;

	i = 0;
	close(fd_in);
	while (i < count_pipes)
	{
		wait(NULL);
		i++;
	}
}

void	child_process(t_commands *command, int fd_in, int command_count)
{
	int			i;
	int			pipes[2];
	t_commands	*head;
	pid_t		pid;

	i = -1;
	head = command;
	while (++i < command_count)
	{
		pipe(pipes);
		pid = fork();
		if (pid == 0)
		{
			ft_handle_signals(CHILD);
			setup_pipes(pipes, i, fd_in, command_count);
			close_pipes(pipes);
			close(fd_in);
			ft_execute(command, head);
			free_all(head, 2);
			exit(g_exit_status);
		}
		dup2(pipes[0], fd_in);
		close_pipes(pipes);
		command = command->next;
	}
}

void	setup_pipes(int pipes[], int i, int fd_in, int command_count)
{
	if (i != 0)
		dup2(fd_in, STDIN_FILENO);
	if ((i + 1) != command_count)
		dup2(pipes[1], STDOUT_FILENO);
}

void	close_pipes(int pipes[])
{
	close(pipes[0]);
	close(pipes[1]);
}
