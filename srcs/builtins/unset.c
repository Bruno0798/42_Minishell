/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:25:21 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/30 18:31:30 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_invalid_unset_command(t_commands *command)
{
	if (!command || !command->token)
		return (true);
	if (command->token->next
		&& ft_strcmp(command->token->next->content, "_") == 0)
		return (true);
	if (command->token->next
		&& ft_strcmp(command->token->next->content, "-") == 0)
		return (false);
	if (command->token->next && (command->token->next->content[0] == '-'
			|| !check_unset(command->token)))
	{
		print_error(ERROR_OPTIONS, NULL, 2);
		return (true);
	}
	return (false);
}

void	remove_env_node(t_commands *command, t_env *previous, t_env *current)
{
	if (previous != NULL)
		previous->next = current->next;
	else
		command->env = current->next;
	free(current->key);
	free(current->value);
	free(current);
}

void	ft_unset(t_commands *command)
{
	t_env	*current;
	t_env	*previous;

	if (is_invalid_unset_command(command))
		return ;
	current = command->env;
	previous = NULL;
	while (current && command->token->next)
	{
		if (!ft_strcmp(current->key, command->token->next->content))
		{
			remove_env_node(command, previous, current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

int	check_unset(t_token *head)
{
	int		i;
	t_token	*current;
	char	*cmd;

	if (!head)
		return (1);
	current = head;
	while (current)
	{
		cmd = current->content;
		i = 0;
		if (!ft_isdigit(cmd[i]))
			return (1);
		while (cmd[i])
		{
			if (!ft_isalnum(cmd[i]) && cmd[i] != '_')
				return (1);
			i++;
		}
		current = current->next;
	}
	return (0);
}
