/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:23:17 by brunolopes        #+#    #+#             */
/*   Updated: 2024/01/23 16:34:20 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_execution(t_commands *command)
{
	pid_t	pid;
	char	**arr;
	char	**arr_command;
	char	**arr_env;
	int		i;

	arr = get_path(command->env);
	arr_command = ft_lst_to_arr(command->token);
	arr_env = ft_env_to_arr(command->env);
	i = -1;
	pid = fork();
	if(pid == 0)
	{
		while(arr[++i])
		{
			arr[i] = ft_strjoin(arr[i], "/");
			arr[i] = ft_strjoin(arr[i], command->token->content);
			if(!access(arr[i], F_OK | X_OK))
				execve(arr[i], arr_command, arr_env);
		}
	}
	waitpid(pid, NULL, 0);
	return (1);
}
