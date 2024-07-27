/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 15:20:22 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/27 12:11:37 by brpereir         ###   ########.fr       */
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

int	ft_check(char *str)
{
	int		i;
	char	*value;

	value = ft_strpbrk(str, "<>");
	while (value && ft_strchr("<>", value[0]))
		value += 1;
	while (value)
	{
		i = -1;
		while (value[++i] == 32)
			;
		if (ft_strchr("<>", value[i]))
		{
			print_error(ERROR_REDIR, NULL, 2);
			return (0);
		}
		value = ft_strpbrk(value + 1, "<>");
		while (value && ft_strchr("<>", value[0]))
			value += 1;
	}
	return (1);
}
