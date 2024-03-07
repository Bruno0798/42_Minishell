/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunolopes <brunolopes@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 12:42:09 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/03/06 20:29:52 by brunolopes       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env *order_list(t_env *env)
{
    t_env *current;
    t_env *tmp;
    char *key;
    char *value;

    current = env;
    while(current->next)
    {
        tmp = current->next;
        while(tmp)
        {
            if(ft_strcmp(current->key, tmp->key) > 0)
            {
                key = current->key;
                value = current->value;
                current->key = tmp->key;
                current->value = tmp->value;
                tmp->key = key;
                tmp->value = value;
            }
            tmp = tmp->next;
        }
        current = current->next;
    }
    return env;
}

void print_env(t_env *env)
{
    t_env *current;

    current = order_list(env);
    while(current)
    {
        printf("declare -x %s=\"%s\"\n", current->key, current->value);
        current = current->next;
    }

}

void ft_export(t_commands *command)
{
    char    *word;

    if(!command->token->next) /* If there is no token to export, print the environment variables */
    {
        print_env(command->env);
        return ;
    }
    if(!(*(ft_strchr(command->token->next->content, '=') + 1)))
    {
        printf("Error\n");
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
        word = ft_strchr(command->token->next->content, '=') + 1;
        *ft_strchr(command->token->next->content, '=') = 0;
        if(!*(command->token->next->content))
        {
            printf("Error\n");
            return ;
        }
        ft_add_env_back(&command->env, ft_new_env(command->token->next->content, word));
    }
}
