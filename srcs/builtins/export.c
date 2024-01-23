/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:42:09 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/23 17:39:31 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_export(t_commands *command)
{
    t_env *current;
    t_env *head;

    current = command->env;
    head = current;

    while (command->token && command->token->next)
    {
        current = head; // Start from the head on each iteration

        while (current)
        {
            if (!ft_strcmp(current->key, command->token->next->content))
            {
                free(current->value);
                current->value = strdup(command->token->next->next->content);
                printf("key: %s\n", current->key);
                printf("value: %s\n", current->value);
            }

            current = current->next;
        }

        command->token = command->token->next;
    }

    command->env = head; // Restore the original head after modifications
}