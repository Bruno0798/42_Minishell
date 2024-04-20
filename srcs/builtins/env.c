/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:30:08 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/04/20 02:02:27 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_print_env(t_commands *command)
{
	t_env *current;

	current = command->env;

	if (command->token->next != NULL)
	{
		if (ft_strchr(command->token->next->content,'-') != NULL)
		{
			dup2(STDERR_FILENO,STDOUT_FILENO);
			print_error(ERROR_OPTIONS, command->token->next->content, 1);
			return;
		}
		dup2(STDERR_FILENO,STDOUT_FILENO);
		printf("env '%s': no such file or directory\n", command->token->next->content);
		return ;
	}
	while(current)
	{
		if(current->visible)
			printf("%s=%s\n", current->key, current->value); /* Print the environment variable */
		current = current->next;
	}
}
