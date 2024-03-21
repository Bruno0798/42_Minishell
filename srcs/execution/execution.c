/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:23:17 by brunolopes        #+#    #+#             */
/*   Updated: 2024/03/20 16:12:13 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_execution(t_commands *command)
{
	pid_t	pid;  /* Process ID */
	char	**arr;  /* Array to hold the system's PATH environment variable */
	char	**arr_command;  /* Array to hold the command's tokens */
	char	**arr_env;  /* Array to hold the command's environment variables */
	int		i;  /* Loop counter */

	arr = get_path(command->env);  /* Retrieve the system's PATH environment variable */
	arr_command = ft_lst_to_arr(command->token);  /* Convert the command's tokens into an array */
	arr_env = ft_env_to_arr(command->env);  /* Convert the command's environment variables into an array */
	i = -1;  /* Initialize loop counter */
	pid = fork();  /* Create a child process */
	if(pid == 0)  /* If in child process */
	{
		while(arr[++i])  /* Iterate over each element in the PATH array */
		{
			arr[i] = ft_strjoin(arr[i], "/");  /* Append a slash to the element */
			arr[i] = ft_strjoin(arr[i], command->token->content);  /* Append the command's content to the element */
			if(!access(arr[i], F_OK | X_OK))  /* If the resulting string corresponds to an executable file */
				execve(arr[i], arr_command, arr_env);  /* Execute the file */
		}
		
	}
	waitpid(pid, NULL, 0);  /* Wait for the child process to terminate */
	return (1);  /* Return 1 indicating successful execution */
}

void ft_handle_redirect(t_commands *command)
{
	ft_redirect(command);
	if(!command->redir_fd)
		return;
	dup2(command->redir_fd, 1);
}

