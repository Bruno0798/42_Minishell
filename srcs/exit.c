/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:38:25 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/06/28 18:56:20 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isnum(char *s);

void	ft_exit(t_commands *command, t_commands *head)
{
	if (command->token->next)
	{
		if (command->token->next->next)
		{
			ft_fprintf(2, " too many arguments");
			g_exit_status = 1;
		}
		else if (command->token->next->content && ft_isnum(command->token->next->content))
		{
			if (command->token->next->content[0] == '-')
				g_exit_status = 256 - ft_atoi(&(command->token->next->content[0]) + 1);
			else
				g_exit_status = ft_atoi(command->token->next->content);
		}
		else
		{
			ft_fprintf(2, " numeric argument required");
			g_exit_status = 2;
		}
	}
	else
		g_exit_status = 0;
	free_all(head, 2);
	exit(g_exit_status);
}

void	print_error(char *msg, char *key, int exit_code)
{
	if (key)
	{
		if (msg)
			ft_fprintf(2,"%s: %s: %s\n", ERROR_TITLE, key, msg);
		else
			ft_fprintf(2,"%s: %s `%s'\n", ERROR_TITLE, ERROR_SYNTAX, key);
	}
	else
		ft_fprintf(2,"%s: %s\n", ERROR_TITLE, msg);
	g_exit_status = exit_code;
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
