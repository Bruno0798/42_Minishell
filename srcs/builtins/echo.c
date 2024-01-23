/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 02:12:30 by brunolopes        #+#    #+#             */
/*   Updated: 2024/01/23 18:17:57 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	todo:
	Options | Spaces in the middle | Checking start of text to print	
*/ 
void ft_echo(t_token *token)
{
	int	i;
	int s_quote;
	int d_quote;
	int flag;
	
	s_quote = 0;
	d_quote = 0;
	flag = 0;
	if(token->type == option) 
	{
		flag = !flag;
		token = token->next;
	}
	while(token)
	{
		i = -1; 
		while(token->content[++i])
		{
			if(token->content[i] == DOUBLE_QUOTE && !s_quote)
				d_quote = !d_quote;
			else if(token->content[i] == SINGLE_QUOTE && !d_quote)
				s_quote = !s_quote;
			else if(token->content[i] == SPACE && !d_quote && !s_quote)
				continue;
			else 
				printf("%c", token->content[i]);
		}
		if(token->next)
			printf(" ");
		token = token->next;
	}
	if (!flag)
		printf("\n");
}
