/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:45:32 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/27 12:12:43 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*expand_value(t_commands *commands, t_token *curr, char *string);

void	ft_expand_others(t_commands *commands)
{
	t_token	*curr;
	char	*str;

	curr = commands->token;
	while (curr)
	{
		str = curr->content;
		str = expand_value(commands, curr, str);
		if (curr->content != str)
		{
			free(curr->content);
			curr->content = str;
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

static char	*expand_value(t_commands *commands, t_token *curr, char	*str)
{
	int		i;

	i = -1;
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
	return (str);
}

bool	is_char_between_quotes(const char *str, char target)
{
	int	s_quote;
	int	d_quote;
	int	i;

	i = 0;
	s_quote = 0;
	d_quote = 0;
	while (str[i])
	{
		if (str[i] == '"' && !s_quote)
			d_quote = !d_quote;
		else if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
		if (str[i] == target)
		{
			if (d_quote || s_quote)
				return (true);
		}
		i++;
	}
	return (false);
}
