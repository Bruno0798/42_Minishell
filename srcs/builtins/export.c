/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:42:09 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/02/26 17:50:01 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_export(t_commands *command)
{
    t_env   *current;
    char    *word;

    if(!command->token->next) /* If there is no token to export, print the environment variables */
    {
        current = command->env;
        while (current)
        {
            printf("declare -x %s=\"%s\"\n", current->key, current->value); /* Print the environment variable */
            current = current->next;
        }
        return ;
    }
    if (ft_fnd_env(command->env, *ft_split(command->token->next->content, '='))) /* If the key already exists, update the value */
    {
        word = ft_strchr(command->token->next->content, '=') + 1; /* Get the value */
        *ft_strchr(command->token->next->content, '=') = 0; /* Set the equal sign to 0 */
        ft_update_env(command->env, command->token->next->content, word); /* Update the environment variable */
        return ;
    }
    if(ft_strchr(command->token->next->content, '=')) /* If the token has an equal sign, add the environment variable */
    {
        word = ft_strchr(command->token->next->content, '=') + 1; /* Get the value */
        *ft_strchr(command->token->next->content, '=') = 0; /* Set the equal sign to 0 */
        ft_add_env_back(&command->env, ft_new_env(command->token->next->content, word)); /* Add the environment variable */
    }
}
