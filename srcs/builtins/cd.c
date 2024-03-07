/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 19:25:00 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/23 18:17:56 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_cd(t_commands *command)
{
	char dir[200]; /* 200 is the maximum size of a path in linux */
	
	getcwd(dir, sizeof(dir)); /* Get the current working directory */
	if (!can_execute(command)) /* Check if the command can be executed */
	{
		printf("cd: no such file or directory: %s\n", command->token->next->content); /* Print error message */
		return ;
	}
	ft_update_env(command->env, "OLDPWD", dir); /* Update the OLDPWD environment variable */
	getcwd(dir, sizeof(dir)); /* Get the current working directory */
	ft_update_env(command->env, "PWD", dir); /* Update the PWD environment variable */
}

bool can_execute(t_commands *command)
{
	char *old_pwd; /* Variable to store the old working directory */
	
	old_pwd = ft_get_value(command->env, "OLDPWD"); /* Get the value of the OLDPWD environment variable */
	if (command->token->next)
	{
		if (ft_strlen(command->token->next->content) == 1 && command->token->next->content[0] == '-') /* Check if the argument is '-' */
		{
			printf("%s\n", old_pwd); /* Print the old working directory */
			return (!chdir(old_pwd)); /* Change the working directory to the old working directory */
		}
		else if (command->token->next->content[0] == '-' && command->token->next->content[1] == '-') /* Check if the argument is '--' */
			return (!chdir(ft_get_value(command->env, "HOME"))); /* Change the working directory to the home directory */
		else if (ft_strlen(command->token->next->content) == 1 && command->token->next->content[0] == '~') /* Check if the argument is '~' */
			return (!chdir((ft_get_value(command->env, "HOME")))); /* Change the working directory to the home directory */
		else
			return (!chdir(command->token->next->content)); /* Change the working directory to the argument */
	}
	else
	 	return (!chdir(ft_get_value(command->env, "HOME"))); /* Change the working directory to the home directory */
}
