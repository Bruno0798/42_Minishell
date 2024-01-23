/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 19:25:00 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/23 12:24:30 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_cd(t_commands *command)
{
	char dir[200];
	
	getcwd(dir, sizeof(dir));
	if (!can_execute(command))
	{
		ft_printf("cd: no such file or directory: %s\n", command->token->next->content);
		return ;
	}
	ft_update_env(command->env, "OLDPWD", dir);
	getcwd(dir, sizeof(dir));
	ft_update_env(command->env, "PWD", dir);
}

bool can_execute(t_commands *command)
{
	char *old_pwd;
	
	old_pwd = ft_get_value(command->env, "OLDPWD");
	if (command->token->next)
	{
		if (ft_strlen(command->token->next->content) == 1 && command->token->next->content[0] == '-')
		{
			ft_printf("%s\n", old_pwd);
			return (!chdir(old_pwd));
		}
		else if (command->token->next->content[0] == '-' && command->token->next->content[1] == '-')
			return (!chdir(ft_get_value(command->env, "HOME")));
		else if (ft_strlen(command->token->next->content) == 1 && command->token->next->content[0] == '~')
			return (!chdir((ft_get_value(command->env, "HOME"))));
		else
			return (!chdir(command->token->next->content));
	}
	else
	 	return (!chdir(ft_get_value(command->env, "HOME")));
}
