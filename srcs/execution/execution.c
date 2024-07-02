/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:13:00 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/01 13:11:05 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exec_abs(t_commands *command, t_commands *head);
void	ft_exec_command(t_commands *command, t_commands *head);
void	exec_exit_status(int status);

void	ft_execute(t_commands *command, t_commands *head)
{
	if (ft_check_redirect(command))
	{
		ft_handle_redirect(command);
		if (!command->token)
			return ;
		if (is_everything_space(command->token->content))
			return ;
		if (!(ft_strcmp(command->token->content, "pwd")))
			ft_pwd(command);
		else if (!(ft_strcmp(command->token->content, "echo")))
			ft_echo(command->token->next);
		else if (!(ft_strcmp(command->token->content, "cd")))
			ft_cd(command);
		else if (!(ft_strcmp(command->token->content, "env")))
			ft_print_env(command);
		else if (!(ft_strcmp(command->token->content, "unset")))
			ft_unset(command);
		else if (!(ft_strcmp(command->token->content, "export")))
			ft_export(command);
		else if (!(ft_strcmp(command->token->content, "exit")))
			ft_exit(command, head);
		else
			ft_execution(command, head);
	}
}

int	ft_execution(t_commands *command, t_commands *head)
{
	if (command->token->content[0] == '/' || !ft_strncmp(command->token->content, "./", 2))
		ft_exec_abs(command, head);
	else
		ft_exec_command(command, head);
	return (1);
}

void	ft_exec_abs(t_commands *command, t_commands *head)
{
	pid_t	pid;
	char	**arr_command;
	char	**arr_env;

	arr_command = ft_lst_to_arr(command->token);
	arr_env = ft_env_to_arr(command->env);
	pid = fork();
	if (pid == 0)
	{
		execve(command->token->content, arr_command, arr_env);
		check_permissions(head, command->token->content);
		free_double_pointer_array(arr_env);
		free_double_pointer_array(arr_command);
	}
	free_double_pointer_array(arr_env);
	free_double_pointer_array(arr_command);
	waitpid(pid, &g_exit_status, 0);
	exec_exit_status(g_exit_status);
}

void	ft_exec_command(t_commands *command, t_commands *head)
{
	pid_t	pid;
	char	**arr;
	char	**arr_command;
	char	**arr_env;
	int		i;
	char	*temp_arr;

	arr = get_path(command->env);
	arr_command = ft_lst_to_arr(command->token);
	arr_env = ft_env_to_arr(command->env);
	i = -1;
	pid = fork();
	if (pid == 0)
	{
		while (arr[++i])
		{
			temp_arr = ft_strjoin(arr[i], "/");
			free(arr[i]);
			arr[i] = ft_strjoin(temp_arr, command->token->content);
			free(temp_arr);
			execve(arr[i], arr_command, arr_env);
		}
		check_permissions(head, command->token->content);
		free_double_pointer_array(arr_env);
		free_double_pointer_array(arr);
		free_double_pointer_array(arr_command);
	}
	free_double_pointer_array(arr_env);
	free_double_pointer_array(arr);
	free_double_pointer_array(arr_command);
	waitpid(pid, &g_exit_status, 0);
	exec_exit_status(g_exit_status);
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
