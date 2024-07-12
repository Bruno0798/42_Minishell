/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 15:20:22 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 15:20:32 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup(t_commands *command, char *str, char **words)
{
	if (command)
		free(command);
	if (str)
		free(str);
	if (words)
		free(words);
}

void	set_null(t_commands **commands,
		t_token **head, t_token **current, char ***words)
{
	*commands = NULL;
	*head = NULL;
	*current = NULL;
	*words = NULL;
}
