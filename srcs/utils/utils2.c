/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:21:20 by brpereir          #+#    #+#             */
/*   Updated: 2024/07/12 06:56:39 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_token_back(t_token **head, t_token *node)
{
	t_token	*token;

	token = *head;
	if (!*head)
	{
		*head = node;
		return ;
	}
	while (token->next)
		token = token->next;
	token->next = node;
}

bool	redir_valid(t_token *token)
{
	int	i;
	int	out;
	int	in;

	i = -1;
	while (token && token->content[++i])
		if (token->content[i] == '<' || token->content[i] == '>')
		{
			in = 0;
			out = 0;
			while (token->content[i] && (ft_strchr("<>", token->content[i])))
			{
				if (token->content[i] == '<' && !out)
					in++;
				else if (token->content[i] == '>' && !in)
					out++;
				else
					return (false);
				if (in > 2 || out > 2)
					return (false);
				i++;
			}
			if (!token->content[i])
				break ;
		}
	return (true);
}

char	*ft_strpbrk(char *s, char *delims)
{
	char	*d;
	int		s_quote;
	int		d_quote;
	int		i;

	s_quote = 0;
	d_quote = 0;
	i = -1;
	while (s[++i])
	{
		if (s[i] == DOUBLE_QUOTE && !s_quote)
			d_quote = !d_quote;
		if (s[i] == SINGLE_QUOTE && !d_quote)
			s_quote = !s_quote;
		d = delims;
		while (*d)
		{
			if (s[i] == *d && !s_quote && !d_quote)
				return (s + i);
			d++;
		}
	}
	return (NULL);
}

t_token	*split_string(char *content)
{
	char	*ptr;
	char	*next;
	t_token	*main;
	t_token	*token;

	ptr = content;
	main = NULL;
	while (ptr && ft_strpbrk(ptr, "<>"))
	{
		next = ft_strpbrk(ptr, "<>");
		if (next[1] == '<' || next[1] == '>')
			token = ft_new_token(ft_substr(ptr, 0, 2), 0);
		else
			token = ft_new_token(ft_substr(ptr, 0, 1), 0);
		ft_token_back(&main, token);
		ptr = next + ft_strlen(token->content);
		if (!*ptr)
			return (main);
		next = ft_strpbrk(ptr, "<>");
		if (!next)
			token = ft_new_token(ft_strdup(ptr), 0);
		else
			token = ft_new_token(ft_substr(ptr, 0, next - ptr), 0);
		ft_token_back(&main, token);
		ptr = ptr + ft_strlen(token->content);
	}
	return (main);
}

t_token	*check_redir_syntax(t_token *head)
{
	char	*next;
	char	*content;
	t_token	*token;

	content = head->content;
	token = NULL;
	if (head->type == error)
		return (head);
	if (!ft_strchr("<>", content[0]))
	{
		next = ft_strpbrk(content, "<>");
		token = ft_new_token(ft_substr(content, 0, next - content), 0);
		content = content + ft_strlen(token->content);
	}
	ft_token_addback(&token, split_string(content));
	free_tokens(head);
	return (token);
}
