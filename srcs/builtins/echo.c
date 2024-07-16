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
	while (ft_strncmp(token->content, "-n", 2) == 0)
	{
		token = token->next;
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