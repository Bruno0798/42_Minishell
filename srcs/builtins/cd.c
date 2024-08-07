/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:11:46 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/08/02 02:56:09 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_env_vars(t_commands *commands, char *dir, int size)
{
	ft_update_env(commands->env, "OLDPWD", dir, 1);
	getcwd(dir, size);
	ft_update_env(commands->env, "PWD", dir, 1);
}

void	handle_home_directory(t_commands *commands)
{
	if (commands->token->next != NULL
		&& !ft_strcmp(commands->token->next->content, "~")
		&& ft_get_value(commands->env, "HOME"))
		chdir(ft_get_value(commands->env, "HOME"));
	else
		print_error(ERROR_HOME, NULL, 1);
}

void	handle_cd_errors(t_commands *commands)
{
	if (commands->token->next->content[0] == '-')
	{
		print_error(ERROR_OPTIONS, "cd", 2);
		return ;
	}
	print_error(ERROR_DIR, commands->token->next->content, 1);
}

bool	can_execute(t_commands *command)
{
	char	*old_pwd;

	old_pwd = ft_get_value(command->env, "OLDPWD");
	if (command->token->next)
	{
		if (ft_strlen(command->token->next->content) == 1
			&& command->token->next->content[0] == '-')
		{
			printf("%s\n", old_pwd);
			return (!chdir(old_pwd));
		}
		if (command->token->next->content[0] == '-'
			&& command->token->next->content[1] == '-')
			return (!chdir(ft_get_value(command->env, "HOME")));
		return (!chdir(command->token->next->content));
	}
	if (ft_get_value(command->env, "HOME"))
		return (!chdir(ft_get_value(command->env, "HOME")));
	return (0);
}

void	ft_cd(t_commands *commands)
{
	char	dir[200];

	getcwd(dir, sizeof(dir));
	if (commands->token->next && commands->token->next->next)
	{
		if (commands->token->next->type == command)
		{
			print_error(ERROR_ARG, NULL, 1);
			return ;
		}
	}
	if (!can_execute(commands))
	{
		if (!commands->token->next
			|| !ft_strcmp(commands->token->next->content, "~"))
		{
			handle_home_directory(commands);
			return ;
		}
		handle_cd_errors(commands);
		return ;
	}
	update_env_vars(commands, dir, sizeof(dir));
	g_exit_status = 0;
}
