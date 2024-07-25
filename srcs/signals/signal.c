/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:55:13 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/25 20:19:43 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	main_signal(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		g_exit_status = 130;
	}
}

void	fork_signal(int signum)
{
	if (signum == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	else if (signum == SIGQUIT)
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
}

void	pipe_signal(int signum)
{
	(void)signum;
	close(STDIN_FILENO);
}

void	init_pipe_signal(void)
{
	signal(SIGINT, pipe_signal);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_handle_signals(int id)
{
	if (id == MAIN)
	{
		signal(SIGINT, main_signal);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (id == CHILD)
	{
		signal(SIGINT, fork_signal);
		signal(SIGQUIT, fork_signal);
		signal(SIGPIPE, pipe_signal);
	}
	else if (id == HERE_DOC)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (id == PIPE)
	{
		init_pipe_signal();
	}
	else if (id == IGNORE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
}
