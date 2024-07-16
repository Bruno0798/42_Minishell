/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 02:12:30 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/04 19:12:48 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*while_option(t_token *token);

void	print_token_content(t_token *token)
{
	int	i;

	while (token)
	{
		i = -1;
		while (token->content[++i])
			printf("%c", token->content[i]);
		if (token->next)
			printf(" ");
		token = token->next;
	}
}

t_token	*process_options(t_token *token, int *flag)
{
	if (token->type == option)
	{
		token = while_option(token);
		*flag = 1;
	}
	return (token);
}

void	ft_echo(t_token *token)
{
	int	flag;

	flag = 0;
	if (!token)
	{
		printf("\n");
		return ;
	}
	token = process_options(token, &flag);
	print_token_content(token);
	if (!flag)
		printf("\n");
	g_exit_status = 0;
}

t_token	*while_option(t_token *token)
{
	while (token && token->type == option)
		token = token->next;
	return (token);
}
