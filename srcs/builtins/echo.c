/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 02:12:30 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/04/09 14:40:32 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Function Prototypes */
static t_token *while_option(t_token *token);

void ft_echo(t_token *token)
{
	int	i;
	int flag = 0;

	if (!token) /* If there is no token to print, return */
	{
		printf("\n");
		return ;
	}
	if (token->type == option) /* If the first token is an option, skip it */
	{
		token = while_option(token); /* Skip while token is equal to -n */
		flag = 1; /* Set flag to 1 */
	}
	while(token)
	{
		i = -1; 
		while(token->content[++i])
				printf("%c", token->content[i]); /* Print the token content */
		if(token->next)
			printf(" "); /* Print a space if the token is not the last one */
		token = token->next;
	}
	if (!flag) /* If the flag is not set, print a new line */
		printf("\n");
}

t_token *while_option(t_token *token)
{
	while(token && token->type == option) /* While token is equal to -n */
		token = token->next;
	return (token);
}
