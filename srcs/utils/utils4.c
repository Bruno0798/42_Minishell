/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 15:20:22 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/23 14:18:28 by bsousa-d         ###   ########.fr       */
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

bool	ft_has_letters(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}
