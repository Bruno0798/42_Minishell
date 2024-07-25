/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:25:21 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/25 20:41:48 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	remove_env_node(t_commands *command, t_env *previous, t_env *current)
{
	if (previous != NULL)
		previous->next = current->next;
	else
		command->env = current->next;
}

void	ft_unset(t_commands *command)
{
	t_env	*current;
	t_env	*previous;

	current = command->env;
	previous = NULL;
	if (!command || !command->token)
		return ;
	if (command->token->next && command->token->next->content[0] == '_')
		return ;
	if (command->token->next && (command->token->next->content[0] == '-'
			|| !check_unset(command->token)))
	{
		print_error(ERROR_OPTIONS, NULL, 2);
		return ;
	}
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
		if (!ft_isdigit((unsigned char)cmd[i]))
			return (1);
		while (cmd[i])
		{
			if (!ft_isalnum((unsigned char)cmd[i]) && cmd[i] != '_')
				return (1);
			i++;
		}
		current = current->next;
	}
	return (0);
}
