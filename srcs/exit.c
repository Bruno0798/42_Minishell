/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:38:25 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/25 20:30:36 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isnum(char *s);

void	handle_token_content(t_commands *command)
{
	char	*content;
	int		number;

	content = command->token->next->content;
	if (content && ft_isnum(content))
	{
		if (content[0] == '-')
			number = ft_atoi(&(content[1]));
		else
			number = ft_atoi(content);
		if (content[0] == '-')
			g_exit_status = 256 - number;
		else
			g_exit_status = number;
	}
	else
		print_error(ERROR_NUM, NULL, 2);
}

void	handle_token_next(t_commands *command, t_commands *head)
{
	if (!ft_has_letters(command->token->next->content))
	{
		handle_token_content(command);
		free_all(head, 2);
		exit(g_exit_status);
	}
	if (command->token->next->next)
	{
		if (g_exit_status != 0)
			print_error(ERROR_ARG, NULL, g_exit_status);
		else
			print_error(ERROR_ARG, NULL, 1);
	}
	else
	{
		handle_token_content(command);
		free_all(head, 2);
		exit(g_exit_status);
	}
}

void	ft_exit(t_commands *command, t_commands *head)
{
	printf("exit\n");
	if (command->token->next)
	{
		handle_token_next(command, head);
		return ;
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
			ft_fprintf(2, "%s: %s: %s\n", ERROR_TITLE, key, msg);
		else
			ft_fprintf(2, "%s: %s `%s'\n", ERROR_TITLE, ERROR_SYNTAX, key);
	}
	else
		ft_fprintf(2, "%s: %s\n", ERROR_TITLE, msg);
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
