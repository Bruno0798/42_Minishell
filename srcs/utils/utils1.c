/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 10:29:46 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/16 16:09:54 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	printLinkedList(t_env *head) 
{
	t_env	*current = head;

	while (current != NULL) {
		printf("Key: %s, Value: %s\n", current->key, current->value);
		current = current->next;
	}
}

char	*ft_search_key(t_env *env, char *search)
{
	t_env	*current = env;

	while (current != NULL)
	{
		if (!ft_strcmp(current->key, search))
			return (current->value);
		current = current->next;
	}
	return (NULL);
} 

bool	is_space(char c)
{
	if (c == 32)
		return true;
	return false;
}

int	find_next_space(char *str)
{
	int	i = 0;

	while (str[i] && str[i] != 0)
		i++;
	return i;
	
}

t_token ft_token_list(char *input)
{
	int i;
	t_token token;
	
	i = -1;
	while(input[++i])
	{
		if(is_space(input[i]))
			continue;
		else if (input[i] == '|')
			token.type = pipes;
		else if (input[i] == '>' && input[i + 1] == '>')
		{
			token.type = redir_out2;
			i++;
		}
		else if (input[i] == '>')
			token.type = redir_out;
		else if (input[i] == '<' && input[i + 1] == '<')
		{
			token.type = redir_in2;
			i++;
		}
		else if(input[i] == '<')
			token.type = redir_in;
		else
		{
			token.content = ft_substr(&input[i], 0, find_next_space(&input[i]));
			i += find_next_space(&input[i]);
		}
		token = *token.next;
		i++;
	}

	return (token);
}

// void create_token(t_token token, char *str)
// {
	
// }

ft_fix_spaces(char **words)
{
	int i;
	int j;
	char **dup;
 

	i = -1;
	j = -1;
	while(words[++i])
	{
		ft_split(words[i], ' ');
	}
}

char **split_pipe(char *str)
{
	char **words;

	if(ft_strchr(str, '|'))
		words = ft_split(str, '|');
	ft_fix_spaces(words);
	return (words ? words : 0);
}

