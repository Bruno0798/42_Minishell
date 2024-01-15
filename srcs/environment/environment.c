/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsousa-d <bsousa-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 10:25:44 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/01/15 14:44:15 by bsousa-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void get_env(t_env **env,char **envp)
{
	int i;
	char *key;
	char *value;
	char *equal_sign;

	i = 0;
	ft_bzero(env, sizeof(t_env));
	while(envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		key = ft_substr(envp[i], 0, equal_sign - envp[i]);
		value = equal_sign + 1;
		ft_add_env_back(env, ft_new_env(key, value));
		free(key);
		i++;
	}
}

void	ft_add_env_back(t_env **env_lst, t_env *new_env)
{
	t_env	*current;

	if (!*env_lst)
	{
		*env_lst = new_env;
		return ;
	}
	current = *env_lst;
	while (current && current->next)
		current = current->next;
	current->next = new_env;
}

t_env	*ft_new_env(char *key, char *value)
{
	t_env	*new_node;

	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

char **get_path(t_env *env_lst)
{
    t_env *env;
    char **paths;
    
    env = env_lst;
	paths = NULL;
    while(env)
    {
        if(strcmp(env->key, "PATH") == 0)
        {
            paths = ft_split(env->value, ':');
            return (paths);
        }
        env = env->next;
    }
    free(paths);
    return (NULL);
}
