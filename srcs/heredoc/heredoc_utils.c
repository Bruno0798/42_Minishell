/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:32:16 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/09 16:32:18 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	has_here_doc(t_commands *commands)
{
	t_token	*curr;

	curr = commands->token;
	while (curr)
	{
		if (curr->type == redir_in2)
			return (true);
		curr = curr->next;
	}
	return (false);
}

void	remake_commands(t_commands *command)
{
	t_commands	*temp;

	temp = command;
	heredoc_files(temp, 1);
	heredoc_files(temp, 2);
}
