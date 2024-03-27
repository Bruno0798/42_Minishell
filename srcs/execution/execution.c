/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 14:23:17 by brunolopes        #+#    #+#             */
/*   Updated: 2024/03/24 17:48:40 by brpereir         ###   ########.fr       */
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
		print_error("command not found", command->token->content, 127);
	}
	waitpid(pid, NULL, 0);  /* Wait for the child process to terminate */
	return (1);  /* Return 1 indicating successful execution */
}

// void ft_handles_redirect(t_commands *command)
// {
// 	ft_redirect(command);
// 	if(!command->redir_fd)
// 		return;
// 	dup2(command->redir_fd, 1);
// }

void ft_handle_redirect(int fd, t_commands *command)
{
	t_token *temp;
	int		old_fd;

	if(!fd)
		return ;
	old_fd = dup(1);
	dup2(fd, 1);
	temp = command->token;
	while(temp)
	{
		if(temp->next && (temp->next->type == redir_out || temp->next->type == redir_out2))
			temp->next = NULL;
		temp = temp->next;
	}
	ft_execute(command);
	dup2(old_fd, 1);
}

int ft_check_redirect(t_commands *command)
{
	t_token *temp;
	int count;
	int	fd;

	fd = 0;
	count = ft_count_redirects(command);
	if (!count)
		return 0;
	temp = command->token;
	while(count)
	{
		if(temp->type == redir_out || temp->type == redir_out2){
			count--;
			if(count == 0 && temp->type == redir_out)
				fd = open(temp->next->content, O_CREAT | O_RDWR | O_TRUNC, 0644);
			else if (count == 0 && temp->type == redir_out2)
				fd = open(temp->next->content, O_CREAT | O_RDWR | O_APPEND, 0644);
			else
				open(temp->next->content, O_CREAT | O_WRONLY, 0644);
		}
		temp = temp->next;
	}
	return (fd);
}

int ft_count_redirects(t_commands *commands)
{
	int count;
	t_token *temp;

	count = 0;
	temp = commands->token;
	while(temp)
	{
		if(temp->type == redir_out || temp->type == redir_out2)
			count++;
		temp = temp->next;
	}

	return count;
}

