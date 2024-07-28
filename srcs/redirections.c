/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:46:18 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/25 18:09:25 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_commands	*delete_redir(t_commands *command)
{
	t_token	*curr;

	curr = command->token;
	while (curr)
	{
		if (curr->type == redir_in2)
		{
			curr->type = redir_in;
			free(curr->next->content);
			curr->next->content = ft_strdup("heredoc.txt");
			return (command);
		}
		curr = curr->next;
	}
	return (command);
}
