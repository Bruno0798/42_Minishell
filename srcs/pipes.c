/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:44:25 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/05/27 14:36:22 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void open_pipes(t_commands *command)
{
	int fd_in;
	int pipes_count;

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

void parent_process(int fd_in, int count_pipes)
{
	int i;
	int status;

	close(fd_in); // Close the input file descriptor

	for (i = 0; i < count_pipes; i++)
	{
		wait(&status); // Wait for each child process

		if (WIFEXITED(status)) // Check if the child terminated normally
		{
			int child_exit_status = WEXITSTATUS(status); // Get the exit status of the child
			if (child_exit_status != 0) // If any child process exits with a non-zero status
			{
				EXIT_STATUS = child_exit_status; // Update the global exit status
			}
		}
		else if (WIFSIGNALED(status)) // If the child was terminated by a signal
		{
			int signal_number = WTERMSIG(status);
			fprintf(stderr, "Child process was terminated by signal %d\n", signal_number);
			EXIT_STATUS = 128 + signal_number; // Commonly used convention for signal termination status
		}
	}
}

void child_process(t_commands *command, int fd_in, int command_count)
{
	int i;
	int pipes[2];
	t_commands *head;
	pid_t  pid;

	i = 0;
	head = command;
	while (i < command_count)
	{
		pipe(pipes);
		pid = fork();
		if (pid == 0)
		{
			if (i != 0)
				dup2(fd_in, STDIN_FILENO);
			if ((i + 1) != command_count)
				dup2(pipes[1], STDOUT_FILENO);
			close(pipes[0]);
			close(pipes[1]);
			close(fd_in);
			ft_execute(command, head);
			free_all(head, 2);
			exit(EXIT_STATUS);
		}
		dup2(pipes[0], fd_in);
		close(pipes[0]);
		close(pipes[1]);
		command = command->next;
		i++;
	}
}
