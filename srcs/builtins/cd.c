/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:11:46 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/05/20 19:43:30 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_cd(t_commands *commands)
{
	char dir[200]; /* 200 is the maximum size of a path in linux */

	getcwd(dir, sizeof(dir)); /* Get the current working directory */
	if(commands->token->next && commands->token->next->next)
	{
		if(commands->token->next->type == command)
		{
			print_error(ERROR_ARG, NULL, 1);
			return ;
		}
	}
	if (!can_execute(commands)) /* Check if the command can be executed */
	{
		if(!commands->token->next || !ft_strcmp(commands->token->next->content, "~")){
			printf("cd: HOME not set\n"); /* Print error message */
			return ;
		}
		printf("cd: no such file or directory: %s\n", commands->token->next->content); /* Print error message */
		return ;
	}
	ft_update_env(commands->env, "OLDPWD", dir, 1); /* Update the OLDPWD environment variable */
	getcwd(dir, sizeof(dir)); /* Get the current working directory */
	ft_update_env(commands->env, "PWD", dir, 1); /* Update the PWD environment variable */
}

int can_execute(t_commands *command)
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
		else
			return (!chdir(command->token->next->content)); /* Change the working directory to the argument */
	}
	else
	 	return (!chdir(ft_get_value(command->env, "HOME"))); /* Change the working directory to the home directory */
}
