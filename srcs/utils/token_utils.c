/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 06:57:17 by bruno             #+#    #+#             */
/*   Updated: 2024/07/12 07:55:49 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	bool	check_echo_option(char *str)
{
	int	i;

	i = 0;
	while (str[++i])
	{
		if (str[i] != 'n')
			return (false);
	}
	return (true);
}

static bool	check_redirection(char *str)
{
	int	i;
	int	out;
	int	in;

	i = -1;
	if (ft_strpbrk(str, "<>"))
	{
		in = 0;
		out = 0;
		i = ft_strpbrk(str, "<>") - str;
		while (str[i] && ft_strchr("<>", str[i]))
		{
			if (str[i] == '<' && !out)
				in++;
			else if (str[i] == '>' && !in)
				out++;
			else
				return (false);
			if (in > 2 || out > 2)
				return (false);
			i++;
		}
	}
	return (true);
}

t_type	ft_token_type(char *word)
{
	if (!ft_strcmp(word, "|"))
		return (pipes);
	if (!ft_strcmp(word, ">>"))
		return (redir_out2);
	if (!ft_strcmp(word, ">"))
		return (redir_out);
	if (!ft_strcmp(word, "<<"))
		return (redir_in2);
	if (!ft_strcmp(word, "<"))
		return (redir_in);
	if (!ft_strncmp(word, "-n", 2) && check_echo_option(word))
		return (option);
	if (!check_redirection(word))
		return (error);
	return (command);
}

void	ft_print_token_list(t_token *head)
{
	t_token	*token;

	token = head;
	while (token)
	{
		printf("Token: '%s' \nType: '%d'\n\n", token->content, token->type);
		token = token->next;
	}
}

void	ft_token_addback(t_token **token, t_token *new)
{
	t_token	*temp;

	temp = *token;
	if (token)
	{
		if (*token)
		{
			while (temp->next)
				temp = temp->next;
			temp->next = new;
		}
		else
			*token = new;
	}
}
