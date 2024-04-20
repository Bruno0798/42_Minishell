/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:55:13 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/04/20 01:58:19 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_signals(void)
{
	signal(SIGINT, ft_handler_ctrl);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_signals_heredoc(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_handler_ctrl(int signum)
{
	if (signum != SIGINT)
		return ;
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
	EXIT_STATUS = 130;
}

void handle_signals(void)
{
	signal(SIGINT, ft_handler_ctrl);
	signal(SIGQUIT, ft_handler_ctrl);
}
