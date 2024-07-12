/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:45:32 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/12 13:47:31 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_expand_others(t_commands *commands)
{
	int		i;
	t_token	*curr;
	char	*str;

	curr = commands->token;
	while (curr)
	{
		i = -1;
		str = curr->content;
		while (str[++i])
		{
			if ((str[i] == '~') && (i == 0)
				&& ft_get_value(commands->env, "HOME") && ((str[i + 1] == ' ')
					|| (curr ->content[i + 1] == '\0') || (str[i + 1] == '/')))
				str = ft_strjoin(ft_get_value(commands->env, "HOME"), str + 1);
			else if ((str[i] == '~') && (i == 0) && (str[i + 1] == '+'))
				str = ft_strjoin(ft_get_value(commands->env, "PWD"), str + 2);
			else if ((str[i] == '~') && (i == 0) && (str[i + 1] == '-'))
				str = ft_strjoin(ft_get_value(commands->env, "OLDPWD"),
						str + 2);
		}
		curr = curr->next;
	}
	free (curr);
}

char	*ft_delete_quotes(char *input)
{
	int		i;
	int		j;
	char	*new_str;
	char	quote;

	new_str = malloc(ft_strlen(input) + 1);
	i = -1;
	j = 0;
	while (input[++i])
	{
		if (input[i] != '"' && input[i] != '\'')
			new_str[j++] = input[i];
		else if (input[i] == '"' || input[i] == '\'')
		{
			quote = input[i++];
			while (input[i] != quote)
				new_str[j++] = input[i++];
		}
	}
	new_str[j] = '\0';
	free(input);
	return (new_str);
}
