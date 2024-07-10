/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:30:15 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/09 16:30:17 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_handle_redirect(t_commands *commands)
{
	t_token	*token;
	t_token	*temp;
	t_token	*head;

	head = NULL;
	token = NULL;
	temp = commands->token;
	while (temp)
	{
		if (temp->type == command || temp->type == option)
		{
			token = malloc(sizeof(t_token));
			token->content = ft_strdup(temp->content);
			token->type = temp->type;
			token->next = NULL;
			ft_token_addback(&head, token);
		}
		temp = temp->next;
	}
	free_tokens(commands->token);
	commands->token = head;
}
