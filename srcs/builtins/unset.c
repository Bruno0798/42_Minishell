/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:25:21 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/23 12:41:52 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_unset(t_commands *command)
{
	t_env *current;
	t_env *previous;

	current = command->env;
	previous = NULL;
	while(current)
	{
		if (!ft_strcmp(current->key, command->token->next->content))
		{
			if (previous)
				previous->next = current->next;
			else
				command->env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}
