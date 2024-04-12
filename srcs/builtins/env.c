/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:30:08 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/04/10 18:08:02 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:30:52 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/23 18:19:02 by brunolopes       ###   ########.fr       */
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
		printf("%s=%s\n", current->key, current->value); /* Print the environment variable */
		current = current->next;
	}
}
