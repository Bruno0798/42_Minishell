/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:11:46 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/02 11:12:33 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_cd(t_commands *commands)
{
	char	dir[200];
	char	pwd[4096];

	getcwd(dir, sizeof(dir));
	if (commands->token->next && commands->token->next->next)
	{
		if (commands->token->next->type == command)
		{
			ft_fprintf(2, ERROR_ARG, NULL, 1);
			g_exit_status = 1;
			return ;
		}
	}
	if (!can_execute(commands))
	{
		if (!commands->token->next || !ft_strcmp(commands->token->next->content, "~"))
		{
			if (commands->token->next != NULL && !ft_strcmp(commands->token->next->content, "~"))
				chdir(extract_home_path(getcwd(pwd, sizeof(pwd))));
			else
				printf("cd: HOME not set\n");
			return ;
		}
		if (commands->token->next->content[0] == '-')
		{
			printf("bash: cd: %s: invalid option\n", commands->token->next->content);
			g_exit_status = 1;
			return ;
		}
		ft_fprintf(2, " No such file or directory");
		g_exit_status = 1;
		return ;
	}
	ft_update_env(commands->env, "OLDPWD", dir, 1);
	getcwd(dir, sizeof(dir));
	ft_update_env(commands->env, "PWD", dir, 1);
}

bool	can_execute(t_commands *command)
{
	char	*old_pwd;

	old_pwd = ft_get_value(command->env, "OLDPWD");
	if (command->token->next)
	{
		if (ft_strlen(command->token->next->content) == 1 && command->token->next->content[0] == '-')
		{
			printf("%s\n", old_pwd);
			return (!chdir(old_pwd));
		}
		if (command->token->next->content[0] == '-' && command->token->next->content[1] == '-')
			return (!chdir(ft_get_value(command->env, "HOME")));
		return (!chdir(command->token->next->content));
	}
	if (ft_get_value(command->env, "HOME"))
		return (!chdir(ft_get_value(command->env, "HOME")));
	return (0);
}
