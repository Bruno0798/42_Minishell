/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:46:18 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/05/21 14:46:18 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_commands	*delete_redir(t_commands *command)
{
	t_token	*curr;
	t_token	*prev;

	curr = command->token;
	prev = NULL;
	while (curr)
	{
		if (curr->type == redir_in2)
		{
			if (prev)
				prev->next = curr->next->next;
			else
				command->token = curr->next->next;
			free(curr->content);
			free(curr->next->content);
			free(curr->next);
			free(curr);
			return (command);
		}
		prev = curr;
		curr = curr->next;
	}
	return (command);
}
