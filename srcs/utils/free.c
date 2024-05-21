/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 16:49:13 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/05/21 17:19:15 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_all(t_commands *command)
{
	ft_free_commands(command);
	exit(0);
}

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

void	ft_free_commands(t_commands *command)
{
	t_commands	*current;
	t_commands	*next;

	current = command;
	while (current)
	{
		next = current->next;
		free_tokens(current->token);
		free_env(current->env);
		free(current);
		current = next;
	}
}


