/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:13:00 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/05/27 18:44:07 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_exec_abs(t_commands *command, t_commands *head);
void ft_exec_command(t_commands *command, t_commands *head);

void ft_execute(t_commands *command, t_commands *head)
{
	if(ft_check_redirect(command))
	{
		ft_handle_redirect(command);
		if (!(ft_strcmp(command->token->content,"pwd")))
			ft_pwd(command);
		else if (!(ft_strcmp(command->token->content,"echo")))
			ft_echo(command->token->next);
		else if (!(ft_strcmp(command->token->content,"cd")))
			ft_cd(command);
		else if (!(ft_strcmp(command->token->content,"env")))
			ft_print_env(command);
		else if (!(ft_strcmp(command->token->content,"unset")))
			ft_unset(command);
		else if (!(ft_strcmp(command->token->content,"export")))
			ft_export(command);
		else if (!(ft_strcmp(command->token->content,"exit")))
			ft_exit(command);
		else
			ft_execution(command, head);
	}
}

int ft_execution(t_commands *command, t_commands *head)
{
	if (command->token->content[0] == '/' || !ft_strncmp(command->token->content, "./", 2))
        ft_exec_abs(command, head);
	else 
		ft_exec_command(command, head);
	return (1); 
}

void ft_exec_abs(t_commands *command, t_commands *head)
{
	pid_t	pid;  /* Process ID */
	char	**arr_command;  /* Array to hold the command's tokens */
	char	**arr_env;  /* Array to hold the command's environment variables */


	arr_command = ft_lst_to_arr(command->token);  /* Convert the command's tokens into an array */
	arr_env = ft_env_to_arr(command->env);  /* Convert the command's environment variables into an array */
	pid = fork();  /* Create a child process */
	if (pid == 0)  /* If in child process */
		if (execve(command->token->content, arr_command, arr_env) == -1)
		{
			print_error("command not found", command->token->content, 127);
			EXIT_STATUS = 127;
			free_double_pointer_array(arr_env);
			free_double_pointer_array(arr_command);
			free_all(head, 2);
			exit(127);
		}
	free_double_pointer_array(arr_env);
	free_double_pointer_array(arr_command);
	waitpid(pid, NULL, 0);  /* Wait for the child process to terminate */
}

void ft_exec_command(t_commands *command, t_commands *head)
{
	pid_t   pid;    /* Process ID */
	char    **arr;  /* Array to hold the system's PATH environment variable */
	char    **arr_command;  /* Array to hold the command's tokens */
	char    **arr_env;  /* Array to hold the command's environment variables */
	int     i;  /* Loop counter */
	char *temp_arr;

	arr = get_path(command->env);  /* Retrieve the system's PATH environment variable */
	if (!arr)
	{
		printf("no such file or directory\n"); //MUDAR FRASE e provavlemente o fd tambem=
		return ;
	}

	arr_command = ft_lst_to_arr(command->token);  /* Convert the command's tokens into an array */
	arr_env = ft_env_to_arr(command->env);  /* Convert the command's environment variables into an array */
	i = -1;
	pid = fork();  /* Create a child process */
	if(pid == 0)  /* If in child process */
	{
		while(arr[++i])  /* Iterate over each element in the PATH array */
		{

			temp_arr = ft_strjoin(arr[i], "/");  /* Append a slash to the element */
			free(arr[i]);
			arr[i] = ft_strjoin(temp_arr, command->token->content); /* Append the command's content to the element */
			free(temp_arr);
			if(!access(arr[i], F_OK | X_OK))  /* If the resulting string corresponds to an executable file */
				execve(arr[i], arr_command, arr_env);  /* Execute the file */
		}
		dup2(STDERR_FILENO, STDOUT_FILENO);
		free_double_pointer_array(arr_env);
		free_double_pointer_array(arr);
		free_double_pointer_array(arr_command);
		free_all(head, 2);
		print_error("command not found", command->token->content, 127);
		exit(127);
	}
	// Parent process
	free_double_pointer_array(arr_env);
	free_double_pointer_array(arr);
	free_double_pointer_array(arr_command);
	// Free arr_command after execve
	waitpid(pid, NULL, 0);  /* Wait for the child process to terminate */
}
