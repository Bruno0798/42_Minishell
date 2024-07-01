/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:25:21 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/05/20 14:58:12 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int check_unset(t_token *head);

void ft_unset(t_commands *command)
{
	t_env *current;
	t_env *previous;

	current = command->env;
	previous = NULL;

	if (command->token->next != NULL)
	{
		if (command->token->next->content[0] == '-' || !check_unset(command->token))
		{
			dup2(STDERR_FILENO, STDOUT_FILENO);
			printf("minishell: unset: %s: invalid option\n", command->token->next->content);
			return;
		}
	}

	while (current != NULL && command->token->next != NULL)
	{
		if (!ft_strcmp(current->key, command->token->next->content))
		{
			if (previous != NULL)
				previous->next = current->next; // Link previous node to the next node
			else
				command->env = current->next; // Update the head of the list

			free(current->key);
			free(current->value);
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}



int check_unset(t_token *head)
{
	t_token *current = head;

	while (current != NULL)
	{
		char *cmd = current->content;
		int i = 0;
		if (ft_isdigit(cmd[i]) != 1)
			return 1;
		while (cmd[i])
		{
			if (ft_isalnum(cmd[i]) == 0 && cmd[i] != '_')
				return 1;
			i++;
		}
		current = current->next;
	}
	return 0;
}
