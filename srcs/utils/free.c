/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 16:49:13 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 13:20:14 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	free_tokens(t_token *token)
{
	t_token	*current;
	t_token	*next;

	current = token;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}

void	ft_free_commands(t_commands *command, int option)
{
	t_commands	*current;
	t_commands	*next;

	current = command;
	if (option >= 2)
		free_env(current->env);
	while (current)
	{
		next = current->next;
		if (option >= 0)
			free_tokens(current->token);
		if (option >= 1)
			free(current);
		current = next;
	}
	if (option == 3)
		exit(g_exit_status);
}
