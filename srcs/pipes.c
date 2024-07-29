/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:44:25 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/29 23:08:27 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_pipes(int pipes[], int fd_in, int option);
void	setup_pipes(int pipes[], int i, int fd_in, int command_count);

void	open_pipes(t_commands *command)
{
	int	fd_in;
	int	pid_pipe;
	int	pipes_count;

	pipes_count = count_commands(command);
	if (pipes_count > 1)
	{
		fd_in = dup(STDIN_FILENO);
		pid_pipe = child_process(command, fd_in, pipes_count);
		parent_process(fd_in, pipes_count, pid_pipe);
	}
	else
		ft_execute(command, command);
}

void	parent_process(int fd_in, int count_pipes, int fork_pid)
{
	int	i;
	int	status;

	i = 0;
	close(fd_in);
	if (waitpid(fork_pid, &status, 0) != -1)
	{
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = WTERMSIG(status) + 128;
	}
	while (i < count_pipes)
	{
		wait(&status);
		i++;
	}
}

int	child_process(t_commands *command, int fd_in, int command_count)
{
	int			i;
	int			pipes[2];
	t_commands	*head;
	pid_t		pid;

	i = -1;
	head = command;
	while (++i < command_count)
	{
		ft_handle_signals(IGNORE);
		pipe(pipes);
		pid = fork();
		if (pid == 0)
		{
			setup_pipes(pipes, i, fd_in, command_count);
			close_pipes(pipes, fd_in, 1);
			ft_execute(command, head);
			head->env = command->env;
			free_all(head, 3);
		}
		dup2(pipes[0], fd_in);
		close_pipes(pipes, fd_in, 0);
		command = command->next;
	}
	return (pid);
}

void	setup_pipes(int pipes[], int i, int fd_in, int command_count)
{
	if (i != 0)
		dup2(fd_in, STDIN_FILENO);
	if ((i + 1) != command_count)
		dup2(pipes[1], STDOUT_FILENO);
}

void	close_pipes(int pipes[], int fd_in, int option)
{
	close(pipes[0]);
	close(pipes[1]);
	if (option)
		close(fd_in);
}
