/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 02:12:30 by brunolopes        #+#    #+#             */
/*   Updated: 2024/01/15 15:44:18 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	todo:
	Options | Spaces in the middle | Checking start of text to print
		
*/ 
void ft_echo(char *str)
{
	int	i;
	int s_quote;
	int d_quote;

	i = -1;
	s_quote = 0;
	d_quote = 0;
	while(str[++i])
	{
		if(str[i] == DOUBLE_QUOTE && !s_quote)
			d_quote = !d_quote;
		else if(str[i] == SINGLE_QUOTE && !d_quote)
			s_quote = !s_quote;
		else if(str[i] == SPACE && !d_quote && !s_quote)
			continue;
		else 
			ft_printf("%c", str[i]);
	}
	ft_printf("\n");
}
