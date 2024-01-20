/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 02:12:30 by brunolopes        #+#    #+#             */
/*   Updated: 2024/01/19 11:50:59 by bsousa-d         ###   ########.fr       */
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
				ft_printf("%c", token->content[i]);
		}
		if(token->next)
			ft_printf(" ");
		token = token->next;
	}
	if (!flag)
		ft_printf("\n");
}
