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
    t_env   *head;
    char    *word;

    //check if its export alone
    if(!command->token->next)
    {
        current = command->env;
        while (current)
        {
            printf("declare -x %s=\"%s\"\n", current->key, current->value);
            current = current->next;
        }
        return ;
    }
    //check if key already exists
    if (ft_fnd_env(command->env, *ft_split(command->token->next->content, '=')))
    {
        word = ft_strchr(command->token->next->content, '=') + 1;
        *ft_strchr(command->token->next->content, '=') = 0;
        ft_update_env(command->env, command->token->next->content, word);
        return ;
    }
    //else, add new key
    if(ft_strchr(command->token->next->content, '='))
    {
        word = ft_strchr(command->token->next->content, '=') + 1;
        *ft_strchr(command->token->next->content, '=') = 0;
        ft_add_env_back(&command->env, ft_new_env(command->token->next->content, word));
    }
}
