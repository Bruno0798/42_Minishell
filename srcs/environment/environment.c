/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brpereir <brpereir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 10:25:44 by bsousa-d          #+#    #+#             */
/*   Updated: 2024/07/02 17:08:50 by brpereir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*extract_home_path(const char *pwd)
{
	char	*home;
	char	*third_slash;
	size_t	length;

	home = NULL;
	third_slash = ft_strchr(pwd, '/');
	if (third_slash)
	{
		third_slash = ft_strchr(third_slash + 1, '/');
		if (third_slash)
		{
			third_slash = ft_strchr(third_slash + 1, '/');
			if (third_slash)
			{
				length = third_slash - pwd;
				home = malloc(length + 1);
				if (home)
				{
					ft_strlcpy(home, pwd, length);
					home[length] = '\0';
				}
			}
		}
	}
	return (home);
}

void	init_env(t_env **env, char **envp)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal_sign;

	i = 0;
	ft_bzero(env, sizeof(t_env));
	if (envp[0] == NULL)
		ft_add_env_back(env, ft_new_env("SHLVL", "1", 1));
	else
		while (envp[i])
		{
			equal_sign = ft_strchr(envp[i], '=');
			key = ft_substr(envp[i], 0, equal_sign - envp[i]);
			value = equal_sign + 1;
			ft_add_env_back(env, ft_new_env(key, value, 1));
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

t_env	*ft_new_env(char *key, char *value, int visible)
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
	new_node->visible = visible;
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

char	**get_path(t_env *env_lst)
{
	t_env	*env;
	char	**paths;

	env = env_lst;
	paths = NULL;
	while (env)
	{
		if (ft_strcmp(env->key, "PATH") == 0)
		{
			paths = ft_split(env->value, ':');
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
	i = 0;
	while (tmp)
	{
		temp = ft_strjoin(tmp->key, "=");
		arr[i] = ft_strjoin(temp, tmp->value);
		free(temp);
		tmp = tmp->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

int	env_size(t_env *env)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (++i);
}

void	ft_update_env(t_env *env_lst, char *key, char *replace, int visible)
{
	t_env	*curr;

	curr = env_lst;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			free(curr->value);
			curr->value = ft_strdup(replace);
			curr->visible = visible;
			return ;
		}
		curr = curr->next;
	}
}

t_env	*ft_fnd_env(t_env *env, char *search)
{
	t_env	*current;

	current = env;
	if (!search)
		return (NULL);
	while (current)
	{
		if (!ft_strcmp(current->key, search))
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	free_double_pointer_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
