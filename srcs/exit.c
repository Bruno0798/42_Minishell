/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:38:25 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/06/19 23:01:35 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isnum(char *s);

void ft_exit(t_commands *command, t_commands *head)
{
	if (command->token->next)
	{
		if (command->token->next && command->token->next->next)
		{
			ft_fprintf(2, " too many arguments");
			EXIT_STATUS = 1;
		}
		else if (command->token->next->content && ft_isnum(command->token->next->content))
		{
			if(command->token->next->content[0] == '-')
				EXIT_STATUS = 256 - ft_atoi(&(command->token->next->content[0]) + 1);
			else
				EXIT_STATUS = ft_atoi(command->token->next->content);
		} 
		else
		{
			ft_fprintf(2, " numeric argument required");
			EXIT_STATUS = 2;
		}
	}
	free_all(head, 2);
	exit(EXIT_STATUS);
}

void	print_error(char *msg, char *key, int exit_code)
{
	if (key)
	{
		if (msg)
			printf("%s: %s: %s\n", ERROR_TITLE, key, msg);
		else
			printf("%s: %s `%s'\n", ERROR_TITLE, ERROR_SYNTAX, key);
	}
	else
		printf("%s: %s\n", ERROR_TITLE, msg);
	EXIT_STATUS = exit_code;
}

int	ft_isnum(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[1] && (s[i] == '-' || s[i] == '+'))
		{
			if (i != 0)
				return (0);
			i++;
		}
		if (ft_isdigit(s[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}