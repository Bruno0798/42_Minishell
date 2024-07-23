/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 15:20:22 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/23 19:59:45 by brpereir         ###   ########.fr       */
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

int	ft_check_option(char *str)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = ft_strdup(str);
	tmp = ft_delete_quotes(tmp);
	if (tmp[0] != '-' || ft_strlen(tmp) == 1)
	{
		free(tmp);
		return (false);
	}
	while (tmp[++i])
	{
		if (tmp[i] != 'n')
		{
			free(tmp);
			return (false);
		}
	}
	free(tmp);
	return (true);
}
