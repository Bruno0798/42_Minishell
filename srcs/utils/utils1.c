/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 10:29:46 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/15 16:52:39 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void printLinkedList(t_env *head) 
{
    t_env *current = head;

    while (current != NULL) {
        printf("Key: %s, Value: %s\n", current->key, current->value);
        current = current->next;
    }
}

char *ft_search_key(t_env *env, char *search)
{
	t_env *current = env;

	while(current != NULL)
	{
		if (!ft_strcmp(current->key, search))
			return (current->value);
		current = current->next;
	}
	return (NULL);
} 

bool is_space(char c)
{
	if (c == 32)
		return true;
	return false;
}

int find_next_space(char *str)
{
	int i = 0;

	while(str[i] && str[i] != 0)
		i++;
	return i;
	
}

void ft_token_list(t_shell shell, char *input)
{
	int i = -1;
	
	t_token token;
	
	while(input[++i])
	{
		if(is_space(input[i]))
			continue;
		if (input[i] == '|')
		{
			token.content = input[i];
			token.type = pipes;
		}
		token.content = ft_substr(&input[i], 0, find_next_space(&input[i]));
		i += find_next_space(input[i]);
		
	}
}
