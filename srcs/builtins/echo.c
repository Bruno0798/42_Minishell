/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 02:12:30 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/02/22 17:19:37 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	todo: Options | Spaces in the middle | Checking start of text to print
*/

static t_token *while_option(t_token *token);

void ft_echo(t_token *token)
{
	int	i;
	int flag;
	
	flag = 0;
	if (!token) 
		return ;
	if (token->type == option)
	{
		token = while_option(token);
		flag = 1;
	}
	while(token)
	{
		i = -1; 
		while(token->content[++i])
				printf("%c", token->content[i]);
		if(token->next)
			printf(" ");
		token = token->next;
	}
	if (!flag)
		printf("\n");
}

t_token *while_option(t_token *token)
{
	while(token && token->type == option)
		token = token->next;
	return (token);
}
