/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:13:00 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/29 18:03:17 by bsousa-d         ###   ########.fr       */
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
	if (command->token->content[0] == '/'
		|| !ft_strncmp(command->token->content, "./", 2))
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
	if (ft_strcmp(command->token->content, "./minishell") == 0)
		ft_handle_signals(IGNORE);
	else
		ft_handle_signals(CHILD);
	pid = fork();
	if (pid == 0)
	{
		execve(command->token->content, arr_command, arr_env);
		free_double_pointer_array(arr_env);
		free_double_pointer_array(arr_command);
		check_permissions(head, command->token->content);
	}
	free_double_pointer_array(arr_env);
	free_double_pointer_array(arr_command);
	waitpid(pid, &g_exit_status, 0);
	exec_exit_status(g_exit_status);
}

void	free_all_memory(char **arr_env, char **arr, char **arr_command)
{
	free_double_pointer_array(arr_env);
	free_double_pointer_array(arr);
	free_double_pointer_array(arr_command);
}

void	execute_with_path(char **path_arr, char *token_content,
		char **arr_command, char **arr_env)
{
	int		i;
	char	*temp_arr;

	i = -1;
	while (path_arr[++i])
	{
		temp_arr = ft_strjoin(path_arr[i], "/");
		free(path_arr[i]);
		path_arr[i] = ft_strjoin(temp_arr, token_content);
		free(temp_arr);
		execve(path_arr[i], arr_command, arr_env);
	}
}
