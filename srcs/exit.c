/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:38:25 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/05/21 14:38:43 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void ft_exit(t_commands *command)
{
	free_all(command);
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