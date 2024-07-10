/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 20:23:12 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/08 20:23:12 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exec_command(t_commands *command, t_commands *head)
{
	pid_t	pid;
	char	*content;
	char	**arr;
	char	**arr_command;
	char	**arr_env;

	content = command->token->content;
	pid = fork();
	if (pid == 0)
	{
		arr = get_path(command->env);
		arr_command = ft_lst_to_arr(command->token);
		arr_env = ft_env_to_arr(command->env);
		if (arr)
			execute_with_path(arr, content, arr_command, arr_env);
		free_all_memory(arr_env, arr, arr_command);
		check_permissions(head, content);
	}
	else
	{
		waitpid(pid, &g_exit_status, 0);
		exec_exit_status(g_exit_status);
	}
}

void	exec_exit_status(int status)
{
	int	exit_status;

	exit_status = 0;
	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		g_exit_status = exit_status;
	}
	else if (WIFSIGNALED(status))
	{
		exit_status = WTERMSIG(status);
		if (exit_status == 3)
			printf("Quit (core dumped)");
		printf("\n");
		g_exit_status = 128 + exit_status;
	}
}
