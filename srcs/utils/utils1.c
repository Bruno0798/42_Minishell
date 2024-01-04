/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 10:29:46 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/04 15:02:00 by bsousa-d         ###   ########.fr       */
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