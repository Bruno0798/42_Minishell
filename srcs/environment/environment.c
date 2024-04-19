/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 10:25:44 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/04/19 13:37:11 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void init_env(t_env **env,char **envp)
{
	int i;
	char *key;
	char *value;
	char *equal_sign;

	i = 0;
	ft_bzero(env, sizeof(t_env)); /* Set the environment to 0 */
	while(envp[i])
	{
		equal_sign = ft_strchr(envp[i], '='); /* Get the equal sign */
		key = ft_substr(envp[i], 0, equal_sign - envp[i]); /* Get the key */
		value = equal_sign + 1; /* Get the value */
		ft_add_env_back(env, ft_new_env(key, value)); /* Add the environment variable */
		free(key); /* Free the key */
		i++;
	}
}

void	ft_add_env_back(t_env **env_lst, t_env *new_env)
{
	t_env	*current;

	if (!*env_lst) /* If the environment list is empty, set the new environment */
	{
		*env_lst = new_env;
		return ;
	}
	current = *env_lst;
	while (current && current->next) /* Go to the last node */
		current = current->next;
	current->next = new_env; /* Set the next node to the new environment */
}

t_env	*ft_new_env(char *key, char *value)
{
	t_env	*new_node;

	new_node = ft_calloc(1, sizeof(t_env)); /* Allocate memory for the new environment */
	if (!new_node) /* If the new environment is null, return null */
		return (NULL);
	new_node->key = ft_strdup(key); /* Set the key */
	if (value)
		new_node->value = ft_strdup(value); /* Set the value */
	else
		new_node->value = NULL;
	if (!new_node->key) /* If the key is null, free the new environment and return null */
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
    while(env) /* Loop through the environment list */
    {
        if(strcmp(env->key, "PATH") == 0) /* If the key is PATH, split the value and return the paths */
        {
            paths = ft_split(env->value, ':'); /* Split the value */
            return (paths);
        }
        env = env->next;
    }
    return (NULL);
}

char	**ft_env_to_arr(t_env *env)
{
	char	**arr;
	char	*temp;
	t_env	*tmp;
	int		i;

	arr = malloc(sizeof(char *) * env_size(env));
	tmp = env;
	i = -1;
	while (tmp)
	{
		temp = ft_strjoin(tmp->key, "=");
		arr[++i] = ft_strjoin(temp, tmp->value);
		tmp = tmp->next;
	}
	arr[i] =  NULL;
	return (arr);
}

int	env_size(t_env *env)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = env;
	while(tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (++i);
}

void ft_update_env(t_env *env_lst, char *key, char *replace)
{
	t_env *curr;

	curr = env_lst;

	while(curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			free(curr->value);
			curr->value = ft_strdup(replace);
			return ;
		}
		curr = curr->next;
	}
}
t_env	*ft_fnd_env(t_env *env, char *search)
{
	t_env	*current;

	current = env;
	if(!search)
		return NULL;
	while (current)
	{
		if (!ft_strcmp(current->key, search))
			return (current);
		current = current->next;
	}
	return (NULL);
}
